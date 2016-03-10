#include "Figura.h"
#include "Board.h"
#include<string>
#include<sstream>

#ifdef LINUX_VERSION
std::string Figura::blackColorCode="\033[0;31m";
std::string Figura::whiteColorCode="\033[0;32m";
std::string Figura::defaultColorCode="\033[0m";
#endif
#ifndef LINUX_VERSION
int Figura::blackColorCode=12;
int Figura::whiteColorCode=10;
int Figura::defaultColorCode=11;
#endif

int Figura::whiteX;
int Figura::whiteY;
int Figura::blackX;
int Figura::blackY;

Figura::Figura(char s, Color c): olaf(s), color(c){
	this->firstMove=true;
}

Figura::Figura(Color color): color(color), olaf('+'){
	this->firstMove=true;
}

Figura::Figura(std::string s): olaf(s[2]), color(isupper(s[2])?WHITE:BLACK){
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

inline char symbol(char s, Color c){
	return (c==WHITE?toupper(s):tolower(s));
}

Pion::Pion(Color color):Figura(symbol('p', color), color){}

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

Krol::Krol(Color color):Figura(symbol('k', color), color){}

bool Krol::checkMove(int x,int y) const{
	return (std::abs(x)==1||std::abs(y)==1);
}

Hetman::Hetman(Color color):Figura(symbol('h', color), color){}

bool Hetman::checkMove(int x,int y) const{
	return ((x==0||y==0)||(std::abs(x)==std::abs(y)));
}

Goniec::Goniec(Color color):Figura(symbol('g', color), color){}

bool Goniec::checkMove(int x,int y) const{
	return (std::abs(x)==std::abs(y));
}

Skoczek::Skoczek(Color color):Figura(symbol('s', color), color){}

bool Skoczek::checkMove(int x,int y) const{
	return ((std::abs(x)==2&&std::abs(y)==1)||(std::abs(x)==1&&std::abs(y)==2));
}

Wieza::Wieza(Color color):Figura(symbol('w', color), color){}

bool Wieza::checkMove(int x,int y) const{
	return (x==0||y==0);
}

Figura::~Figura(){}
