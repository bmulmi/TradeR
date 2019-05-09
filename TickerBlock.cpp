//
// Created by bibhash on 4/17/19.
//

#include <iostream>
#include <memory.h>
#include <sstream>
#include <iomanip>
#include "TickerBlock.h"

TickerBlock::TickerBlock(std::string a_tickerName, std::vector<DateTime> a_refDates, std::string a_path) {
    m_tickerName = a_tickerName;
    m_refDates = a_refDates;
    m_tickerPath = a_path;
    ParseFile(a_path);
    HandleMissingData();
}

std::vector<double>& TickerBlock::operator[](TickerBlock::FIELD_ID a_FieldID) {
    return m_priceData[a_FieldID];
}

void TickerBlock::ParseFile(std::string a_pathToFile) {
    std::ifstream in(a_pathToFile);

    if(!in){
        std::cerr << "ERROR! Could not open : " << a_pathToFile << std::endl;
        exit(1);
    }

    std::string line;
    getline(in, line);  // get the first line which contains file info
    //std::cout << "first line: " << line << std::endl;
    getline(in, line);  // get the second line which has the header names
    //std::cout << line << std::endl;

    //parse every line
    while(!in.eof()){
        line.clear();
        getline(in, line);

        // use string stream to parse the line
        double data[FIELD_ID::END_ALL_FIELDS];
        int i = 0;
        std::istringstream ss(line);
        std::string each;
        while (getline(ss, each, ',')){
            data[i] = std::stod(each);
            i++;
        }

        int dataIndex = 0;  // keep track of data array index
        // push eachData into the respective vectors of m_priceData array
        for (int i = 0; i < FIELD_ID::END_ALL_FIELDS; i++){
            if (i == DATE_FIELD)
                m_tickerDates.emplace_back(std::to_string(data[dataIndex++]));

            if (FIRST_ITERATION && i == ADJ_OPEN_INDEX){
                // ADJ_OPEN field must be empty for the first iteration of the file
                double adjOpen = EMPTY_DATA;
                m_priceData[i].push_back(adjOpen);

                // we are currently at the FIELD_ADJ_CLOSE index of the csv
                // so push data[FIELD_ADJ_CLOSE] into respective array index
                m_priceData[++i].push_back(data[dataIndex++]);
                FIRST_ITERATION = false;
                continue;
            }

            if (i == ADJ_OPEN_INDEX){
                double open = m_priceData[FIELD_ID::FIELD_OPEN].back();
                double adjClose = m_priceData[FIELD_ID::FIELD_ADJ_CLOSE].back();
                double close = m_priceData[FIELD_ID::FIELD_CLOSE].back();
                double adjOpen = ( open * adjClose ) / close ;
                m_priceData[i].push_back(adjOpen);
                m_priceData[++i].push_back(data[dataIndex++]);
                continue;
            }

            m_priceData[i].push_back(data[dataIndex++]);
        }
    }
}

void TickerBlock::HandleMissingData() {
    // iterate through the m_tickerDates
    // compare it with m_refDates
    // if !equal
    // insert EMPTY DATA into m_priceData at that index
}

void TickerBlock::PrintParsedData() {
    for (int i = 0; i < 10; i++) {
        std::cout << std::setw(6) << m_priceData[FIELD_OPEN].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_HIGH].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_LOW].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_CLOSE].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_VOLUME].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_ADJ_OPEN].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_ADJ_CLOSE].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_DIVIDEND].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_SPLIT].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_VWAP].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_SHARE_OUTSTANDING].at(i) << "\t";
        std::cout << std::endl;
    }
}