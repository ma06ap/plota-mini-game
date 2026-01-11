#include "time.h"


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

