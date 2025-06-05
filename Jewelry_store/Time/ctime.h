#pragma once
#include <stdexcept>

class Time {
private:
    int _hours;
    int _mins;
    int _seconds;

public:
    Time();
    Time(int hours, int mins, int seconds);
    Time(const Time& other);

    int getHours() const { return _hours; }
    int getMinutes() const { return _mins; }
    int getSeconds() const { return _seconds; }

    void setHours(int hours);
    void setMinutes(int mins);
    void setSeconds(int seconds);
};