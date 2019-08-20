//
// Created by bibhash on 8/12/19.
//

#ifndef TRADER_TRADINGOBJECT_H
#define TRADER_TRADINGOBJECT_H

#include "DateTime.h"
#include "Utilities.h"
#include <vector>

class TradingObject {

public:
    TradingObject();
    TradingObject(std::string a_ticker);

    void addCapitalInStock(double a_cap);
    void removeCapitalInStock(double a_cap);
    void addShares(double a_shares);
    void removeShares(double a_shares);
    void addTransaction(double a_count);
    void addDailyReturn(double a_amount);
    void addSignal(double a_signal);
    void addPnLData(double a_amount);
    void addDailyTotalMarketValue(double a_amount);
    void addDailyNetMarketValue(double a_amount);
    void setIsInLongPosition(bool val);
    void setIsInShortPosition(bool val);
    void setCurrSharesHeld(double val);

    std::string getTickerName();
    double getCapInStock();
    double getCurrSharesHeld();
    std::vector<double> getDailyCapital();
    std::vector<double> getDailyShares();
    std::vector<double> getDailyReturns();
    std::vector<double> getSignals();
    std::vector<double> getDailyPnL();
    std::vector<double> getDailyTransactions();
    std::vector<double> getDailyTotalMarketValue();
    std::vector<double> getDailyNetMarketValue();
    bool isInLongPosition();
    bool isInShortPosition();

private:
    std::string m_tickerName;
    double m_capitalInStock;
    double m_totalTransactions;
    double m_totalNumShares;
    double m_currNumSharesHeld;

    std::vector<double> m_dailyCapitalInStock;
    std::vector<double> m_dailyPnL;
    std::vector<double> m_cumulativePnL;
    std::vector<double> m_dailyReturns;
    std::vector<double> m_dailyTransactions;
    std::vector<double> m_dailyShares;
    std::vector<double> m_dailyTotalMarketValue;
    std::vector<double> m_dailyNetMarketValue;
    std::vector<double> m_signals;

    bool m_isInLongPosition;
    bool m_isInShortPosition;
};


#endif //TRADER_TRADINGOBJECT_H
