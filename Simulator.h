//
// Created by bibhash on 8/8/19.
//

#ifndef TRADER_SIMULATOR_H
#define TRADER_SIMULATOR_H

#include <fstream>

#include "ConfigParser.h"
#include "Database.h"
#include "DateTime.h"
#include "TickerBlock.h"
#include "TradingObject.h"
#include "Utilities.h"


class Simulator {

public:
    Simulator()=default;
    Simulator(int argc, char** argv);
    void runSim();
    void trade(double& a_price, TradingObject& a_trdObject, double a_signal);

    virtual double calculateSignal(TickerBlock& tickerBlock, int index) = 0;

protected:
    int m_argc;
    char** m_configFileName;
    Database* m_db;
    ConfigParser* m_configs;
    std::vector<TradingObject> m_tradingObjects;

    // member variables from the configuration file
    double m_entrySignal;
    double m_exitSignal;
    double m_availableCapital;
    double m_maxCapitalPerStock;
    int m_maxDaysInPosition;
    int m_maxPositionPerStock;

    int m_dailyReport;
    int m_tradingReport;
    int m_sharpeRatioReport;

    // member variables initialized during trade simulation
    std::vector<double> m_dailyPnL;
    double m_totalDailyPnL;
    double m_totalCumulativePnl;
    double m_totalMarketValue;
    double m_netMarketValue;

    double m_totalPositions;
    double m_totalLongPositions;
    double m_totalShortPositions;
    DateTime* m_currDate;

    std::ofstream m_sharpeRatioYearly;
    std::ofstream m_dailyStatistics;
    std::ofstream m_tradingStatistics;
    std::ofstream m_sharpeRatioStatistics;

    void buy(double& a_price, TradingObject& a_trdObject, double a_numShares);
    void sell(double& a_price, TradingObject& a_trdObject, double a_numShares);
    void openPosition(double& a_price, TradingObject& a_trdObject, double a_signal);
    void closePosition(double& a_price, TradingObject& a_trdObject, double a_signal);
    void recordStats();
    void initializeModel();
    void initializeTradingObjects();
    void recordTransaction(TradingObject &a_obj);
    void generateReports();
    void recordSharpeRatio();
    void recordSharpeYearly();
};


#endif //TRADER_SIMULATOR_H
