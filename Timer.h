#ifndef TIMER_H
#define TIMER_H
#include<string>

class Timer{
		clock_t startTime;
		clock_t stopTime;
	public:
		Timer();
		std::string operator+(std::string s);
		void start();
		clock_t getTime();
		clock_t stop();
		operator std::string();
};
#endif
