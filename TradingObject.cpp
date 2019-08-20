//
// Created by bibhash on 8/12/19.
//

#include "TradingObject.h"

TradingObject::TradingObject(std::string a_ticker) {
    m_tickerName = a_ticker;
    m_capitalInStock = 0;
    m_totalTransactions = 0;
    m_totalNumShares = 0;
}

void TradingObject::addCapitalInStock(double a_cap) {
    m_capitalInStock += a_cap;
    m_dailyCapitalInStock.push_back(a_cap);
}

void TradingObject::removeCapitalInStock(double a_cap) {
    m_capitalInStock -= a_cap;
    m_dailyCapitalInStock.push_back(-a_cap);
}

double TradingObject::getCapInStock() {
    return m_capitalInStock;
}

void TradingObject::addShares(double a_shares) {
    m_totalNumShares += a_shares;
    m_dailyShares.push_back(a_shares);
}

void TradingObject::removeShares(double a_shares) {
    m_totalNumShares -= a_shares;
    m_dailyShares.push_back(-a_shares);
}

void TradingObject::addTransaction(double a_count) {
    m_totalTransactions += a_count;
    m_dailyTransactions.push_back(a_count);
}

void TradingObject::addDailyReturn(double a_amount) {
    m_dailyReturns.push_back(a_amount);
}

void TradingObject::addSignal(double a_signal) {
    m_signals.push_back(a_signal);
}

void TradingObject::addPnLData(double a_amount) {
    m_dailyPnL.push_back(a_amount);

    if (m_cumulativePnL.empty())
        m_cumulativePnL.push_back(a_amount);
    else
        m_cumulativePnL.push_back(m_cumulativePnL.back() + a_amount);
}

void TradingObject::addDailyTotalMarketValue(double a_amount) {
    m_dailyTotalMarketValue.push_back(a_amount);
}

void TradingObject::addDailyNetMarketValue(double a_amount) {
    m_dailyNetMarketValue.push_back(a_amount);
}

std::string TradingObject::getTickerName() {
    return m_tickerName;
}

std::vector<double> TradingObject::getDailyCapital() {
    return m_dailyCapitalInStock;
}

std::vector<double> TradingObject::getDailyShares() {
    return m_dailyShares;
}

std::vector<double> TradingObject::getDailyReturns() {
    return m_dailyReturns;
}

std::vector<double> TradingObject::getSignals() {
    return m_signals;
}

std::vector<double> TradingObject::getDailyPnL() {
    return m_dailyPnL;
}

std::vector<double> TradingObject::getDailyTransactions() {
    return m_dailyTransactions;
}

std::vector<double> TradingObject::getDailyTotalMarketValue() {
    return m_dailyTotalMarketValue;
}

std::vector<double> TradingObject::getDailyNetMarketValue() {
    return m_dailyNetMarketValue;
}

void TradingObject::setIsInLongPosition(bool val) {
    m_isInLongPosition = val;
}

void TradingObject::setIsInShortPosition(bool val) {
    m_isInShortPosition = val;
}

bool TradingObject::isInLongPosition() {
    return m_isInLongPosition;
}

bool TradingObject::isInShortPosition() {
    return m_isInShortPosition;
}

void TradingObject::setCurrSharesHeld(double val) {
    m_currNumSharesHeld = val;
}

double TradingObject::getCurrSharesHeld() {
    return m_currNumSharesHeld;
}