#ifndef TIMER_H
#define TIMER_H
#include<string>

class Timer{
	double startTime;
	double stopTime;
	public:
		void start();
		double stop();
		operator std::string();
};
#endif
