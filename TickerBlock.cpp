/*
 * This class will be responsible for representing the price data
 * of individual tickers. It has the functions to read csv price
 * data files and storing these data to perform simulation.
 */
#include <iostream>
#include <memory.h>
#include <sstream>
#include <iomanip>
#include "TickerBlock.h"


/**/
/*

 TickerBlock::TickerBlock()

 NAME

    TickerBlock::TickerBlock - constructor for TickerBlock

 SYNOPSIS
    TickerBlock::TickerBlock(std::string &a_tickerName, std::vector<DateTime>
                            &a_refDates, const std::string a_path);

        a_tickerName    --> name of the Ticker
        a_refDates      --> vector of DateTime objects that represent active
                            trading days
        a_path          --> path to the price data of the ticker

 DESCRIPTION

    This is the constructor of the TickerBlock object.
    It is responsible for calling functions to parse the
    price data file and then handle missing dates.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/17/2019

 */
/**/
TickerBlock::TickerBlock(std::string &a_tickerName, std::vector<DateTime> &a_refDates, const std::string a_path) {
    m_tickerName = a_tickerName;
    m_refDates = a_refDates;
    m_tickerPath = a_path;
    ParseFile(a_path);
    HandleMissingData();
}


/**/
/*

 TickerBlock::operator[]()

 NAME

    TickerBlock::operator[] - overloads the '[]' operator

 SYNOPSIS

    std::vector<double>& TickerBlock::operator[](TickerBlock::FIELD_ID a_FieldID);

        a_FieldID   --> the Field_ID of the price data

 DESCRIPTION

    This operator overloader is responsible for returning the vector
    of the Field_ID passed in as an argument.

 RETURNS

 AUTHOR

 DATE

 */
/**/
std::vector<double>& TickerBlock::operator[](TickerBlock::FIELD_ID a_FieldID) {
    return m_priceData[a_FieldID];
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
void TickerBlock::ParseFile(std::string a_pathToFile) {
    std::ifstream in(a_pathToFile);

    if(!in){
        std::cerr << "ERROR! Could not open : " << a_pathToFile << std::endl;
        exit(1);
    }

    std::string line;

    // get the first line which contains file info
    getline(in, line);
//    std::cout << "first line: " << line << std::endl;

    // get the second line which has the header names
    getline(in, line);
//    std::cout << line << std::endl;

    //parse every line
    while(!in.eof()){
        line.clear();
        getline(in, line);
//        std::cout << line << std::endl;
        // boolean flag for the line is empty or has Data
        bool hasData = false;

        // use string stream to parse the line
        double data[FIELD_ID::END_ALL_FIELDS] = {0};
        int i = 0;
        std::istringstream ss(line);
        std::string each;
        while (getline(ss, each, ',')){
//            std::cout << typeid(each).name() << " ";
            data[i] = std::stod(each);
            i++;
//            std::cout << data[i];
            hasData = true;
        }
//        std::cout << data;
//        std::cout << "\n" << std::endl;
        if (!hasData) continue;

        // keep track of data array index
        int dataIndex = 0;

        // push eachData from the csv into the respective vectors of m_priceData array
        for (int i = 0; i < FIELD_ID::END_ALL_FIELDS; i++){
            if (i == DATE_FIELD)
                m_tickerDates.insert(m_tickerDates.begin(), std::to_string(data[dataIndex++]));

            if (i >= COMPUTED_FIELDS_START){
                double close = m_priceData[FIELD_ID::FIELD_CLOSE].at(0);
                double adj_close = m_priceData[FIELD_ID::FIELD_ADJ_CLOSE].at(0);
                double adj_ratio = adj_close / close;

                switch (i) {
                    case FIELD_ID::FIELD_ADJ_OPEN : {
                        double open = m_priceData[FIELD_ID::FIELD_OPEN].at(0);
                        double adj_open = open * adj_ratio;
                        m_priceData[i].insert(m_priceData[i].begin(), adj_open);
                        break;
                    }
                    case FIELD_ID::FIELD_ADJ_HIGH : {
                        double high = m_priceData[FIELD_ID::FIELD_HIGH].at(0);
                        double adj_high = high * adj_ratio;
                        m_priceData[i].insert(m_priceData[i].begin(), adj_high);
                        break;
                    }
                    case FIELD_ID::FIELD_ADJ_LOW: {
                        double low = m_priceData[FIELD_ID::FIELD_LOW].at(0);
                        double adj_low = low * adj_ratio;
                        m_priceData[i].insert(m_priceData[i].begin(), adj_low);
                        break;
                    }
                    case FIELD_ID::FIELD_ADJ_VOLUME : {
                        double volume = m_priceData[FIELD_ID::FIELD_VOLUME].at(0);
                        double adj_volume = volume * adj_ratio;
                        m_priceData[i].insert(m_priceData[i].begin(), adj_volume);
                        break;
                    }
                    default : {
                        break;
                    }
                }
            }
            else
                m_priceData[i].insert(m_priceData[i].begin(), data[dataIndex++]);


//            if (FIRST_ITERATION && i == ADJ_OPEN_INDEX){
//                // ADJ_OPEN field must be empty for the first iteration of the file
//                double adjOpen = UNAVAILABLE_DATA;
//                m_priceData[i].emplace(m_priceData[i].begin(), adjOpen);
//
//                // we are currently at the FIELD_ADJ_CLOSE index of the csv
//                // so push data[FIELD_ADJ_CLOSE] into respective array index
//                m_priceData[++i].insert(m_priceData[i].begin(), data[dataIndex++]);
//                FIRST_ITERATION = false;
//                continue;
//            }
//
//            if (i == ADJ_OPEN_INDEX){
//                double open = m_priceData[FIELD_ID::FIELD_OPEN].back();
//                double adjClose = m_priceData[FIELD_ID::FIELD_ADJ_CLOSE].back();
//                double close = m_priceData[FIELD_ID::FIELD_CLOSE].back();
//                double adjOpen = ( open * adjClose ) / close ;
//                double adjOpen = -999;
//                m_priceData[i].insert(m_priceData[i].begin(), adjOpen);
//                m_priceData[++i].insert(m_priceData[i].begin(), data[dataIndex++]);
//                continue;
//            }

        }
    }
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
void TickerBlock::HandleMissingData() {
    // loop through the reference dates
    for (int i = 0; i < m_refDates.size(); i++){
        auto ticDate = m_tickerDates.begin() + i;
        //std::cout << m_tickerDates[i].getDate() << "---" << m_refDates[i].getDate() << std::endl;
        if (!(m_tickerDates[i] == m_refDates[i])) {
            //std::cout << "inserting at: " << i<< std::endl;
            //std::cout << m_tickerName << std::endl;
            //std::cout  << m_refDates[i].getDate() << " is absent. Inserting Unavailable data..." << std::endl;

            m_tickerDates.emplace(ticDate, m_refDates[i]);

            // when the date is not start date
            if (i != 0) {
                // fill from yesterday's data
                for (int j = 0; j < FIELD_ID::END_ALL_FIELDS; j++){
                    auto position = m_priceData[j].begin() + i;
                    double yesterdaysData = *(m_priceData[j].begin() + i - 1);
                    m_priceData[j].insert(position, yesterdaysData);
                }
            }
            else {
                for (int j = 0; j < FIELD_ID::END_ALL_FIELDS; j++){
                    auto position = m_priceData[j].begin() + i;
                    m_priceData[j].insert(position, UNAVAILABLE_DATA);
                }
            }
        }
    }
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
void TickerBlock::PrintParsedData() {
    for (int i = 0; i < 10; i++) {
        std::cout << std::setw(6) << m_priceData[FIELD_OPEN].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_HIGH].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_LOW].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_CLOSE].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_VOLUME].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_ADJ_CLOSE].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_DIVIDEND].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_SPLIT].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_VWAP].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_SHARE_OUTSTANDING].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_ADJ_OPEN].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_ADJ_HIGH].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_ADJ_LOW].at(i) << "\t";
        std::cout << std::setw(6) << m_priceData[FIELD_ADJ_VOLUME].at(i) << "\t";

        std::cout << std::endl;
    }
    std::cout << "\n" << std::endl;
}