#include <iostream>
#include <stdexcept>
#include "../Time/ctime.h"

Time::Time() : _hours(0), _mins(0), _seconds(0) {}

Time::Time(int hours, int mins, int seconds) {
    if (hours < 0 || hours > 23 || mins < 0 || mins > 59 || seconds < 0 || seconds > 59) {
        throw std::invalid_argument("Time::Time: invalid time values");
    }
    _hours = hours;
    _mins = mins;
    _seconds = seconds;
}

Time::Time(const Time& other) : _hours(other._hours), _mins(other._mins), _seconds(other._seconds) {}

void Time::setHours(int hours) {
    if (hours < 0 || hours > 23) throw std::invalid_argument("Time::setHours: invalid hour");
    _hours = hours;
}

void Time::setMinutes(int mins) {
    if (mins < 0 || mins > 59) throw std::invalid_argument("Time::setMinutes: invalid minute");
    _mins = mins;
}

void Time::setSeconds(int seconds) {
    if (seconds < 0 || seconds > 59) throw std::invalid_argument("Time::setSeconds: invalid second");
    _seconds = seconds;
}
