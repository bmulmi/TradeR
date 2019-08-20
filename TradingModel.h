//
// Created by bibhash on 8/16/19.
//

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
