//
// Created by bibhash on 8/8/19.
//

#include "Simulator.h"
#include <iostream>

Simulator::Simulator(int argc, char** argv) : m_argc(argc), m_configFileName(argv){
    m_totalCapitalInvested = 0;
    m_totalCapitalReturned = 0;
    initializeModel();
    initializeTradingObjects();
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

void Simulator::initializeTradingObjects() {
    std::cout << "Creating Trading Objects..." << std::endl;

    std::vector<std::string> tickers_in_universe = m_db->getTickers();
    std::cout << tickers_in_universe.size() << std::endl;
    for (auto const& ticker : tickers_in_universe) {
        TradingObject tradingObject(ticker);
        tradingObject.setIsInLongPosition(false);
        tradingObject.setIsInShortPosition(false);
        m_tradingObjects.push_back(tradingObject);
        std::cout << "Trd Onj Ticker Name: " << tradingObject.getTickerName() << std::endl;
    }
}

void Simulator::runSim() {
    std::cout << "Running Simulation..." << std::endl;

    std::vector<std::string> tickers_in_universe = m_db->getTickers();
    std::vector<DateTime> trading_dates = m_db->getTradingDates();

    // initialize the overall member variables;
    m_totalCapitalInvested = 0;
    m_totalCapitalReturned = 0;
    m_totalSharesHeld = 0;

    for (int index = 0; index < trading_dates.size(); index++) {

        for (auto &trdObj : m_tradingObjects) {

            std::string tickerName = trdObj.getTickerName();
            TickerBlock& tickerBlock = (*m_db)[tickerName];

            double signal = calculateSignal(tickerBlock, index);
            std::cout << "\nsignal: "  << signal << std::endl;
            // handle missing data's signal
            if (signal == -999)
                continue;

            trdObj.addSignal(signal);

            double stockPrice = tickerBlock[TickerBlock::FIELD_CLOSE].at(index);

            trade(stockPrice, trdObj, signal);
        }

    }

    std::cout << "Simulation Ended!" << std::endl;
}

void Simulator::trade(double &a_price, TradingObject &a_trdObject, double a_signal) {
    // first close any position accordingly
    closePosition(a_price, a_trdObject, a_signal);

    // if position is closed then neither positions will be true, so we will open new positions accordingly
    if (!a_trdObject.isInShortPosition() && !a_trdObject.isInLongPosition()) {
        openPosition(a_price, a_trdObject, a_signal);
    }
}

void Simulator::closePosition(double &a_price, TradingObject &a_trdObject, double a_signal) {
    if (!a_trdObject.isInLongPosition() && !a_trdObject.isInShortPosition())
        return;

    if (a_trdObject.isInLongPosition()){
        if (a_signal >= m_exitSignal) {
            std::cout << "selling in long: ";
            sell(a_price, a_trdObject);
            //set isInLongPosition to false
            a_trdObject.setIsInLongPosition(false);
            return;
        }
        else {
            //do nothing
            a_trdObject.addShares(0);
            a_trdObject.removeCapitalInStock(0);
            a_trdObject.addTransaction(0);
            a_trdObject.addDailyReturn(0);
            //set isInLongPosition to true and isInShortPosition to false
            a_trdObject.setIsInLongPosition(true);
            a_trdObject.setIsInShortPosition(false);
            std::cout << "did nothing" << std::endl;
            return;
        }
    }

    if (a_trdObject.isInShortPosition()){
        if (a_signal <= -m_exitSignal) {
            std::cout << "buy back in short: ";
            buy(a_price, a_trdObject);
            //set isInShortPosition to false
            a_trdObject.setIsInShortPosition(false);

        }
        else {
            //do nothing
            a_trdObject.removeShares(0);
            a_trdObject.addCapitalInStock(0);
            a_trdObject.addTransaction(0);
            a_trdObject.addDailyReturn(0);
            //set isInShortPosition to true and isInLongPosition to false
            std::cout << "did nothing" << std::endl;

            a_trdObject.setIsInShortPosition(true);
            a_trdObject.setIsInLongPosition(false);
            return;
        }
    }
}

void Simulator::openPosition(double &a_price, TradingObject &a_trdObject, double a_signal) {
    if (a_signal >= m_entrySignal) {
        a_trdObject.setIsInLongPosition(true);
        a_trdObject.setIsInShortPosition(false);
        std::cout << "buying in long: ";
        buy(a_price, a_trdObject);
        return;
    }
    if (a_signal <= -m_entrySignal) {
        a_trdObject.setIsInShortPosition(true);
        a_trdObject.setIsInLongPosition(false);
        std::cout << "selling in short: ";
        sell(a_price, a_trdObject);
        return;
    }
}

void Simulator::buy(double &a_price, TradingObject &a_trdObject) {
    double numberOfPositionsToBuy = a_trdObject.isInShortPosition() ?
            a_trdObject.getCurrSharesHeld() : m_maxCapitalPerStock / a_price;
    a_trdObject.addShares(numberOfPositionsToBuy);
    a_trdObject.removeCapitalInStock(numberOfPositionsToBuy * a_price);
    a_trdObject.addTransaction(numberOfPositionsToBuy);
    a_trdObject.setCurrSharesHeld(numberOfPositionsToBuy);
    std::cout<< "bought: " << numberOfPositionsToBuy << std::endl;
}

void Simulator::sell(double &a_price, TradingObject &a_trdObject) {
    double numSharesHeld = a_trdObject.isInShortPosition() ?
            m_maxCapitalPerStock / a_price : a_trdObject.getCurrSharesHeld();
    a_trdObject.removeShares(numSharesHeld);
    a_trdObject.addCapitalInStock(numSharesHeld * a_price);
    a_trdObject.addTransaction(numSharesHeld);
    a_trdObject.addDailyReturn(numSharesHeld * a_price);
    a_trdObject.setCurrSharesHeld(numSharesHeld);
    std::cout<< "sold: " << numSharesHeld << std::endl;
}