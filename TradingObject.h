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
    void addShares(int a_shares);
    void removeShares(int a_shares);
    void addTransaction(int a_count);
    void addDailyReturn(double a_amount);
    void addSignal(double a_signal);
    void addPnLData(double a_amount);
    void addDailyTotalMarketValue(double a_amount);
    void addDailyNetMarketValue(double a_amount);


    std::string getTickerName();
    std::vector<double> getDailyCapital();
    std::vector<int> getDailyShares();
    std::vector<double> getDailyReturns();
    std::vector<double> getSignals();
    std::vector<double> getDailyPnL();
    std::vector<int> getDailyTransactions();
    std::vector<double> getDailyTotalMarketValue();
    std::vector<double> getDailyNetMarketValue();


private:
    std::string m_tickerName;
    double m_capitalInStock;
    int m_totalTransactions;
    int m_numShares;

    std::vector<double> m_dailyCapitalInStock;
    std::vector<double> m_dailyPnL;
    std::vector<double> m_cumulativePnL;
    std::vector<double> m_dailyReturns;
    std::vector<int> m_dailyTransactions;
    std::vector<int> m_dailyShares;
    std::vector<double> m_dailylTotalMarketValue;
    std::vector<double> m_dailyNetMarketValue;
    std::vector<double> m_signals;
};


#endif //TRADER_TRADINGOBJECT_H
