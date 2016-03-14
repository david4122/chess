#include "Timer.h"
#include<string>
#include<sstream>
#include<ctime>

Timer::Timer(){}

void Timer::start(){
	this->startTime=clock();
}

clock_t Timer::getTime(){
	return difftime(clock(), startTime);
}

clock_t Timer::stop(){
	this->stopTime=clock();
	return difftime(stopTime, startTime);
}

Timer::operator std::string(){
	std::stringstream ss;
	ss<<difftime(stopTime, startTime);
	std::string s;
	ss>>s;
	return s;
}
