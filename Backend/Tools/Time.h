#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

// using namespace std;

class Time {
private:
    int hour;
    int minute;
    int second;
    void setNow();
public:
    Time();
    Time(int h, int m, int s);
    
};
