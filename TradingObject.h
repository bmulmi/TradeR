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
    double getTodayPnL(int a_index);
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

    void openTransaction(DateTime* a_date, double a_signal, double a_numShares, double a_price);
    void closeTransaction(DateTime* a_date, double a_signal, double a_price);
    DateTime getTransactionOpenDate();
    DateTime getTransactionCloseDate();
    double getTransactionOpenSignal();
    double getTransactionCloseSignal();
    double getTransactionNumShares();
    double getTransactionOpenPrice();
    double getTransactionClosePrice();
    double getTransactionDaysInPosition();

    void calculateDailySharpeRatio();
    double getSharpeRatio(int index);

private:
    std::string m_tickerName;
    double m_capitalInStock;
    double m_totalTransactions;
    double m_totalNumShares;

    std::vector<double> m_dailyCapitalInStock;
    std::vector<double> m_dailyPnL;
    std::vector<double> m_cumulativePnL;
    std::vector<double> m_dailyReturns;
    std::vector<double> m_dailyTransactions;
    std::vector<double> m_dailyShares;
    std::vector<double> m_dailyTotalMarketValue;
    std::vector<double> m_dailyNetMarketValue;
    std::vector<double> m_signals;
    std::vector<double> m_sharpeRatio;

    // Transaction statistics
    DateTime* m_transactionOpenDate;
    DateTime* m_transactionCloseDate;
    double m_transactionOpenSignal;
    double m_transactionCloseSignal;
    double m_currNumSharesHeld;
    double m_transactionOpenPrice;
    double m_transactionClosePrice;
    double m_daysInPosition;

    // Positions Type
    bool m_isInLongPosition;
    bool m_isInShortPosition;

};


#endif //TRADER_TRADINGOBJECT_H
