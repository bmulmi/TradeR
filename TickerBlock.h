//
// Created by bibhash on 4/17/19.
//

#ifndef TRADER_TICKERBLOCK_H
#define TRADER_TICKERBLOCK_H

#include <string>
#include <vector>
#include <fstream>
#include "DateTime.h"

class TickerBlock {
public:
    TickerBlock();
    TickerBlock(std::string a_tickerName, std::vector<DateTime> a_refDates, std::string a_path);
    ~TickerBlock();
    void PrintParsedData();

    enum FIELD_ID{
        FIELD_OPEN,
        FIELD_HIGH,
        FIELD_LOW,
        FIELD_CLOSE,
        FIELD_VOLUME,
        FIELD_ADJ_OPEN,
        FIELD_ADJ_CLOSE,
        FIELD_DIVIDEND,
        FIELD_SPLIT,
        FIELD_VWAP,
        FIELD_SHARE_OUTSTANDING,
        LAST_FIELD = FIELD_SHARE_OUTSTANDING,
        END_ALL_FIELDS
    };

    std::vector<double> &operator [] (TickerBlock::FIELD_ID a_FieldID);

private:
    std::string m_tickerName;
    std::vector<DateTime> m_tickerDates;
    std::vector<DateTime> m_refDates;
    std::string m_tickerPath;
    std::vector<double> m_priceData[END_ALL_FIELDS];

    void ParseFile(std::string a_pathToFile);
    void HandleMissingData();

    int DATE_FIELD = 0;
    int ADJ_OPEN_INDEX = 5;
    bool FIRST_ITERATION = true;
    int EMPTY_DATA = -999;
};


#endif //TRADER_TICKERBLOCK_H
