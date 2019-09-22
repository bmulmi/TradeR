/*
 * This is the header file for TradingModel class. This class
 * will be responsible for representing the Trading Rule which
 * users want to implement.
 */

#ifndef TRADER_TRADINGMODEL_H
#define TRADER_TRADINGMODEL_H


#include "Simulator.h"

class TradingModel : protected Simulator {
public:
    TradingModel(int argc, char** argv);
    ~TradingModel()=default;
    void runSim() { Simulator::runSim(); };
    virtual double calculateSignal(TickerBlock& tickerBlock, int dateIndex) override;

};


#endif //TRADER_TRADINGMODEL_H
