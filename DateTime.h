/*
 * This is the header file of the DateTime class. This class
 * will be responsible for Date and Time manipulation. Currently
 * it will only be responsible for handling Dates and Date
 * arithmetic.
 */

#ifndef TRADER_DATETIME_H
#define TRADER_DATETIME_H


#include <string>

class DateTime {
public:
    DateTime(int a_mth, int a_day, int a_year);
    DateTime(std::string const &a_date); // string should be in YYYYMMDD
    ~DateTime() = default;
    int getDate();
    int getYear();
    int getMonth();
    int getDay();

    friend bool operator==(const DateTime &a_left, const DateTime &a_right);
    friend bool operator>(const DateTime &a_left, const DateTime &a_right);
    friend bool operator<(const DateTime &a_left, const DateTime &a_right);

private:
    int m_year;
    int m_month;
    int m_day;
    int m_date;
};


#endif //TRADER_DATETIME_H
