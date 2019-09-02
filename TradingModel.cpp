/*
 * This class will be responsible for representing the Trading
 * Rule. It defines the virtual functions of the simulator
 * class such as calculateSignal which is one of the most
 * important parts of the simulator.
 */

#include "TradingModel.h"

/**/
/*

 TradingModel::TradingModel()

 NAME

    TradingModel::TradingModel - constructor for the Trading Model class

 SYNOPSIS

    TradingModel::TradingModel(int argc, char **argv);

        argc    --> count of arguments passed from the command line
        argv    --> pointer to pointer of arguments passed

 DESCRIPTION

    This is the constructor of the Trading Model class which simply
    calls the constructor of the Simulator class

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    8/16/2019

 */
/**/
TradingModel::TradingModel(int argc, char **argv) : Simulator(argc, argv) {

}


/**/
/*

 TradingModel::calculateSignal()

 NAME

    TradingModel::calculateSignal - calculates signal for the day

 SYNOPSIS
    double TradingModel::calculateSignal(TickerBlock& tickerBlock, int dateIndex);

        tickerBlock --> ticker block object for which to calculate signal
        dateIndex   --> index of the reference date vector where the simulation
                        currently is at

 DESCRIPTION

    This function defines the virtual function of the simulator which is
    responsible for calculating signal for the day of the simulation for
    a ticker

 RETURNS

    Returns signal as a double

 AUTHOR

    Bibhash Mulmi

 DATE

    8/16/2019

 */
/**/
double TradingModel::calculateSignal(TickerBlock& tickerBlock, int dateIndex) {
    std::vector<double> adjClose = tickerBlock[TickerBlock::FIELD_ADJ_CLOSE];

    if (dateIndex < 20)
        return -999;

    double fiveDayMA;
    double twentyDayMA;

    double fiveDayTotal = 0;
    double twentyDayTotal = 0;

    auto index = adjClose.begin() + dateIndex;

    int count = 0;

    for (;count < 20; ++count, --index) {
        // when there is not enough amount of data to calculate the signal
        if (index == adjClose.begin() && count < 20) {
            return -999;
        }

        // handle UNAVAILABLE data
        if (*index == -999) {
            // reset the count
            --count;
            continue;
        }

        if (count < 5) {
            fiveDayTotal += *index;
        }

        twentyDayTotal += *index;


    }

    fiveDayMA = fiveDayTotal / 5;
    twentyDayMA = twentyDayTotal / 20;

    double signal = ((fiveDayMA - twentyDayMA) / twentyDayMA) * 100;
    return signal;
}