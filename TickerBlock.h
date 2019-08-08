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
    TickerBlock(std::string &a_tickerName, std::vector<DateTime> &a_refDates, const std::string a_path);
    ~TickerBlock();
    void PrintParsedData();

    enum FIELD_ID{
        FIELD_OPEN,
        DATA_FIELD_START = FIELD_OPEN,
        FIELD_HIGH,
        FIELD_LOW,
        FIELD_CLOSE,
        FIELD_VOLUME,
        FIELD_ADJ_CLOSE,
        FIELD_DIVIDEND,
        FIELD_SPLIT,
        FIELD_VWAP,
        FIELD_SHARE_OUTSTANDING,
        DATA_FIELD_END = FIELD_SHARE_OUTSTANDING,

        FIELD_ADJ_OPEN,
        COMPUTED_FIELDS_START = FIELD_ADJ_OPEN,
        FIELD_ADJ_HIGH,
        FIELD_ADJ_LOW,
        FIELD_ADJ_VOLUME,
        COMPUTED_FIELDS_END = FIELD_ADJ_VOLUME,

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
    bool FIRST_ITERATION = true;
    int UNAVAILABLE_DATA = -999;
};


#endif //TRADER_TICKERBLOCK_H
