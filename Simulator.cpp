//
// Created by bibhash on 8/8/19.
//

#include "Simulator.h"
#include <iostream>

Simulator::Simulator(int argc, char** argv) : m_argc(argc), m_configFileName(argv){
    m_totalCapitalInvested = 0;
    m_totalCapitalReturned = 0;
    initializeModel();
}

void Simulator::initializeModel() {
    ConfigParser configs(m_configFileName[1]);
    m_configs = &configs;
    m_configs->ParseData();
//    m_configs->PrintConfigs();

    m_db = &(Database::GetInstanceOfDb(m_configs->GetConfigValues("global", "universe_file"),
            m_configs->GetConfigValues("global", "price_data")));

    m_entrySignal = std::stod(m_configs->GetConfigValues("entry_conditions", "signal_threshold"));
    m_exitSignal = std::stod(m_configs->GetConfigValues("exit_conditions", "signal_threshold"));
    m_availableCapital = std::stod(m_configs->GetConfigValues("portfolio", "capital"));
    m_maxCapitalPerStock = std::stod(m_configs->GetConfigValues("portfolio", "maximum_capital_object"));
    m_maxDaysInPosition = std::stoi(m_configs->GetConfigValues("portfolio", "max_position_days"));
    m_maxPositionPerStock = std::stoi(m_configs->GetConfigValues("portfolio", "max_positions"));

    m_monthlyReport = std::stoi(m_configs->GetConfigValues("publication", "monthly"));
    m_dailyReport = std::stoi(m_configs->GetConfigValues("publication", "daily"));
    m_transactionReport = std::stoi(m_configs->GetConfigValues("publication", "transaction"));
}

void Simulator::initializeMemberVariables() {
    m_capitalInStock = 0;
    m_currentDaysInPosition = 0;
    m_perStockCapitalInvested = 0;
    m_perStockCapitalReturned = 0;
    m_perStockSharesHeld = 0;
    m_positionsBoughtPerTrade = 0;
    m_positionsSoldPerTrade = 0;
}

void Simulator::runSim() {
    std::cout << "Running Simulation..." << std::endl;
    std::vector<std::string> tickers_in_universe = m_db->getTickers();
    std::vector<DateTime> trading_dates = m_db->getTradingDates();
    std::map<std::string, TickerBlock*> trading_db = m_db->getDatabase();

    // initialize the overall member variables;
    m_totalCapitalInvested = 0;
    m_totalCapitalReturned = 0;
    m_totalSharesHeld = 0;

    for (int index = 0; index < trading_dates.size(); index++) {
        for (auto tickerData : trading_db) {
            TickerBlock& current = *tickerData.second;
            std::cout << current[TickerBlock::FIELD_CLOSE].size() << std::endl;
        }
    }

//    for (auto ticker : tickers_in_universe){
//        TickerBlock& currentTickerBlock = (*m_db)[ticker];
//
//        TradingObject currentTradingObject (ticker);
//
//        initializeMemberVariables();
//
//        std::cout << "Simulating: " << ticker << std::endl;
//
//        for (int index = 0; index < currentTickerBlock[TickerBlock::FIELD_CLOSE].size(); index++){
//
//
//        }
//    }
    std::cout << "Simulation Ended!" << std::endl;
}