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

void Simulator::initializeTradingObjects() {
    std::cout << "Creating Trading Objects..." << std::endl;

    std::vector<std::string> tickers_in_universe = m_db->getTickers();
//    std::cout << tickers_in_universe.size() << std::endl;
    for (auto const& ticker : tickers_in_universe) {
        TradingObject tradingObject(ticker);
        tradingObject.setIsInLongPosition(false);
        tradingObject.setIsInShortPosition(false);
        tradingObject.setCurrSharesHeld(0);
        m_tradingObjects.push_back(tradingObject);
        std::cout << "Trd Obj Ticker Name: " << tradingObject.getTickerName() << std::endl;
    }
}

void Simulator::runSim() {
    std::cout << "Running Simulation..." << std::endl;

    std::vector<std::string> tickers_in_universe = m_db->getTickers();
    std::vector<DateTime> trading_dates = m_db->getTradingDates();

    // initialize the overall member variables;
    m_capitalInStock = 0;
    m_totalCapitalInvested = 0;
    m_totalCapitalReturned = 0;
    m_totalCumulativePnl = 0;

    for (int index = 0; index < trading_dates.size(); index++) {
        m_totalDailyPnL = 0;
        m_totalLongPositions = 0;
        m_totalShortPositions = 0;
        m_totalPositions = 0;
        m_netMarketValue = 0;
        m_totalMarketValue = 0;

        for (auto &trdObj : m_tradingObjects) {

            std::string tickerName = trdObj.getTickerName();
            TickerBlock& tickerBlock = (*m_db)[tickerName];

            double signal = calculateSignal(tickerBlock, index);
            std::cout << "\nsignal: "  << signal << std::endl;

            trdObj.addSignal(signal);

            // handle missing data
            if (signal == -999){
                trdObj.addShares(0);
                trdObj.removeCapitalInStock(0);
                trdObj.addTransaction(0);
                trdObj.addDailyReturn(0);
                continue;
            }

            double stockPrice = tickerBlock[TickerBlock::FIELD_CLOSE].at(index);

            double yesterdayPrice;
            if (index == 0 || stockPrice == -999)
                yesterdayPrice = 0;
            else
                yesterdayPrice = tickerBlock[TickerBlock::FIELD_CLOSE].at(index-1);

            trade(stockPrice, trdObj, signal);

            double positions_held = trdObj.getCurrSharesHeld();
            // calculations after trade
            if (trdObj.isInShortPosition()){
                m_totalShortPositions -= positions_held;
                m_totalPositions += -positions_held;
            }
            else {
                m_totalLongPositions += positions_held;
                m_totalPositions += positions_held;
            }

            m_totalDailyPnL += (stockPrice - yesterdayPrice) * positions_held;
            m_totalCumulativePnl += m_totalDailyPnL;
            m_totalMarketValue += m_totalPositions * stockPrice;
            m_netMarketValue += positions_held * stockPrice;
        }

        recordStats()
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
        if (a_signal <= m_exitSignal) {
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
        if (a_signal >= -m_exitSignal) {
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
            -(a_trdObject.getCurrSharesHeld()) : Utilities::roundOff (m_maxCapitalPerStock / a_price, 100);
    a_trdObject.addShares(numberOfPositionsToBuy);
    a_trdObject.removeCapitalInStock(numberOfPositionsToBuy * a_price);
    a_trdObject.addTransaction(numberOfPositionsToBuy);
    a_trdObject.setCurrSharesHeld(numberOfPositionsToBuy);
    std::cout<< "bought: " << numberOfPositionsToBuy << std::endl;
}

void Simulator::sell(double &a_price, TradingObject &a_trdObject) {
    double numSharesHeld = a_trdObject.isInShortPosition() ?
            -(Utilities::roundOff(m_maxCapitalPerStock / a_price, 100)) : a_trdObject.getCurrSharesHeld();
    a_trdObject.removeShares(numSharesHeld);
    a_trdObject.addCapitalInStock(numSharesHeld * a_price);
    a_trdObject.addTransaction(numSharesHeld);
    a_trdObject.addDailyReturn(numSharesHeld * a_price);
    a_trdObject.setCurrSharesHeld(numSharesHeld);
    std::cout<< "sold: " << numSharesHeld << std::endl;
}