//
// Created by bibhash on 4/20/19.
//

#ifndef TRADER_DATETIME_H
#define TRADER_DATETIME_H


#include <string>

class DateTime {
public:
    DateTime(int a_mth, int a_day, int a_year);
    DateTime(std::string date); // string should be in YYYYMMDD
    ~DateTime() = default;
    friend bool operator==(const DateTime &left, const DateTime &right);
    friend bool operator>(const DateTime &left, const DateTime &right);
    friend bool operator<(const DateTime &left, const DateTime &right);


private:
    int m_year;
    int m_month;
    int m_day;
};


#endif //TRADER_DATETIME_H