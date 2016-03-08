#include "Figura.h"
#include "Board.h"
#include<string>
#include<sstream>

#ifdef LINUX_VERSION
std::string Figura::blackColorCode;
std::string Figura::whiteColorCode;
std::string Figura::defaultColorCode;
#else
int Figura::blackColorCode;
int Figura::whiteColorCode;
int Figura::defaultColorCode;
#endif

int Figura::whiteX;
int Figura::whiteY;
int Figura::blackX;
int Figura::blackY;


Figura::Figura(Color color){
    this->color=color;
    this->olaf='+';
	this->firstMove=true;
#ifndef LINUX_VERSION
	Figura::blackColorCode=12;
	Figura::whiteColorCode=10;
	Figura::defaultColorCode=11;
#else
	Figura::blackColorCode="\033[0;31m";
	Figura::whiteColorCode="\033[0;32m";
	Figura::defaultColorCode="\033[0m";
#endif
}

Figura::Figura(std::string s){
	this->olaf=s[2];
	this->color=(isupper(s[2])?WHITE:BLACK);
	this->firstMove=(s[3]=='1');
}

bool Figura::checkMove(int x, int y) const{
	return false;
}

Figura::operator std::string() const{
	std::stringstream ss;
	ss<<olaf;
	ss<<firstMove;
	std::string s;
	ss>>s;
	return s;
}

inline bool isupper(char c){
	return (c>='A'&&c<='Z');
}

Pion::Pion(Color color):Figura(color){
    this->olaf=(color==WHITE?'P':'p');
}

bool Pion::checkMove(int x,int y) const{
	if(this->firstMove){
        if(this->isWhite()){
            return (y==0&&(x==-2||x==-1));
        }
        else{
            return (y==0&&(x==2||x==1));
        }
	} else{
        if(this->isWhite()){
            if(y==0&&x==-1)
                return true;
            return false;
        } else{
            if(y==0&&x==1)
                return true;
            return false;
        }
	}
}

Krol::Krol(Color color):Figura(color){
    this->color=color;
    olaf=(color==WHITE?'K':'k');
}

bool Krol::checkMove(int x,int y) const{
	return (std::abs(x)==1||std::abs(y)==1);
}

Hetman::Hetman(Color color):Figura(color){
    this->color=color;
    olaf=(color==WHITE?'H':'h');
}

bool Hetman::checkMove(int x,int y) const{
	return ((x==0||y==0)||(std::abs(x)==std::abs(y)));
}

Goniec::Goniec(Color color):Figura(color){
    this->color=color;
    olaf=(color==WHITE?'G':'g');
}

bool Goniec::checkMove(int x,int y) const{
	return (std::abs(x)==std::abs(y));
}

Skoczek::Skoczek(Color color):Figura(color){
    this->color=color;
    olaf=(color==WHITE?'S':'s');
}

bool Skoczek::checkMove(int x,int y) const{
	return ((std::abs(x)==2&&std::abs(y)==1)||(std::abs(x)==1&&std::abs(y)==2));
}

Wieza::Wieza(Color color):Figura(color){
    this->color=color;
    olaf=(color==WHITE?'W':'w');
}

bool Wieza::checkMove(int x,int y) const{
	return (x==0||y==0);
}

Figura::~Figura(){}
