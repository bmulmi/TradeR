//
// Created by bibhash on 8/12/19.
//

#include <iostream>
#include "TradingObject.h"


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
TradingObject::TradingObject(std::string a_ticker) {
    m_tickerName = a_ticker;
    m_capitalInStock = 0;
    m_totalTransactions = 0;
    m_totalNumShares = 0;
    countTrade = 0;
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
void TradingObject::addCapitalInStock(double a_cap) {
    m_capitalInStock += a_cap;
    m_dailyCapitalInStock.push_back(a_cap);
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
void TradingObject::removeCapitalInStock(double a_cap) {
    m_capitalInStock -= a_cap;
    m_dailyCapitalInStock.push_back(-a_cap);
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
double TradingObject::getCapInStock() {
    return m_capitalInStock;
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
void TradingObject::addShares(double a_shares) {
    m_totalNumShares += a_shares;
    m_dailyShares.push_back(a_shares);
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
void TradingObject::removeShares(double a_shares) {
    m_totalNumShares -= a_shares;
    m_dailyShares.push_back(-a_shares);
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
void TradingObject::addTransaction(double a_count) {
    m_totalTransactions += a_count;
    m_dailyTransactions.push_back(a_count);
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
void TradingObject::addDailyReturn(double a_amount) {
    m_dailyReturns.push_back(a_amount);
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
void TradingObject::addSignal(double a_signal) {
    m_signals.push_back(a_signal);
    m_daysInPosition++;
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
void TradingObject::addPnLData(double a_amount) {
    m_dailyPnL.push_back(a_amount);

    if (m_cumulativePnL.empty())
        m_cumulativePnL.push_back(a_amount);
    else
        m_cumulativePnL.push_back(m_cumulativePnL.back() + a_amount);
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
void TradingObject::addDailyTotalMarketValue(double a_amount) {
    m_dailyTotalMarketValue.push_back(a_amount);
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
void TradingObject::addDailyNetMarketValue(double a_amount) {
    m_dailyNetMarketValue.push_back(a_amount);
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
std::string TradingObject::getTickerName() {
    return m_tickerName;
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
std::vector<double> TradingObject::getDailyCapital() {
    return m_dailyCapitalInStock;
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
std::vector<double> TradingObject::getDailyShares() {
    return m_dailyShares;
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
std::vector<double> TradingObject::getDailyReturns() {
    return m_dailyReturns;
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
std::vector<double> TradingObject::getSignals() {
    return m_signals;
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
std::vector<double> TradingObject::getDailyPnL() {
    return m_dailyPnL;
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
std::vector<double> TradingObject::getDailyTransactions() {
    return m_dailyTransactions;
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
std::vector<double> TradingObject::getDailyTotalMarketValue() {
    return m_dailyTotalMarketValue;
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
std::vector<double> TradingObject::getDailyNetMarketValue() {
    return m_dailyNetMarketValue;
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
void TradingObject::setIsInLongPosition(bool val) {
    m_isInLongPosition = val;
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
void TradingObject::setIsInShortPosition(bool val) {
    m_isInShortPosition = val;
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
bool TradingObject::isInLongPosition() {
    return m_isInLongPosition;
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
bool TradingObject::isInShortPosition() {
    return m_isInShortPosition;
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
void TradingObject::setCurrSharesHeld(double val) {
    m_currNumSharesHeld = val;
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
double TradingObject::getCurrSharesHeld() {
    return m_currNumSharesHeld;
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
void TradingObject::calculateDailySharpeRatio() {
    if (m_dailyReturns.size() != m_signals.size()){
        std::cout << m_tickerName << " mismatch!" << m_dailyReturns.size() << " != " << m_signals.size() << "\n";
        return;
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

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
double TradingObject::getSharpeRatio(int index) {
    return m_sharpeRatio.at(index);
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
void TradingObject::openTransaction(DateTime* a_date, double a_signal, double a_numShares, double a_price) {
    m_transactionOpenDate = a_date;
    m_transactionOpenSignal = a_signal;
    m_currNumSharesHeld = a_numShares;
    m_transactionOpenPrice = a_price;
    m_daysInPosition = 0;
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
void TradingObject::closeTransaction(DateTime* a_date, double a_signal, double a_price) {
    m_transactionCloseDate = a_date;
    m_transactionCloseSignal = a_signal;
    m_transactionClosePrice = a_price;
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
DateTime TradingObject::getTransactionOpenDate() {
    return *m_transactionOpenDate;
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
DateTime TradingObject::getTransactionCloseDate() {
    return *m_transactionCloseDate;
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
double TradingObject::getTransactionOpenPrice() {
    return m_transactionOpenPrice;
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
double TradingObject::getTransactionClosePrice() {
    return m_transactionClosePrice;
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
double TradingObject::getTransactionOpenSignal() {
    return m_transactionOpenSignal;
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
double TradingObject::getTransactionCloseSignal() {
    return m_transactionCloseSignal;
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
double TradingObject::getTransactionNumShares() {
    return m_currNumSharesHeld;
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
double TradingObject::getTransactionDaysInPosition(){
    return m_daysInPosition;
}