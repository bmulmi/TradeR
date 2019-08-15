//
// Created by bibhash on 8/12/19.
//

#include "TradingObject.h"

TradingObject::TradingObject(std::string a_ticker) {
    m_tickerName = a_ticker;
    m_capitalInStock = 0;
    m_totalTransactions = 0;
    m_numShares = 0;
}

void TradingObject::addCapitalInStock(double a_cap) {
    m_capitalInStock += a_cap;
    m_dailyCapitalInStock.push_back(a_cap);
}

void TradingObject::removeCapitalInStock(double a_cap) {
    m_capitalInStock -= a_cap;
    m_dailyCapitalInStock.push_back(-a_cap);
}

void TradingObject::addShares(int a_shares) {
    m_numShares += a_shares;
    m_dailyShares.push_back(a_shares);
}

void TradingObject::removeShares(int a_shares) {
    m_numShares -= a_shares;
    m_dailyShares.push_back(-a_shares);
}

void TradingObject::addTransaction(int a_count) {
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
    m_dailylTotalMarketValue.push_back(a_amount);
}

void TradingObject::addDailyNetMarketValue(double a_amount) {
    m_dailyNetMarketValue.push_back(a_amount);
}