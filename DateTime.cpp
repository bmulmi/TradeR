/*
 * This class will be responsible to handle Date. Each instance
 * of this class will represent the date. It has three main member
 * variables of day, month and year. This class has its own operators
 * overloaded.
 */

#include "DateTime.h"

/**/
/*

 DateTime::DateTime()

 NAME

    DateTime::DateTime - constructor for DateTime class

 SYNOPSIS

    DateTime::DateTime(int a_mth, int a_day, int a_year);

        a_mth   --> month
        a_day   --> day
        a_year  --> year

 DESCRIPTION

    This is the constructor of the DateTime class.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/20/2019

 */
/**/
DateTime::DateTime(int a_mth, int a_day, int a_year) {
    m_year = a_year;
    m_month = a_mth;
    m_day = a_day;
    m_date = m_year*10000 + m_month*100 + m_day;
}


/**/
/*

 DateTime::DateTime()

 NAME

    DateTime::DateTime - constructor for DateTime class

 SYNOPSIS

    DateTime::DateTime(std::string a_date);

        a_date  --> string representation of date in
                    YYYYMMDD format

 DESCRIPTION

    This is the constructor for DateTime class that converts
    the string of date into int and then extracts the month,
    day and year from it.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/20/2019

 */
/**/
DateTime::DateTime(std::string const &a_date) {
    m_date = std::stoi(a_date);
    int day = m_date % 100;
    int mth = (m_date / 100) % 100;
    int yrs = m_date / 10000;
    m_year = yrs;
    m_month = mth;
    m_day = day;
}


/**/
/*

 DateTime::getDate()

 NAME

    DateTime::getDate - returns the date

 SYNOPSIS

    int DateTime::getDate()

 DESCRIPTION

    This is an accessor function that returns the
    date as integer.

 RETURNS

    Returns the m_date

 AUTHOR

    Bibhash Mulmi

 DATE

    4/20/2019

 */
/**/
int DateTime::getDate() {
    return m_date;
}


/**/
/*

 DateTime::getYear()

 NAME

    DateTime::getYear - returns the year of the date

 SYNOPSIS

    int DateTime::getYear()

 DESCRIPTION

    This accessor returns the year of the date as integer.

 RETURNS

    Returns the year.

 AUTHOR

    Bibhash Mulmi

 DATE

    4/20/2019

 */
/**/
int DateTime::getYear() {
    return m_year;
}


/**/
/*

 DateTime::getMonth()

 NAME

    DateTime::getMonth - returns month

 SYNOPSIS

    int DateTime::getMonth()

 DESCRIPTION

    This accessor returns the month of the date as integer

 RETURNS

    Returns the month

 AUTHOR

    Bibhash Mulmi

 DATE

    4/20/2019

 */
/**/
int DateTime::getMonth() {
    return m_month;
}


/**/
/*

 DateTime::getDay()

 NAME

    DateTime::getDay - returns the day of the year

 SYNOPSIS

    int DateTime::getDay()

 DESCRIPTION

    This accessor function returns the day of the year as integer

 RETURNS

    Returns the day

 AUTHOR

    Bibhash Mulmi

 DATE

    4/20/2019

 */
/**/
int DateTime::getDay() {
    return m_day;
}


/**/
/*

 operator==()

 NAME

    operator== - overloads the '==' operator

 SYNOPSIS

    bool operator==(const DateTime &a_left, const DateTime &a_right)

        a_left    --> DateTime object left of '=='
        a_right   --> DateTime object right of '=='

 DESCRIPTION

    This operator overloader compares the member variables of the
    two DateTime objects and checks whether they are equal or not.

 RETURNS

    Returns True if the day, month and year matches. Else returns
    False.

 AUTHOR

    Bibhash Mulmi

 DATE

    4/20/2019

 */
/**/
bool operator==(const DateTime &a_left, const DateTime &a_right) {
    if ((a_left.m_day == a_right.m_day) && (a_left.m_month == a_right.m_month) && (a_left.m_year == a_right.m_year))
        return true;
    return false;
}


/**/
/*

 operator>

 NAME

    operator> - overloads the '>' operator

 SYNOPSIS

    bool operator>(const DateTime &a_left, const DateTime &a_right);

        a_left  --> DateTime object left of '>'
        a_right --> DateTime object right of '>'

 DESCRIPTION

    This operator overloader compares the two DateTime object's member
    variables month, day and year. It checks to see whether the left
    object has any greater member variable. It first checks the year.
    If the year is equal then it checks the month and consequently if
    the month is equal then it checks if the day is greater.

 RETURNS

    Returns true if left date is greater than the right date. Else it
    returns false.

 AUTHOR

    Bibhash Mulmi

 DATE

    4/20/2019

 */
/**/
bool operator>(const DateTime &a_left, const DateTime &a_right){
    if (a_left.m_year > a_right.m_year)
        return true;
    else if (a_left.m_year == a_right.m_year && a_left.m_month > a_right.m_month)
        return true;
    else if (a_left.m_year == a_right.m_year && a_left.m_month == a_right.m_month && a_left.m_day > a_right.m_day)
        return true;
    else
        return false;
}


/**/
/*

 operator<

 NAME

    operator< - overloads the '<' operator

 SYNOPSIS

    bool operator< (const DateTime &a_left, const DateTime &a_right);

        a_left  --> DateTime object left of '<'
        a_right --> DateTime object right of '<'

 DESCRIPTION

    This operator overloader compares the two DateTime object by
    using the 'operator' overloaded earlier

 RETURNS

    Returns true if the left date is less than the right date.
    Else it returns false.

 AUTHOR

    Bibhash Mulmi

 DATE

    4/20/2019

 */
/**/
bool operator<(const DateTime &a_left, const DateTime &a_right){
    return a_right>a_left;
}
