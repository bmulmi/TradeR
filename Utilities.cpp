/*
 * This namespace wraps all the functions used by different classes
 * throughout this project.
 */

#include <cmath>
#include "Utilities.h"


/**/
/*

 Utilities::toLoweCase()

 NAME

    Utilities::toLowerCase - converts string to lower case

 SYNOPSIS

    std::string Utilities::toLowerCase(std::string &a_word);

        a_word  --> the word to be converted to lower case

 DESCRIPTION

    This function converts the string to lowercase character by
    character.

 RETURNS

    Returns the converted string

 AUTHOR

    Bibhash Mulmi

 DATE

    4/15/2019

 */
/**/
std::string Utilities::toLowerCase(std::string &a_word) {
    std::string temp;

    for (char c : a_word){
        temp.push_back(tolower(c));
    }

    return temp;
}


/**/
/*

 Utilities::roundOff()

 NAME

    Utilities::roundOff - rounds off the value

 SYNOPSIS

    int Utilities::roundOff(double a_value, int a_roundOffTo);

        a_value         --> value to be rounded off
        a_roundOffTo    --> nearest unit to round off to

 DESCRIPTION

    This function will be responsible for rounding off the value to the
    nearest unit passed in as argument.

 RETURNS

    Returns the rounded off value as an integer

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
int Utilities::roundOff(double a_value, int a_roundOffTo) {
    double valueToRoundOff = a_value;
    valueToRoundOff = valueToRoundOff / a_roundOffTo;
    valueToRoundOff = valueToRoundOff + 0.5;
    valueToRoundOff = int(valueToRoundOff) * a_roundOffTo;
    return int(valueToRoundOff);
}


/**/
/*

 Utilities::average()

 NAME

    Utilities::average - calculates average

 SYNOPSIS

    double Utilities::average(std::vector<double> a_values);

        a_values    --> vector of values to calculate average for

 DESCRIPTION

    This function will be responsible for calculating the average
    of the values passed in as a vector

 RETURNS

    Returns the average as a double

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
double Utilities::average(std::vector<double> a_values) {
    double sum = 0;
    for (double val : a_values)
        sum += val;
    return sum / a_values.size();
}


/**/
/*

 Utilities::standardDeviation()

 NAME

    Utilities::standardDeviation - calculates standard deviation

 SYNOPSIS

    double Utilities::standardDeviation(std::vector<double> a_values);

        a_values    --> vector of values to calculate standard deviation for

 DESCRIPTION

    This function will be responsible for calculating the standard deviation
    of the values passed in as a vector. It calculates the standard deviation
    in 4 steps.

    Step 1: Find the mean
    Step 2: Find distance between each value and mean, and square them
    Step 3: Sum up the distances found and divide it by the size of Vector
    Step 4: Take the square root of value obtained from step 3

 RETURNS

    Returns the standard deviation as a double

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019
 */
/**/
double Utilities::standardDeviation(std::vector<double> a_values) {
    // step 1: Find the mean
    double mean = average (a_values);

    // step 2: Find distance between each value and mean, then square them
    std::vector<double> distance;
    for (double val : a_values){
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