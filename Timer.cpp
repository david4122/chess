#include "Timer.h"
#include<ctime>
#include<sstream>

void Timer::start(){
	this->startTime=time(0);
}

double Timer::stop(){
	this->stopTime=time(0);
	return difftime(stopTime, startTime);
}

Timer::operator std::string(){
	std::stringstream ss;
	ss<<(difftime(time(0), startTime));
	ss<<"s";
	std::string s;
	ss>>s;
	return s;
}
