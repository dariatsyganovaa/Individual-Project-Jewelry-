#pragma once

enum Status { SUCCESS, ERROR, WARNING };

class Time {
	int _hours;
	int _mins;
	int _seconds;

public:
	Time();
	Time(int hours, int mins, int seconds);
	Time(const Time& time);
	Time(std::string time);

	Status check();
	Status convert();

	Status add_hours(int hours);
	Status add_mins(int mins);
	Status add_seconds(int seconds);
	Time& add(const Time& time);

	Status assign(const Time& time);
	int compare(const Time& time);

	Status input();
	Status output();

};