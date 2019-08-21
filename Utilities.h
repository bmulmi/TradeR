//
// Created by bibhash on 4/15/19.
//

#ifndef TRADER_UTILITIES_H
#define TRADER_UTILITIES_H


#include <string>
#include <vector>

namespace Utilities {
    std::string toLowerCase(std::string &word);
    int roundOff (double value, int roundOfTo);
    double average(std::vector<double> values);
    double standardDeviation(std::vector<double> values);
};


#endif //TRADER_UTILITIES_H
