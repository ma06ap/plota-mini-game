//
// Created by matin on 12/30/25.
//

#ifndef MESSENGER_TIME_H
#define MESSENGER_TIME_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class Date {
private:
    int year;
    int month;
    int day;
    void setNow();
public:
    Date();
    Date(int y, int m, int d);
    static Date fromString(const string& str);
    string toString() const;
    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }
    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
};

class Time {
private:
    int hour;
    int minute;
    int second;
    void setNow();
public:
    Time();
    Time(int h, int m, int s);
    static Time fromString(const string& str);
    string toString() const;
};

class DateTime {
private:
    Date date;
    Time time;
public:
    DateTime();
    DateTime(const Date& d, const Time& t);
    static DateTime fromString(const string& str);
    string toString() const;
    const Date& getDate() const { return date; }
    const Time& getTime() const { return time; }
};

#endif //MESSENGER_TIME_H