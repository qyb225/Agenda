#include "Date.hpp"
#include <cstdlib>

bool isLeapYear(const int year);
int monthDays(const int month, const int year);
std::string intToStr(int num);

Date::Date() {
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
    m_year = t_year;
    m_month = t_month;
    m_day = t_day;
    m_hour = t_hour;
    m_minute = t_minute;
}

Date::Date(const std::string& dateString) {
    if (dateString.size() != 16 || (dateString[4] != '-' || 
        dateString[7] != '-' || dateString[10] != '/' || dateString[13] != ':')) {
        m_year = 0;
        m_month = 0;
        m_day = 0;
        m_hour = 0;
        m_minute = 0;
    }
    else {
        m_year = atoi(dateString.substr(0, 4).c_str());
        m_month = atoi(dateString.substr(5, 2).c_str());
        m_day = atoi(dateString.substr(8, 2).c_str());
        m_hour = atoi(dateString.substr(11, 2).c_str());
        m_minute = atoi(dateString.substr(14, 2).c_str());
    }
}

int Date::getYear() const {
    return m_year;
}

void Date::setYear(const int t_year) {
    m_year = t_year;
}

int Date::getMonth() const {
    return m_month;
}

void Date::setMonth(const int t_month) {
    m_month = t_month;
}

int Date::getDay() const {
    return m_day;
}

void Date::setDay(const int t_day) {
    m_day = t_day;
}

int Date::getHour() const {
    return m_hour;
}

void Date::setHour(const int t_hour) {
    m_hour = t_hour;
}

int Date::getMinute() const {
    return m_minute;
}

void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
}

bool Date::isValid(const Date& t_date) {
    if (t_date.m_hour > 23 || t_date.m_hour < 0 || t_date.m_minute < 0 || t_date.m_minute > 59) {
        return false;
    }
    if (t_date.m_year < 1000 || t_date.m_year > 9999) {
        return false;
    }
    if (t_date.m_month < 1 || t_date.m_month > 12) {
        return false;
    }
    if (t_date.m_day < 1 || t_date.m_day > monthDays(t_date.m_month, t_date.m_year)) {
        return false;
    }
    return true;
}

Date Date::stringToDate(const std::string& t_dateString) {
    Date temp = Date(t_dateString);
    if (isValid(temp)) {
        return temp;
    }
    return Date();
}

std::string Date::dateToString(const Date& t_date) {
    if (!isValid(t_date)) {
        return "0000-00-00/00:00";
    }
    std::string dateStr = "";
    dateStr = intToStr(t_date.m_year) + "-" + intToStr(t_date.m_month) + "-"
        + intToStr(t_date.m_day) + "/" + intToStr(t_date.m_hour) + ":" + intToStr(t_date.m_minute);
    return dateStr;
}

Date& Date::operator=(const Date& t_date) {
    m_year = t_date.m_year;
    m_month = t_date.m_month;
    m_day = t_date.m_day;
    m_hour = t_date.m_hour;
    m_minute = t_date.m_minute;
    return *this;
}

bool Date::operator==(const Date& t_date) const {
    return m_year == t_date.m_year && m_month == t_date.m_month
      && m_day == t_date.m_day && m_hour == t_date.m_hour && m_minute == t_date.m_minute;
}

bool Date::operator>(const Date& t_date) const {
    if (m_year > t_date.m_year) {
        return true;
    }
    else if (m_year < t_date.m_year) {
        return false;
    }
    else {
        if (m_month > t_date.m_month) {
            return true;
        }
        else if (m_month < t_date.m_month) {
            return false;
        }
        else {
            if (m_day > t_date.m_day) {
                return true;
            }
            else if (m_day < t_date.m_day) {
                return false;
            }
            else {
                if (m_hour > t_date.m_hour) {
                    return true;
                }
                else if (m_hour < t_date.m_hour) {
                    return false;
                }
                else {
                    if (m_minute > t_date.m_minute) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }
            }
        }
    }
}

bool Date::operator<(const Date& t_date) const {
    return !(*this > t_date) && !(*this == t_date);
}

bool Date::operator>=(const Date& t_date) const {
    return *this > t_date || *this == t_date;
}

bool Date::operator<=(const Date& t_date) const {
    return !(*this > t_date);
}

bool isLeapYear(const int year) {
    bool isLeap = false;
    if (year % 100 == 0) {
        if (year % 400 == 0) {
            isLeap = true;
        }
        else {
            isLeap = false;
        }
    }
    else {
        if (year % 4 == 0) {
            isLeap = true;
        } 
        else {
            isLeap = false;
        }
    }
    return isLeap;
}

int monthDays(const int month, const int year) {
    int days = -1;
    bool isLeap = isLeapYear(year);
    if (month == 1 || month == 3 || month == 5 || 
        month == 7 || month == 8 || month == 10 || month == 12) {
        days = 31;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        days = 30;
    }
    else if (month == 2) {
        if (isLeap) {
            days = 29;
        }
        else {
            days = 28;
        }
    }
    return days;
}

std::string intToStr(int num) {
    if (num < 0) {
        return "";
    }
    std::string strNum = "";
    int level = 0;
    int basic = 1;
    while (num / basic > 9) {
        basic *= 10;
        ++level;
    }
    while (level > -1) {
        int single = num / basic;
        num -= single * basic;
        basic /= 10;
        strNum += single + '0';
        --level;
    }
    if (strNum.size() == 1) {
        strNum = "0" + strNum;
    }
    return strNum;
}
