/*
 * This is the header file for the Utilities namespace. This namespace
 * will be responsible for wrapping different functions used by the
 * project.
 */

#ifndef TRADER_UTILITIES_H
#define TRADER_UTILITIES_H


#include <string>
#include <vector>
#include <iostream>
#include <cmath>

namespace Utilities {
    std::string toLowerCase(std::string &a_word);
    int roundOff (double a_value, int a_roundOffTo);
    double average(std::vector<double> a_values);
    double standardDeviation(std::vector<double> a_values);
};


#endif //TRADER_UTILITIES_H
