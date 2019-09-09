/*
 * This class is represents each of the instrument being
 * traded in the simulation environment. It keeps track of
 * all the necessary statistics and computations for
 * generating reports for the user.
 */

#include "TradingObject.h"


/**/
/*

 TradingObject::TradingObject()

 NAME

    TradingObject::TradingObject - constructor for the class

 SYNOPSIS

    TradingObject::TradingObject(std::string a_ticker);

        a_ticker    --> the ticker name

 DESCRIPTION

    Constructor for this class

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
TradingObject::TradingObject(std::string a_ticker, double a_initialCapital) {
    m_tickerName = a_ticker;
    m_capitalInStock = a_initialCapital;
    m_totalTransactions = 0;
    m_totalNumShares = 0;
}


/**/
/*

 TradingObject::addCapitalInStock()

 NAME

    TradingObject::adCapitalInStock - keeps track of available
    capital

 SYNOPSIS

    void TradingObject::addCapitalInStock(double a_cap);

        a_cap   --> capital to be added

 DESCRIPTION

    This mutator function is responsible for adding capital
    to the instrument currently being traded in the simulation
    environment.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::addCapitalInStock(double a_cap) {
    m_capitalInStock += a_cap;
    m_dailyCapitalInStock.push_back(a_cap);
}


/**/
/*

 TradingObject::removeCapitalInStock()

 NAME

    Trading Object::removeCapitalInStock - subtracts capital

 SYNOPSIS

    void TradingObject::removeCapitalInStock(doubel a_cap);

        a_cap   --> capital to be subtracted

 DESCRIPTION

    This mutator function is responsible for subtracting
    capital from the trading object

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::removeCapitalInStock(double a_cap) {
    m_capitalInStock -= a_cap;
    m_dailyCapitalInStock.push_back(-a_cap);
}


/**/
/*

 TradingObject::getCapitalInStock()

 NAME

    TradingObject::getCapInStock - returns available capital

 SYNOPSIS

    double TradingObject::getCapInStock();

 DESCRIPTION

    This accessor function is responsible for returning the
    available capital for the trading object

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
double TradingObject::getCapInStock() {
    return m_capitalInStock;
}


/**/
/*

 TradingObject::addShares()

 NAME

    TradingObject::addShares - adds number of shares

 SYNOPSIS

    void TradingObject::addShares(double a_shares);

        a_shares    --> number of shares to be added

 DESCRIPTION

    This mutator function is responsible for adding
    number of shares traded for the trading object.
    It also keeps track of daily shares traded in the
    simulation environment.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::addShares(double a_shares) {
    m_totalNumShares += a_shares;
    m_dailyShares.push_back(a_shares);
}


/**/
/*

 TradingObject::removeShares()

 NAME

    TradingObject::removeShares - subtracts number of shares

 SYNOPSIS

    void TradingObject::removeShares(double a_shares);

        a_shares    --> number of shares

 DESCRIPTION

    This mutator function is responsible for subtracting the
    number of shares traded for the trading object. It also
    keeps track of daily shares traded in the simulation
    environment.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::removeShares(double a_shares) {
    m_totalNumShares -= a_shares;
    m_dailyShares.push_back(-a_shares);
}


/**/
/*

 TradingObject::addTransaction()

 NAME

    TradingObject::addTransaction - adds transactions count

 SYNOPSIS

    void TradingObject::addTransaction(double a_count);

        a_count --> number of transactions to be added

 DESCRIPTION

    This mutator function is responsible for adding the number
    of transactions made for the trading object. It also keeps
    track of daily transactions made by storing it in a vector.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::addTransaction(double a_count) {
    m_totalTransactions += a_count;
    m_dailyTransactions.push_back(a_count);
}


/**/
/*

 TradingObject::addDailyReturn()

 NAME

    TradingObject::addDailyReturn - adds daily returns

 SYNOPSIS

    void TradingObject::addDailyReturn(double a_amount);

        a_amount    --> amount of return to be added

 DESCRIPTION

    This mutator function keeps track of the daily return
    of the stock by pushing the values in the vector.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::addDailyReturn(double a_amount) {
    m_dailyReturns.push_back(a_amount);
}


/**/
/*

 TradingObject::addSignal()

 NAME

    TradingObject::addSignal - adds signal

 SYNOPSIS

    void TradingObject::addSignal(double a_signal);

 DESCRIPTION

    This function adds signal to the trading object.
    It also keeps track of the all the signals by pushing
    them to a vector.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::addSignal(double a_signal) {
    m_signals.push_back(a_signal);
    m_daysInPosition++;
}


/**/
/*

 TradingObject::addPnLData()

 NAME

    TradingObject::addPnLData - adds amount to PnL vectors

 SYNOPSIS

    void TradingObject::addPnLData(double a_amount);

        a_amount    --> amount to be added to PnL vectors

 DESCRIPTION

    This mutator function adds the amount to the daily PnL
    vector and then calculates the cumulative PnL to add it
    to the cumulative PnL vector.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::addPnLData(double a_amount) {
    m_dailyPnL.push_back(a_amount);

    if (m_cumulativePnL.empty())
        m_cumulativePnL.push_back(a_amount);
    else
        m_cumulativePnL.push_back(m_cumulativePnL.back() + a_amount);
}


/**/
/*

 TradingObject::getTickerName()

 NAME

    TradingObject::getTickerName - returns the ticker name of the
    trading object

 SYNOPSIS

    std::string TradingObject::getTickerName();

 DESCRIPTION

    This accessor function returns the ticker name of the trading
    object

 RETURNS

    Returns ticker name as a string

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
std::string TradingObject::getTickerName() {
    return m_tickerName;
}


/**/
/*

 TradingObject::getDailyCapital()

 NAME

    TradingObject::getDailyCapital - returns daily capital vector

 SYNOPSIS

    std::vector<double> TradingObject::getDailyCapital();

 DESCRIPTION

    This accessor function returns the daily capital available in
    stock vector

 RETURNS

    Returns vector of doubles which are daily capital

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
std::vector<double> TradingObject::getDailyCapital() {
    return m_dailyCapitalInStock;
}


/**/
/*

 TradingObject::getDailyShares()

 NAME

    TradingObject::getDailyShares - returns daily shares vector

 SYNOPSIS

    std::vector<double> TradingObject::getDailyShares()

 DESCRIPTION

    This accessor function returns the daily shares vector for the
    trading object. This vector consists of shares held in every
    active trading day.

 RETURNS

    Returns daily shares as a vector of doubles

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
std::vector<double> TradingObject::getDailyShares() {
    return m_dailyShares;
}


/**/
/*

 TradingObject::getDailyReturns()

 NAME

    TradingObject::getDailyReturns - returns vector of daily returns

 SYNOPSIS

    std::vector<double> TradingObject::getDailyReturns();

 DESCRIPTION

    This accessor function returns the daily returns vector of the
    trading object.

 RETURNS

    Returns daily returns as a vector of doubles

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
std::vector<double> TradingObject::getDailyReturns() {
    return m_dailyReturns;
}


/**/
/*

 TradingObject::getSignals()

 NAME

    TradingObject::getSignals - returns vector of signals

 SYNOPSIS

    std::vector<double> TradingObject::getSignals();

 DESCRIPTION

    This accessor function returns the daily signal of the trading
    object as a vector of doubles.

 RETURNS

    Returns signals as a vector of doubles

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
std::vector<double> TradingObject::getSignals() {
    return m_signals;
}


/**/
/*

 TradingObject::getDailyPnL()

 NAME

    TradingObject::getDailyPnL - returns daily PnL

 SYNOPSIS

    std::vector<double> TradingObject::getDailyPnL()

 DESCRIPTION

    This accessor function returns the daily PnL data as a vector
    for the trading object.

 RETURNS

    Returns the daily PnL data as vector of doubles

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
std::vector<double> TradingObject::getDailyPnL() {
    return m_dailyPnL;
}


/**/
/*

 TradingObject::getDailyTransactions()

 NAME

    TradingObject::getDailyTransactions - returns daily transaction data

 SYNOPSIS

    std::vector<double> TradingObject::getDailyTransactions();

 DESCRIPTION

    This accessor function returns ths daily transaction data of the trading
    object as a vector of doubles

 RETURNS

    Returns vector of doubles

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
std::vector<double> TradingObject::getDailyTransactions() {
    return m_dailyTransactions;
}

/**/
/*

 TradingObject::setIsInLongPosition()

 NAME

    TradingObject::setIsInLongPosition - sets the long position boolean

 SYNOPSIS

    void TradingObject::setIsInLongPosition(bool a_val);

        a_val   --> value to be set

 DESCRIPTION

    This mutator function sets the long position boolean value for the
    trading object. The long position boolean value determines whether
    the trading object is in long position or not.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::setIsInLongPosition(bool a_val) {
    m_isInLongPosition = a_val;
}


/**/
/*

 TradingObject::setIsInShortPosition()

 NAME

    TradingObject::setIsInShortPosition - sets the short position boolean

 SYNOPSIS
    void TradingObject::setIsInShortPosition(bool a_val);

        a_val   --> value to be set for short position boolean

 DESCRIPTION

    This mutator function sets the short position boolean. This boolean
    determines whether the trading object is in short position or not.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::setIsInShortPosition(bool a_val) {
    m_isInShortPosition = a_val;
}


/**/
/*

 TradingObject::isInLongPosition()

 NAME

    TradingObject::isInLongPosition - returns boolean for long position

 SYNOPSIS

    bool TradingObject::isInLongPosition()

 DESCRIPTION

    This function is responsible for returning the boolean value for
    the trading position type.

 RETURNS

    Returns true if the object is in long position. Else false.

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
bool TradingObject::isInLongPosition() {
    return m_isInLongPosition;
}


/**/
/*

 TradingObject::isInShortPosition()

 NAME

    TradingObject::isInShortPosition - returns boolean for short position

 SYNOPSIS

    bool TradingObject::isInShortPosition()

 DESCRIPTION

    This function is responsible for returning the boolean value for
    the trading position type.

 RETURNS

    Returns true if the object is in short position. Else false.

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
bool TradingObject::isInShortPosition() {
    return m_isInShortPosition;
}


/**/
/*

 TradingObject::setCurrSharesHeld()

 NAME

    TradingObject::setCurrSharesHeld - sets the shares being held

 SYNOPSIS

    void TradingObject::setCurrSharesHeld(double a_val);

        a_val   --> number of shares held

 DESCRIPTION

    This mutator function is responsible for setting the number of
    current shares being held by the trading object

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::setCurrSharesHeld(double a_val) {
    m_currNumSharesHeld = a_val;
}


/**/
/*

 TradingObject::getCurrSharesHeld()

 NAME

    TradingObject::getCurrSharesHeld - returns the number of shares held

 SYNOPSIS

    double TradingObject::getCurrSharesHeld();

 DESCRIPTION

    This accessor function returns the current number of shares held by
    the trading object.

 RETURNS

    Returns the number of current shares held.

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
double TradingObject::getCurrSharesHeld() {
    return m_currNumSharesHeld;
}


/**/
/*

 TradingObject::calculateDailySharpeRatio()

 NAME

    TradingObject::calculateDailySharpeRatio - calculates the sharpe ratio

 SYNOPSIS

    void TradingObject::calculateDailySharpeRatio()

 DESCRIPTION

    This function will be responsible for calculating the sharpe ratio for
    each day traded in the simulation environment. It refers to the size of
    daily return vector and calculates the sharpe ratio referring to that
    vector

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
void TradingObject::calculateDailySharpeRatio() {
    //std::cout << "Daily Return size: " << m_dailyReturns.size() << std::endl;
    while (m_dailyReturns.size() != m_signals.size()){
        m_dailyReturns.insert(m_dailyReturns.end(), 0);
    }

    for (int i = 0; i < m_dailyReturns.size(); ++i){
        std::vector<double> dailyReturns(m_dailyReturns.begin(), m_dailyReturns.begin() + i);
        double averageOfReturns = Utilities::average(dailyReturns);
        double standarDeviation = Utilities::standardDeviation(dailyReturns);
        m_sharpeRatio.push_back( averageOfReturns / standarDeviation );
    }
}


/**/
/*

 TradingObject::getSharpeRatio()

 NAME

    TradingObject::getSharpeRatio - returns the sharpe ratio

 SYNOPSIS

    double TradingObject::getSharpeRatio(int a_index);

        a_index --> date index of the sharpe ratio

 DESCRIPTION

    This accessor function is responsible for returning the
    sharpe ratio for the date index.

 RETURNS

    Returns the sharpe ratio as a double

 AUTHOR

    Bibhash Mulmi

 DATE

    8/12/2019

 */
/**/
double TradingObject::getSharpeRatio(int a_index) {
    return m_sharpeRatio.at(a_index);
}


/**/
/*

 TradingObject::openTransaction()

 NAME

    TradingObject::openTrasnaction - opens transaction

 SYNOPSIS

    void TradingObject::openTransaction(DateTime* a_date,
                                        double a_signal, double a_numShares, double a_price)

        a_date      --> opening date object
        a_signal    --> opening day signal
        a_numShares --> number of shares traded
        a_price     --> price of the stock

 DESCRIPTION

    This function will be responsible for opening the transaction. It assigns
    the argument values to the respective transaction member variables.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/18/2019

 */
/**/
void TradingObject::openTransaction(DateTime* a_date, double a_signal, double a_numShares, double a_price) {
    m_transactionOpenDate = a_date;
    m_transactionOpenSignal = a_signal;
    m_currNumSharesHeld = a_numShares;
    m_transactionOpenPrice = a_price;
    m_daysInPosition = 0;
}


/**/
/*

 TradingObject::closeTransaction()

 NAME

    TradingObject::closeTransaction - closes the transaction

 SYNOPSIS

    void TradingObject::closeTransaction(DateTime* a_date, double a_signal, double a_price)

        a_date      --> closing Date object
        a_signal    --> closing day signal
        a_price     --> closing day stock price

 DESCRIPTION

    This function will be responsible for closing the transaction
    for the trading object. It assigns the necessary values to the
    transaction member variables.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/18/2019

 */
/**/
void TradingObject::closeTransaction(DateTime* a_date, double a_signal, double a_price) {
    m_transactionCloseDate = a_date;
    m_transactionCloseSignal = a_signal;
    m_transactionClosePrice = a_price;
}


/**/
/*

 TradingObject::getTransactionOpenDate() {

 NAME

    TradingObject::getTransactionOpenDate - returns transaction open date

 SYNOPSIS

    DateTime TradingObject::getTransactionOpenDate();

 DESCRIPTION

    This accessor function returns the transaction open date

 RETURNS

    Returns the pointer to the transaction open date object

 AUTHOR

    Bibhash Mulmi

 DATE

    8/18/2019

 */
/**/
DateTime TradingObject::getTransactionOpenDate() {
    return *m_transactionOpenDate;
}


/**/
/*

 TradingObject::getTransactionCloseDate()

 NAME

    TradingObject::getTransactionCloseDate - returns closing date

 SYNOPSIS

    DateTime TradingObject::getTransactionCloseDate();

 DESCRIPTION

    This accessor function is responsible for
 RETURNS

 AUTHOR

   Bibhash Mulmi

 DATE

    8/18/2019

 */
/**/
DateTime TradingObject::getTransactionCloseDate() {
    return *m_transactionCloseDate;
}


/**/
/*

 TradingObject::getTransactionOpenPrice()

 NAME

    TradingObject::getTransactionOpenPrice - returns open stock price

 SYNOPSIS

    double TradingObject::getTransactionOpenPrice();

 DESCRIPTION

    This accessor function returns the stock price at opening of the
    transaction

 RETURNS

    Returns price of stock at opening of the transaction

 AUTHOR

    Bibhash Mulmi

 DATE

    8/18/2019

 */
/**/
double TradingObject::getTransactionOpenPrice() {
    return m_transactionOpenPrice;
}


/**/
/*

 TradingObject::getTransactionClosePrice()

 NAME

    TradingObject::getTransactionClosePrice - returns close stock price

 SYNOPSIS

    double TradingObject::getTransactionClosePrice();

 DESCRIPTION

    This accessor function returns the price of the stock at closing
    of the transaction

 RETURNS

    Returns the transaction close price

 AUTHOR

    Bibhash Mulmi

 DATE

    8/18/2019

 */
/**/
double TradingObject::getTransactionClosePrice() {
    return m_transactionClosePrice;
}


/**/
/*

 TradingObject::getTransactionOpenSignal()

 NAME

    TradingObject::getTransactionOpenSignal -  returns signal at open

 SYNOPSIS

    double TradingObject::getTransactionOpenSignal();

 DESCRIPTION

    This accessor function returns the signal at opening of the
    transaction

 RETURNS

    Returns the signal at open of transaction

 AUTHOR

    Bibhash Mulmi

 DATE

    8/18/2019

 */
/**/
double TradingObject::getTransactionOpenSignal() {
    return m_transactionOpenSignal;
}


/**/
/*

 TradingObject::getTransactionCloseSignal() {

 NAME

    TradingObject::getTransactionCloseSignal - return closing signal

 SYNOPSIS

    double TradingObject::getTransactionCloseSignal();

 DESCRIPTION

    This accessor function returns the signal at closing of the
    transaction

 RETURNS

    Returns the transaction signal at close

 AUTHOR

    Bibhash Mulmi

 DATE

    8/18/2019

 */
/**/
double TradingObject::getTransactionCloseSignal() {
    return m_transactionCloseSignal;
}


/**/
/*

 TradingObject::getTransactionDaysInPosition()

 NAME

    TradingObject::getTransactionDaysInPosition - returns the days in
    position

 SYNOPSIS

    double TradingObject::getTransactionDaysInPosition();

 DESCRIPTION

    This accessor function returns the days stayed in holding a position
    for the trading object

 RETURNS

    Returns number of days in position as a double

 AUTHOR

    Bibhash Mulmi

 DATE

    8/18/2019

 */
/**/
double TradingObject::getTransactionDaysInPosition(){
    return m_daysInPosition;
}