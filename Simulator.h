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
    void initializeModel();
    void trade(double& a_price, TradingObject& a_trdObject, double a_signal);
    void recordStats();

//    virtual double calculateSignal();

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

    int m_monthlyReport;
    int m_dailyReport;
    int m_transactionReport;

    // member variables initialized when trade simulation
    double m_capitalInStock;
    double m_totalCapitalInvested;
    double m_totalCapitalReturned;
    double m_perStockCapitalInvested;
    double m_perStockCapitalReturned;

    int m_currentDaysInPosition;
    int m_totalSharesHeld;
    int m_perStockSharesHeld;
    int m_positionsBoughtPerTrade;
    int m_positionsSoldPerTrade;


    std::ofstream m_out;
    std::ofstream m_dailyStatistics;
    std::ofstream m_monthlyStatistics;
    std::ofstream m_transactionStatistics;

    void initializeMemberVariables();
    void negativeSignalTrading();
    void positiveSignalTrading();
};


#endif //TRADER_SIMULATOR_H
