//
// Created by bibhash on 4/15/19.
//

#include <cmath>
#include "Utilities.h"
std::string Utilities::toLowerCase(std::string &word) {
    std::string temp;

    for (char c : word){
        temp.push_back(tolower(c));
    }

    return temp;
}

int Utilities::roundOff(double value, int roundOfTo) {
    double valueToRoundOff = value;
    valueToRoundOff = valueToRoundOff / roundOfTo;
    valueToRoundOff = valueToRoundOff + 0.5;
    valueToRoundOff = int(valueToRoundOff) * roundOfTo;
    return int(valueToRoundOff);
}

double Utilities::average(std::vector<double> values) {
    double sum = 0;
    for (double val : values)
        sum += val;
    return sum / values.size();
}

double Utilities::standardDeviation(std::vector<double> values) {
    // step 1: Find the mean
    double mean = average (values);

    // step 2: Find distance between each value and mean, then square them
    std::vector<double> distance;
    for (double val : values){
        double temp = std::pow(val - mean, 2);
        distance.push_back(temp);
    }

    // step 3: Sum up the distances found and divide it by N
    // simply means finding the average of the new vector
    double avg = average(distance);

    // step 4: Find the SD by taking the square root
    double SD = std::sqrt(avg);

    return SD;
}