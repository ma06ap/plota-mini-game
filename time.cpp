//
// Created by matin on 12/30/25.
//

#include "Time.h"
#include "Exception.h"

void Date::setNow() {
    std::time_t now = std::time(nullptr);
    std::tm local = *std::localtime(&now);
    year  = local.tm_year + 1900;
    month = local.tm_mon + 1;
    day   = local.tm_mday;
}

Date::Date() {
    setNow();
}

Date::Date(int y, int m, int d) : year(y), month(m), day(d) {
    if (m < 1 || m > 12 || d < 1 || d > 31) {
        throw DateException("Invalid date");
    }
}

Date Date::fromString(const string& str) {
    int y, m, d;
    char delim = '-';
    istringstream iss(str);
    iss >> y >> delim >> m >> delim >> d;
    return Date(y, m, d);
}

string Date::toString() const {
    ostringstream oss;
    oss << setfill('0') << setw(4) << year << "-" << setw(2) << month << "-" << setw(2) << day;
    return oss.str();
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
}

void Time::setNow() {
    std::time_t now = std::time(nullptr);
    std::tm local = *std::localtime(&now);
    hour   = local.tm_hour;
    minute = local.tm_min;
    second = local.tm_sec;
}

Time::Time() {
    setNow();
}

Time::Time(int h, int m, int s) : hour(h), minute(m), second(s) {}

Time Time::fromString(const string& str) {
    int h, m, s;
    char delim;
    istringstream iss(str);
    iss >> h >> delim >> m >> delim >> s;
    return Time(h, m, s);
}

string Time::toString() const {
    ostringstream oss;
    oss << setfill('0') << setw(2) << hour << ":" << setw(2) << minute << ":" << setw(2) << second;
    return oss.str();
}

DateTime::DateTime() : date(), time() {}

DateTime::DateTime(const Date& d, const Time& t) : date(d), time(t) {}

DateTime DateTime::fromString(const string& str) {
    string dateStr = str.substr(0, 10);
    string timeStr = str.substr(11, 8);
    return DateTime(Date::fromString(dateStr), Time::fromString(timeStr));
}

string DateTime::toString() const {
    return date.toString() + " " + time.toString();
}
