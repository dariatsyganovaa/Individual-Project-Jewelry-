#include <iostream>
#include <stdexcept>
#include "../Time/ctime.h"

Time::Time() {
    _hours = 0;
    _mins = 0;
    _seconds = 0;
}

Time::Time(int hours, int mins, int seconds) {
    if (hours < 0 || hours >= 24) {
        throw std::invalid_argument("Error! The clock value is incorrect. It should be in the range 0-23.");
    }
    if (mins < 0 || mins >= 60) {
        throw std::invalid_argument("Error! The minutes value is incorrect. It should be in the range 0-59.");
    }
    if (seconds < 0 || seconds >= 60) {
        throw std::invalid_argument("Error! The seconds value is incorrect. It should be in the range 0-59.");
    }

    _hours = hours;
    _mins = mins;
    _seconds = seconds;
}

Time::Time(const Time& time) {
    /*if (&time == NULL) {
        throw std::invalid_argument("Error! Incorrect time object when copying.");
    }*/
    _hours = time._hours;
    _mins = time._mins;
    _seconds = time._seconds;
}
