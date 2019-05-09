//
// Created by bibhash on 4/15/19.
//

#include "Utilities.h"
std::string Utilities::toLowerCase(std::string &word) {
    std::string temp;

    for (char c : word){
        temp.push_back(tolower(c));
    }

    return temp;
}