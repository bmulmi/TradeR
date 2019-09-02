//
// Created by bibhash on 8/16/19.
//

#include "TradingModel.h"
#include <iostream>


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

 */
/**/
TradingModel::TradingModel(int argc, char **argv) : Simulator(argc, argv) {

}


/**/
/*

 NAME

 SYNOPSIS

 DESCRIPTION

 RETURNS

 AUTHOR

 DATE

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
//    auto start = end - 20;
    int amountUn = 0;
    int count = 0;
    for (;count < 20; ++count, --index) {
        // when there is not enough amount of data to calculate the signal
        if (index == adjClose.begin() && count < 20) {
            //std::cout << "unavailable data for " << dateIndex << std::endl;
            return -999;
        }

        // handle UNAVAILABLE data
        if (*index == -999) {
            // reset the count
            --count;
            amountUn++;
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
//    std::cout << "Date: " << dateIndex << "amountun: " << amountUn << std::endl;
//    std::cout << "five day total: " << fiveDayTotal << std::endl;
//    std::cout << "twenty day total: " << twentyDayTotal << std::endl;
    //std::cout << "fiveDayMA: " << fiveDayTotal / 5 << std::endl;
    //std::cout << "twentyDayMA: " << twentyDayTotal / 20 << std::endl;
    //std::cout << "SIGNAL: " << signal << std::endl;
    return signal;
}