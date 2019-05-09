//
// Created by bibhash on 4/20/19.
//

#include "DateTime.h"

DateTime::DateTime(int a_mth, int a_day, int a_year) {
    m_year = a_year;
    m_month = a_mth;
    m_day = a_day;
}

DateTime::DateTime(std::string a_date) {
    int date = std::stoi(a_date);
    int day = date % 100;
    int mth = (date / 100) % 100;
    int yrs = date / 10000;
    m_year = yrs;
    m_month = mth;
    m_day = day;
}

bool operator==(const DateTime &left, const DateTime &right) {
    if ((left.m_day == right.m_day) && (left.m_month == right.m_month) && (left.m_year == right.m_year))
        return true;
    return false;
}

bool operator>(const DateTime &left, const DateTime &right){
    if (left.m_year > right.m_year)
        return true;
    else if (left.m_year == right.m_year && left.m_month > right.m_month)
        return true;
    else if (left.m_year == right.m_year && left.m_month == right.m_month && left.m_day > right.m_day)
        return true;
    else
        return false;
}

bool operator<(const DateTime &left, const DateTime &right){
    return right>left;
}