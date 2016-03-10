#ifndef FIGURA_H
#define FIGURA_H

#include "Color.h"
#include "Exceptions.h"
#include<cstdlib>
#include<string>

class Figura {
	friend class Board;

	protected:
#ifdef LINUX_VERSION
		static std::string blackColorCode;
		static std::string whiteColorCode;
		static std::string defaultColorCode;
#else
        static int blackColorCode;
        static int whiteColorCode;
        static int defaultColorCode;
#endif
		const Color color;
		const char olaf;
		static int whiteX, whiteY, blackX, blackY;
		mutable bool firstMove;
		explicit Figura(char='+', Color=UNDEFINED);

	public:
		explicit Figura(Color=UNDEFINED);
		Figura(std::string);

		operator std::string() const;

        static int whiteKingX(){
            return whiteX;
        }

        static int whiteKingY(){
            return whiteY;
        }

        static int blackKingX(){
            return blackX;
        }

        static int blackKingY(){
            return blackY;
        }

		static void setWhiteKingPos(int x, int y){
            whiteX=x;
            whiteY=y;
		}

        static void setBlackKingPos(int x, int y){
            blackX=x;
            blackY=y;
        }

#ifdef LINUX_VERSION
		static void setColorCodes(std::string black, std::string white, std::string defaultColor){
			Figura::blackColorCode=black;
			Figura::whiteColorCode=white;
			Figura::defaultColorCode=defaultColor;
		}
#else
        static void setColorCodes(int black, int white, int defaultColor){
            Figura::blackColorCode=black;
            Figura::whiteColorCode=white;
            Figura::defaultColorCode=defaultColor;
        }
#endif

		bool isWhite() const {
			return color==WHITE;
		}

		Color getColor() const {
			return color;
		}

#ifdef LINUX_VERSION
		std::string getColorCode() const {
			return (this->color==WHITE?(Figura::whiteColorCode):(this->color==BLACK?(Figura::blackColorCode):(Figura::defaultColorCode)));
		}
#else
        int getColorCode() const{
            return (this->color==WHITE?(Figura::whiteColorCode):(this->color==BLACK?(Figura::blackColorCode):(Figura::defaultColorCode)));
        }
#endif

		char getChar() const {
			return olaf;
		}

		virtual bool checkMove(int, int) const;
		~Figura();
};

class Pion: public Figura{
	public:
		virtual bool checkMove(int, int) const;
		Pion(Color);
		~Pion();
};

class Krol: public Figura{
	public:
		Krol(Color);
		virtual bool checkMove(int, int) const;
		~Krol();
};

class Hetman: public Figura{
	public:
		Hetman(Color);
		virtual bool checkMove(int, int) const;
		~Hetman();
};


class Goniec: public Figura{
	public:
		Goniec(Color);
		virtual bool checkMove(int, int) const;
		~Goniec();
};

class Skoczek: public Figura{
	public:
		Skoczek(Color);
		virtual bool checkMove(int, int) const;
		~Skoczek();
};

class Wieza: public Figura{
	public:
		Wieza(Color);
		virtual bool checkMove(int, int) const;
		~Wieza();
};
#endif
