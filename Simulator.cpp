/*
 * This is the backbone of TradeR. It will be responsible for
 * calling functions to read and parse the Configurations.ini file,
 * the universe file and the price data files. It will also be responsible
 * for simulating through each day of the Trading Dates on each of
 * the ticker's price data. Meanwhile it will store the necessary data
 * of individual tickers in Trading Objects and also keep track of
 * the simulation details such as daily PnL data and so on.
 */

#include "Simulator.h"

/**/
/*

 Simulator::Simulator()

 NAME

    Simulator::Simulator - constructor for the Simulator Class

 SYNOPSIS

    Simulator::Simulator(int argc, char** argc);

        argc    --> the number of arguments passed from the command line
        argv    --> name of the configuration file

 DESCRIPTION

    This is the constructor of the Simulator Class. This function calls
    functions that initialize member variables and constructs individual
    trading objects.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
Simulator::Simulator(int argc, char** argv) : m_argc(argc), m_configFileName(argv){
    initializeModel();
    initializeTradingObjects();
}


/**/
/*

 Simulator::initializeModel()

 NAME

    Simulator::initializeModel - initializes member variables

 SYNOPSIS

    void Simulator::initializeModel();

 DESCRIPTION

    This function will be responsible for initializing member variables by accessing
    the parsed configuration file's data from the ConfigParser Object.

    It also opens ofstream file objects according to the config details.


 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
void Simulator::initializeModel() {
    ConfigParser configs(m_configFileName[1]);
    m_configs = &configs;
    m_configs->ParseData();
    //m_configs->PrintConfigs();

    m_db = &(Database::GetInstanceOfDb(m_configs->GetConfigValues("global", "universe_file"),
            m_configs->GetConfigValues("global", "price_data")));

    m_entrySignal = std::stod(m_configs->GetConfigValues("entry_conditions", "signal_threshold"));
    m_exitSignal = std::stod(m_configs->GetConfigValues("exit_conditions", "signal_threshold"));
    m_availableCapital = std::stod(m_configs->GetConfigValues("portfolio", "capital"));
    m_maxCapitalPerStock = std::stod(m_configs->GetConfigValues("portfolio", "maximum_capital_object"));
    m_maxDaysInPosition = std::stoi(m_configs->GetConfigValues("portfolio", "max_position_days"));
    m_maxPositionPerStock = std::stoi(m_configs->GetConfigValues("portfolio", "max_positions"));

    m_dailyReport = std::stoi(m_configs->GetConfigValues("publication", "daily"));
    m_tradingReport = std::stoi(m_configs->GetConfigValues("publication", "trading"));
    m_sharpeRatioReport = std::stoi(m_configs->GetConfigValues("publication", "sharperatio"));


    if (m_dailyReport == 1) {
        m_dailyStatistics.open("DailyStats.csv");
        m_dailyStatistics << "Date,Total Pos, Total Long,Total Short,Dollar PnL,Cumulative PnL,Total Market Value,Net Market Value \n";
    }

    if (m_tradingReport == 1) {
        m_tradingStatistics.open("TradingStats.txt");
    }

    if (m_sharpeRatioReport == 1){
        m_sharpeRatioStatistics.open("SharpeRatioReport.csv");
        m_sharpeRatioYearly.open("SharpeRatioYearly.txt");
    }
}
/*void Simulator::initializeModel();*/


/**/
/*

 Simulator::initializeTradingObjects()

 NAME

    Simulator::initializeTradingObjects - constructs trading objects

 SYNOPSIS

    void Simulator::initializeTradingObjects();

 DESCRIPTION

    This function will be responsible for initializing Trading Objects
    for individual tickers whose price data is available.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
void Simulator::initializeTradingObjects() {
    std::cout << "Creating Trading Objects..." << std::endl;

    std::vector<std::string> tickers_in_universe = m_db->getTickers();

    for (auto const& ticker : tickers_in_universe) {
        TradingObject tradingObject(ticker, m_maxCapitalPerStock);
        tradingObject.setIsInLongPosition(false);
        tradingObject.setIsInShortPosition(false);
        tradingObject.setCurrSharesHeld(0);
        m_tradingObjects.push_back(tradingObject);
    }
    std::cout << "Trading Objects Created #: " << m_tradingObjects.size() << std::endl;
}


/**/
/*

 Simulator::runSim()

 NAME

    Simulator::runSim - performs simulation

 SYNOPSIS

    void Simulator::runSim();

 DESCRIPTION

    This function will be responsible for running the entire financial simulator.
    It has two nested loops. The outer loop runs on the trading dates and the inner
    loop runs on the individual trading objects.

    The function also keeps track of the necessary daily data. Towards the end of
    each outer loop, it records statistics for that date. Then, in the end this
    function calls the function to generate necessary reports.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
void Simulator::runSim() {
    std::cout << "Running Simulation..." << std::endl;

    std::vector<std::string> tickers_in_universe = m_db->getTickers();
    std::vector<DateTime> trading_dates = m_db->getTradingDates();
    std::cout << "Total # days to Simulate: " << trading_dates.size() << std::endl;

    m_totalCumulativePnl = 0;

    for (int index = 0; index < trading_dates.size(); index++) {
        // reset the daily data variables
        m_totalDailyPnL = 0;
        m_totalLongPositions = 0;
        m_totalShortPositions = 0;
        m_totalPositions = 0;
        m_netMarketValue = 0;
        m_totalMarketValue = 0;
        m_currDate = &trading_dates.at(index);

        for (auto &trdObj : m_tradingObjects) {

            std::string tickerName = trdObj.getTickerName();
            TickerBlock& tickerBlock = (*m_db)[tickerName];

            double signal = calculateSignal(tickerBlock, index);

            trdObj.addSignal(signal);

            // handle missing data
            if (signal == -999){
                trdObj.addShares(0);
                trdObj.addCapitalInStock(0);
                trdObj.addPnLData(0);
                trdObj.addTransaction(0);
                trdObj.addDailyReturn(0);
                continue;
            }

            double stockPrice = tickerBlock[TickerBlock::FIELD_CLOSE].at(index);

            double yesterdayPrice;

            if (index == 0 || stockPrice == -999) {
                yesterdayPrice = 0;
            }
            else {
                yesterdayPrice = tickerBlock[TickerBlock::FIELD_CLOSE].at(index - 1);
            }

            trade(stockPrice, trdObj, signal);

            double positions_held = trdObj.getCurrSharesHeld();

            // calculations after trading the stock for daily data
            if (trdObj.isInShortPosition()){
                m_totalShortPositions -= positions_held;
            }
            if (trdObj.isInLongPosition()){
                m_totalLongPositions += positions_held;
            }

            m_totalPositions += positions_held;
            m_totalDailyPnL += (stockPrice - yesterdayPrice) * positions_held;
            m_totalCumulativePnl += m_totalDailyPnL;
            m_totalMarketValue += m_totalPositions * stockPrice;
            m_netMarketValue += positions_held * stockPrice;
        }
        m_dailyPnL.push_back(m_totalDailyPnL);
        recordStats();
    }

    std::cout << "Simulation Ended!" << std::endl;
    generateReports();

}
/*void Simulator::runSim();*/


/**/
/*

 Simulator::trade()

 NAME

    Simulator::trade - opens and closes position accordingly

 SYNOPSIS

    void Simulator::trade(double &a_price, TradingObject &a_trdObject, double a_signal);

        a_price     --> price of the stock
        a_trdObject --> trading object for the ticker
        a_signal    --> signal of the trading object

 DESCRIPTION

    This function will be responsible for closing and opening positions according to
    the signal received and also by checking the type of position the trading object
    is currently at.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
void Simulator::trade(double &a_price, TradingObject &a_trdObject, double a_signal) {
    // first close any position accordingly
    if (a_trdObject.isInLongPosition() || a_trdObject.isInShortPosition()){
        closePosition(a_price, a_trdObject, a_signal);
    }
    // if position is closed then neither positions will be true,
    // so we will open new positions accordingly
    if (!a_trdObject.isInShortPosition() && !a_trdObject.isInLongPosition()) {
        openPosition(a_price, a_trdObject, a_signal);
    }
}


/**/
/*

 Simulator::closePosition()

 NAME

    Simulator::closePosition - closes the position held by Trading object

 SYNOPSIS

    void Simulator::closePosition(double &a_price, TradingObject &a_trdObject, double a_signal);

        a_price     --> price of the stock
        a_trdObject --> Trading Object of the ticker
        a_signal    --> signal of the trading object

 DESCRIPTION

    This function will be responsible for closing the position of the Trading Object.

    If the trading object is in long position and the signal is less than or equal to
    the exit signal, then it sells the shares held. If the trading object is in short
    position and the signal is greater than or equal to the negative exit signal then
    it buys back the short sold shares. Else it does nothing and adds 0 to the necessary
    variables of the trading object. On each step it calls the recordTransaction()
    function to record the transaction made.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
void Simulator::closePosition(double &a_price, TradingObject &a_trdObject, double a_signal) {

    double numSharesHeld = a_trdObject.getCurrSharesHeld();

    if (a_trdObject.isInLongPosition() && a_signal <= m_exitSignal){

        sell(a_price, a_trdObject, numSharesHeld);

        a_trdObject.setIsInLongPosition(false);
        a_trdObject.setIsInShortPosition(false);
        a_trdObject.closeTransaction(m_currDate, a_signal, a_price);

        recordTransaction(a_trdObject);

        return;
    }

    if (a_trdObject.isInShortPosition() && a_signal >= -m_exitSignal){

        buy(a_price, a_trdObject, numSharesHeld);

        a_trdObject.setIsInShortPosition(false);
        a_trdObject.setIsInLongPosition(false);
        a_trdObject.closeTransaction(m_currDate, a_signal, a_price);

        recordTransaction(a_trdObject);

        return;
    }

    // if it reaches this point, then do nothing
    a_trdObject.addShares(0);
    a_trdObject.removeCapitalInStock(0);
    a_trdObject.addTransaction(0);
    a_trdObject.addDailyReturn(0);
}
/*void Simulator::closePosition(double &a_price, TradingObject &a_trdObject, double a_signal);*/

/**/
/*

 Simulator::openPosition()

 NAME

    Simulator::openPosition - opens the position of the trading object

 SYNOPSIS

    void Simulator::openPosition(double &a_price, TradingObject &a_trdObject, double a_signal);

        a_price     --> price of the stock
        a_trdObject --> Trading Object of the ticker
        a_signal    --> signal of the trading object

 DESCRIPTION

    This function will be responsible for opening positions for the trading object.
    First it calculates the capital to invest in the stock. Then, it calculates the
    number of positions to buy or short sell accordingly.

    If the signal is greater than or equal to the entry signal, it buys long positions.
    If the signal is less than or equal to the negative entry signal, it sells short
    positions. Else, it holds no positions. It adds 0 to the necessary variables of the
    trading object

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
void Simulator::openPosition(double &a_price, TradingObject &a_trdObject, double a_signal) {

    double capitalToInvest = (a_trdObject.getCapInStock() > m_maxCapitalPerStock) ? m_maxCapitalPerStock :
            a_trdObject.getCapInStock();

    double numberOfPositions = Utilities::roundOff (capitalToInvest / a_price, 100);

    if (a_signal >= m_entrySignal) {
        // go long, so buy
        a_trdObject.setIsInLongPosition(true);
        a_trdObject.setIsInShortPosition(false);
        a_trdObject.openTransaction(m_currDate, a_signal, numberOfPositions, a_price);
        buy(a_price, a_trdObject, numberOfPositions);
    }
    else if (a_signal <= -m_entrySignal) {
        // go short, so sell
        a_trdObject.setIsInShortPosition(true);
        a_trdObject.setIsInLongPosition(false);
        a_trdObject.openTransaction(m_currDate, a_signal, -numberOfPositions, a_price);
        sell(a_price, a_trdObject, -numberOfPositions);
    }
    else {
        // do nothing
        a_trdObject.addShares(0);
        a_trdObject.removeCapitalInStock(0);
        a_trdObject.addPnLData(0);
        a_trdObject.addTransaction(0);
        a_trdObject.addDailyReturn(0);
    }
}
/*void Simulator::openPosition(double &a_price, TradingObject &a_trdObject, double a_signal);*/

/**/
/*

 Simulator::buy()

 NAME

    Simulator::buy - buys shares

 SYNOPSIS

    void Simulator::buy(double &a_price, TradingObject &a_trdObject, double a_numShares);

        a_price     --> price of stock
        a_trdObject --> Trading Object of the ticker
        a_numShares --> Number of positions to buy

 DESCRIPTION

    This function is responsible for buying the shares of stock.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
void Simulator::buy(double &a_price, TradingObject &a_trdObject, double a_numShares) {
    a_trdObject.addShares(a_numShares);
    a_trdObject.removeCapitalInStock(a_numShares * a_price);
    a_trdObject.setCurrSharesHeld(a_numShares);
}


/**/
/*

 Simulator::sell()

 NAME

    Simulator::sell - sells the stocks

 SYNOPSIS

    void Simulator::sell(double &a_price, TradingObject &a_trdObject, double a_numShares);

        a_price     --> price of the stock
        a_trdObject --> Trading Object
        a_numShares --> Number of positions to sell

 DESCRIPTION

    This function is responsible for selling the shares of stock. It calculates
    the number of positions to sell. If the trading object is in short position,
    then it removes the rounded off value of (capital to invest per stock / stock price)
    to nearest hundred, else it removes the amount of shares that is being held.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
void Simulator::sell(double &a_price, TradingObject &a_trdObject, double a_numShares) {
    a_trdObject.removeShares(a_numShares);
    a_trdObject.addCapitalInStock(a_numShares * a_price);
    a_trdObject.setCurrSharesHeld(a_numShares);
    a_trdObject.addDailyReturn(a_numShares * a_price);
}


/**/
/*

 Simulator::recordStats()

 NAME

    Simulator::recordStats - records daily statistics

 SYNOPSIS

    void Simulator::recordStats();

 DESCRIPTION

    This function writes the daily statistic data into the file stream
    responsible for daily report

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
void Simulator::recordStats() {
    if (m_dailyReport != 1)
        return;

    m_dailyStatistics << m_currDate->getDate() << "," \
            << m_totalPositions << "," \
            << m_totalLongPositions << "," \
            << m_totalShortPositions << "," \
            << m_totalDailyPnL << "," \
            << m_totalCumulativePnl << "," \
            << m_totalMarketValue << "," \
            << m_netMarketValue << "\n";
}


/**/
/*

 Simulator::recordTransaction()

 NAME

    Simulator::recordTransaction - records statistics for trading report

 SYNOPSIS

    void Simulator::recordTransaction(TradingObject &a_obj);

        a_obj   --> Trading Object

 DESCRIPTION

    This function is responsible for recording transaction statistics by
    writing the data into the file stream responsible for generating the
    trading report

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/10/2019

 */
/**/
void Simulator::recordTransaction(TradingObject &a_obj) {
    if (m_tradingReport != 1)
        return;

    m_tradingStatistics << "Ticker Name: " << a_obj.getTickerName() << "\t" \
            << "Number of Positions: " << a_obj.getCurrSharesHeld() << "\t" \
            << "Days in Position: " << a_obj.getTransactionDaysInPosition() << "\n" \

            << "Position Opened on: " << a_obj.getTransactionOpenDate().getDate() << "\t" \
            << "Position Closed on: " << a_obj.getTransactionCloseDate().getDate() << "\n" \

            << "Signal on Open: " << a_obj.getTransactionOpenSignal() << "\t" \
            << "Signal on Close: " << a_obj.getTransactionCloseSignal() << "\n" \

            << "Price on Open: " << a_obj.getTransactionOpenPrice() << "\t" \
            << "Price on Close: " << a_obj.getTransactionClosePrice() << "\n" \
            << "\n\n";
}


/**/
/*

 Simulator::generateReports()

 NAME

    Simulator::generateReports - generates all the reports

 SYNOPSIS

    void Simulator::generateReports();

 DESCRIPTION
    This function is responsible for closing the file stream objects for
    daily statistics and trading report. It also calls functions to generate
    sharpe ratios.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
void Simulator::generateReports() {
    std::cout << "Generating Reports..." << std::endl;

    // sharpe ratio report
    if (m_sharpeRatioReport == 1) {
        //recordSharpeRatio();
        recordSharpeYearly();
    }

    if (m_tradingReport == 1) {
        m_tradingStatistics.close();
        std::cout << "Trading Report Generated!" << std::endl;
    }

    if (m_dailyReport == 1) {
        m_dailyStatistics.close();
        std::cout << "Daily Statistics Report Generated!" << std::endl;
    }

}
/*void Simulator::generateReports();*/


/**/
/*

 Simulator::recordSharpeRatio()

 NAME

    Simulator::recordSharpeRatio - generates the sharpe ratio report

 SYNOPSIS

    void Simulator::generateReports();

 DESCRIPTION
        This function is responsible for generating Daily Sharpe Ratio
        and writing them to a file stream object for each trading objects.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/

void Simulator::recordSharpeRatio() {

    std::cout << "Sharpe Ratio Report" << std::endl;

    for (auto &trdObj : m_tradingObjects) {
        trdObj.calculateDailySharpeRatio();
    }

    std::cout << "Sharpe Ratios Calculated!" << std::endl;

    std::vector<std::string> tickers = m_db->getTickers();

    // Write the header
    m_sharpeRatioStatistics << "Date, Date";

    for (auto &ticker : m_tradingObjects) {
        m_sharpeRatioStatistics << "," << ticker.getTickerName();
    }

    m_sharpeRatioStatistics << "\n";

    // Write the data
    std::vector<DateTime> trading_dates = m_db->getTradingDates();

    for (int i = 0; i < trading_dates.size(); i++) {
        DateTime* currDate = &trading_dates.at(i);

        std::string strDate = std::to_string(currDate->getYear()) + "/"
                              + std::to_string(currDate->getMonth()) + "/"
                              + std::to_string(currDate->getDay());

        int intDate = currDate->getDate();

        m_sharpeRatioStatistics << intDate << "," << strDate;

        for (auto &trdObj : m_tradingObjects) {
            m_sharpeRatioStatistics << "," << trdObj.getSharpeRatio(i);
        }

        m_sharpeRatioStatistics << "\n";
    }

    m_sharpeRatioStatistics.close();

    std::cout << "Sharpe Ratio Report Generated!" << std::endl;
}

/**/
/*

 Simulator::recordSharpeYearly()

 NAME

    Simulator::recordSharpeYearly - calculates the yearly sharpe ratio

 SYNOPSIS

    void Simulator::recordSharpeYearly()

 DESCRIPTION

    This function calculates the yearly sharpe ratio and writes them
    to the ofstream object.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    9/9/2019

 */
void Simulator::recordSharpeYearly() {
    std::cout << "Sharpe Ratio Yearly Report" << std::endl;

    std::vector<DateTime> trading_dates = m_db->getTradingDates();

    m_sharpeRatioYearly << "Year\tSharpe Ratio\n";
    int start = 0;
    for (int i = 0; i < trading_dates.size() - 1; ++i){
        if (trading_dates.at(i).getYear() != trading_dates.at(i+1).getYear() || i == trading_dates.size() - 2){
            // then index i is the end of year index
            std::vector<double> totalDailyReturns(m_dailyPnL.begin() + start, m_dailyPnL.begin() + i);

            // calculate the yearly sharpe ratio
            double averageOfReturns = Utilities::average(totalDailyReturns);
            double standardDeviation = Utilities::standardDeviation(totalDailyReturns);
            double sharpeRatio = sqrt(252) * averageOfReturns / standardDeviation;
            m_sharpeRatioYearly << trading_dates.at(i).getYear() << "\t" << sharpeRatio << "\n";

            start = i;
        }
    }

    m_sharpeRatioYearly.close();
}
