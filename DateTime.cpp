//
// Created by bibhash on 4/20/19.
//

#include "DateTime.h"
#include "Database.h"

DateTime::DateTime(int a_mth, int a_day, int a_year) {
    m_year = a_year;
    m_month = a_mth;
    m_day = a_day;
    m_date = m_year*10000 + m_month*100 + m_day;
}

DateTime::DateTime(std::string a_date) {
    m_date = std::stoi(a_date);
    int day = m_date % 100;
    int mth = (m_date / 100) % 100;
    int yrs = m_date / 10000;
    m_year = yrs;
    m_month = mth;
    m_day = day;
}

int DateTime::getDate() {
    return m_date;
}

int DateTime::getYear() {
    return m_year;
}

int DateTime::getMonth() {
    return m_month;
}

int DateTime::getDay() {
    return m_day;
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
