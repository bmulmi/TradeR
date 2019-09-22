/*
 * This is the header file of the Trading Object class. This class will
 * be responsible for keeping the track of trading details for each ticker
 * being traded in the simulation environment.
 */

#ifndef TRADER_TRADINGOBJECT_H
#define TRADER_TRADINGOBJECT_H

#include "DateTime.h"
#include "Utilities.h"
#include <vector>

class TradingObject {

public:
    TradingObject();
    TradingObject(std::string a_ticker, double a_initialCapital);

    // mutators
    void addCapitalInStock(double a_cap);
    void removeCapitalInStock(double a_cap);
    void addShares(double a_shares);
    void removeShares(double a_shares);
    void addTransaction(double a_count);
    void addDailyReturn(double a_amount);
    void addSignal(double a_signal);
    void addPnLData(double a_amount);
    void setIsInLongPosition(bool a_val);
    void setIsInShortPosition(bool a_val);
    void setCurrSharesHeld(double a_val);

    // accessors
    std::string getTickerName();
    double getCapInStock();
    double getCurrSharesHeld();
    std::vector<double> getDailyCapital();
    std::vector<double> getDailyShares();
    std::vector<double> getDailyReturns();
    std::vector<double> getSignals();
    std::vector<double> getDailyPnL();
    std::vector<double> getDailyTransactions();
    bool isInLongPosition();
    bool isInShortPosition();
    double getSharpeRatio(int a_index);

    // transaction statistics functions
    void openTransaction(DateTime* a_date, double a_signal, double a_numShares, double a_price);
    void closeTransaction(DateTime* a_date, double a_signal, double a_price);
    DateTime getTransactionOpenDate();
    DateTime getTransactionCloseDate();
    double getTransactionOpenSignal();
    double getTransactionCloseSignal();
    double getTransactionOpenPrice();
    double getTransactionClosePrice();
    double getTransactionDaysInPosition();

    void calculateDailySharpeRatio();

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
