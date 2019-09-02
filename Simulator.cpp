/*
 * This is the backbone of TradeR. It will be responsible for
 * reading and parsing the Configurations.ini file, the universe
 * file and the price data files. It will then be responsible for
 * simulating through each day of the Trading Dates on each of
 * the ticker's data. Meanwhile it will store the necessary data
 * of individual tickers in Trading Objects and also keep track of
 * the simulation details such as daily PnL data and so on.
 */

#include "Simulator.h"
#include <iostream>

int count = 1;


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
Simulator::Simulator(int argc, char** argv) : m_argc(argc), m_configFileName(argv){
    initializeModel();
    initializeTradingObjects();
}


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
void Simulator::initializeModel() {
    ConfigParser configs(m_configFileName[1]);
    m_configs = &configs;
    m_configs->ParseData();
//    m_configs->PrintConfigs();

    m_db = &(Database::GetInstanceOfDb(m_configs->GetConfigValues("global", "universe_file"),
            m_configs->GetConfigValues("global", "price_data")));

    m_entrySignal = std::stod(m_configs->GetConfigValues("entry_conditions", "signal_threshold"));
    m_exitSignal = std::stod(m_configs->GetConfigValues("exit_conditions", "signal_threshold"));
    m_availableCapital = std::stod(m_configs->GetConfigValues("portfolio", "capital"));
    m_maxCapitalPerStock = std::stod(m_configs->GetConfigValues("portfolio", "maximum_capital_object"));
    m_maxDaysInPosition = std::stoi(m_configs->GetConfigValues("portfolio", "max_position_days"));
    m_maxPositionPerStock = std::stoi(m_configs->GetConfigValues("portfolio", "max_positions"));

    m_monthlyReport = std::stoi(m_configs->GetConfigValues("publication", "monthly"));
    m_dailyReport = std::stoi(m_configs->GetConfigValues("publication", "daily"));
    m_tradingReport = std::stoi(m_configs->GetConfigValues("publication", "trading"));
    m_sharpeRatioReport = std::stoi(m_configs->GetConfigValues("publication", "sharperatio"));

    m_out.open("TradingSummary.txt");

    if (m_dailyReport == 1) {
        m_dailyStatistics.open("DailyStats.csv");
        m_dailyStatistics << "Date,Total Pos, Total Long,Total Short,Dollar PnL,Cumulative PnL,Total Market Value,Net Market Value \n";
    }

    if (m_tradingReport == 1) {
        m_tradingStatistics.open("TradingStats.txt");
    }

    if (m_sharpeRatioReport == 1){
        m_sharpeRatioStatistics.open("SharpeRatioReport.csv");
    }
}


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
void Simulator::initializeTradingObjects() {
    std::cout << "Creating Trading Objects..." << std::endl;

    std::vector<std::string> tickers_in_universe = m_db->getTickers();
//    std::cout << tickers_in_universe.size() << std::endl;
    for (auto const& ticker : tickers_in_universe) {
        TradingObject tradingObject(ticker);
        tradingObject.setIsInLongPosition(false);
        tradingObject.setIsInShortPosition(false);
        tradingObject.setCurrSharesHeld(0);
        m_tradingObjects.push_back(tradingObject);
//        std::cout << "Trd Obj Ticker Name: " << tradingObject.getTickerName() << std::endl;
    }
    std::cout << "Trading Object Created #: " << m_tradingObjects.size() << std::endl;
}


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
void Simulator::runSim() {
    std::cout << "Running Simulation..." << std::endl;

    std::vector<std::string> tickers_in_universe = m_db->getTickers();
    std::vector<DateTime> trading_dates = m_db->getTradingDates();
    std::cout << "Dates: " << trading_dates.size() << std::endl;
    // initialize the overall member variables;
    m_capitalInStock = 0;
    m_totalCapitalInvested = 0;
    m_totalCapitalReturned = 0;
    m_totalCumulativePnl = 0;

    for (int index = 0; index < trading_dates.size(); index++) {
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
                trdObj.incTrade();
                trdObj.addShares(0);
                trdObj.removeCapitalInStock(0);
                trdObj.addTransaction(0);
                trdObj.addDailyReturn(0);
                m_out << m_currDate->getDate() << "\n";// << count << " added daily return: 0\n";
                count++;
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

            // calculations after trade
            if (trdObj.isInShortPosition()){
                m_totalShortPositions -= positions_held;
                m_totalPositions += -positions_held;
            }
            else {
                m_totalLongPositions += positions_held;
                m_totalPositions += positions_held;
            }

            m_totalDailyPnL += (stockPrice - yesterdayPrice) * positions_held;
            m_totalCumulativePnl += m_totalDailyPnL;
            m_totalMarketValue += m_totalPositions * stockPrice;
            m_netMarketValue += positions_held * stockPrice;
        }

        recordStats();
    }

    std::cout << "Simulation Ended!" << std::endl;
    generateReports();
}


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
void Simulator::trade(double &a_price, TradingObject &a_trdObject, double a_signal) {
    // first close any position accordingly
    closePosition(a_price, a_trdObject, a_signal);

    // if position is closed then neither positions will be true, so we will open new positions accordingly
    if (!a_trdObject.isInShortPosition() && !a_trdObject.isInLongPosition()) {
        openPosition(a_price, a_trdObject, a_signal);
    }
}


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
void Simulator::closePosition(double &a_price, TradingObject &a_trdObject, double a_signal) {

    double numSharesHeld = a_trdObject.isInShortPosition() ?
                           -(Utilities::roundOff(m_maxCapitalPerStock / a_price, 100)) : a_trdObject.getCurrSharesHeld();

    if (a_trdObject.isInLongPosition() && a_signal <= m_exitSignal){
        sell(a_price, a_trdObject);

        a_trdObject.addDailyReturn(numSharesHeld * a_price);
        a_trdObject.incTrade();
        //a_trdObject.incTrade();

        m_out << m_currDate->getDate() << "\n";// << count << " added daily return: " <<numSharesHeld * a_price<< "\n";
        count++;

        a_trdObject.setIsInLongPosition(false);
        a_trdObject.setIsInShortPosition(false);
        a_trdObject.closeTransaction(m_currDate, a_signal, a_price);

        recordTransaction(a_trdObject);
    }

    else if (a_trdObject.isInShortPosition() && a_signal >= -m_exitSignal){

        buy(a_price, a_trdObject);

        a_trdObject.addDailyReturn(numSharesHeld * a_price);
        //a_trdObject.incTrade();

        m_out << m_currDate->getDate() << "\n";// << count << " added daily return: " << numSharesHeld * a_price << "\n";
        count++;

        a_trdObject.setIsInShortPosition(false);
        a_trdObject.setIsInLongPosition(false);

        a_trdObject.closeTransaction(m_currDate, a_signal, a_price);

        recordTransaction(a_trdObject);
    }

    else if ((a_trdObject.isInShortPosition() && a_signal < -m_exitSignal) || (a_trdObject.isInLongPosition() && a_signal > m_exitSignal)){
        //do nothing
        a_trdObject.addShares(0);
        a_trdObject.removeCapitalInStock(0);
        a_trdObject.addTransaction(0);
        a_trdObject.addDailyReturn(0);
        //a_trdObject.incTrade();
        m_out << m_currDate->getDate() << "\n";// << count << " added daily return: 0" << "\n";
        count++;
    }

    else {
        return;
    }
}


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
void Simulator::openPosition(double &a_price, TradingObject &a_trdObject, double a_signal) {
    if (a_signal >= m_entrySignal) {
        // go long, go buy
        a_trdObject.setIsInLongPosition(true);
        a_trdObject.setIsInShortPosition(false);

        double numberOfPositions = a_trdObject.isInShortPosition() ?
            -(a_trdObject.getCurrSharesHeld()) : Utilities::roundOff (m_maxCapitalPerStock / a_price, 100);

        a_trdObject.openTransaction(m_currDate, a_signal, numberOfPositions, a_price);

        buy(a_price, a_trdObject);

        return;
    }
    else if (a_signal <= -m_entrySignal) {
        // go short, go sell
        a_trdObject.setIsInShortPosition(true);
        a_trdObject.setIsInLongPosition(false);

        double numberOfPositions = a_trdObject.isInShortPosition() ?
            -(Utilities::roundOff(m_maxCapitalPerStock / a_price, 100)) : a_trdObject.getCurrSharesHeld();

        a_trdObject.openTransaction(m_currDate, a_signal, numberOfPositions, a_price);

        sell(a_price, a_trdObject);

        return;
    }
    else {
        // do nothing
        a_trdObject.addShares(0);
        a_trdObject.removeCapitalInStock(0);
        a_trdObject.addTransaction(0);
        a_trdObject.addDailyReturn(0);
        //a_trdObject.incTrade();

        m_out << m_currDate->getDate() << "\n"; // << count << " added daily return: 0\n";
        count++;
    }
}


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
void Simulator::buy(double &a_price, TradingObject &a_trdObject) {
    double numberOfPositionsToBuy = a_trdObject.isInShortPosition() ?
            -(a_trdObject.getCurrSharesHeld()) : Utilities::roundOff (m_maxCapitalPerStock / a_price, 100);

    a_trdObject.addShares(numberOfPositionsToBuy);
    a_trdObject.removeCapitalInStock(numberOfPositionsToBuy * a_price);
    //a_trdObject.addTransaction(numberOfPositionsToBuy);
    a_trdObject.setCurrSharesHeld(numberOfPositionsToBuy);
}


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
void Simulator::sell(double &a_price, TradingObject &a_trdObject) {
    double numSharesHeld = a_trdObject.isInShortPosition() ?
            -(Utilities::roundOff(m_maxCapitalPerStock / a_price, 100)) : a_trdObject.getCurrSharesHeld();

    a_trdObject.removeShares(numSharesHeld);
    a_trdObject.addCapitalInStock(numSharesHeld * a_price);
    //a_trdObject.addTransaction(numSharesHeld);
    a_trdObject.setCurrSharesHeld(numSharesHeld);

    //a_trdObject.addDailyReturn(numSharesHeld * a_price);
}


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

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

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
void Simulator::recordTransaction(TradingObject &a_obj) {
    if (m_tradingReport != 1)
        return;

    m_tradingStatistics << "Ticker Name: " << a_obj.getTickerName() << "\t" \
            << "Number of Positions: " << a_obj.getTransactionNumShares() << "\t" \
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

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
void Simulator::generateReports() {
    std::cout << "Generating Reports..." << std::endl;

    // sharpe ratio report
    if (m_sharpeRatioReport == 1) {

        std::cout << "Sharpe Ratio Report" << std::endl;

        for (auto &trdObj : m_tradingObjects) {
            trdObj.calculateDailySharpeRatio();
            std::cout << trdObj.getTickerName() << " " << trdObj.getTrade() << std::endl;

        }

        std::cout << "Sharpe Ratios Calculated!" << std::endl;

//        std::vector<std::string> tickers = m_db->getTickers();
//
//        // Write the header
//        m_sharpeRatioStatistics << "Date, Date";
//
//        for (auto &ticker : m_tradingObjects) {
//            m_sharpeRatioStatistics << "," << ticker.getTickerName();
//        }
//
//        m_sharpeRatioStatistics << "\n";
//
//        // Write the data
//        std::vector<DateTime> trading_dates = m_db->getTradingDates();
//
//        for (int i = 1; i < trading_dates.size(); i++) {
//            DateTime* currDate = &trading_dates.at(i);
//
//            std::string strDate = std::to_string(currDate->getYear()) + "/"
//                                + std::to_string(currDate->getMonth()) + "/"
//                                + std::to_string(currDate->getDay());
//
//            int intDate = currDate->getDate();
//
//            m_sharpeRatioStatistics << intDate << "," << strDate;
//
//            for (auto &trdObj : m_tradingObjects) {
//                m_sharpeRatioStatistics << "," << trdObj.getSharpeRatio(i);
//            }
//
//            m_sharpeRatioStatistics << "\n";
//        }
//
//        m_sharpeRatioStatistics.close();
//
//        std::cout << "Sharpe Ratio Report Generated!" << std::endl;
    }

    if (m_tradingReport == 1) {
        m_tradingStatistics.close();

    }

    if (m_dailyReport == 1) {
        m_dailyStatistics.close();
    }

}