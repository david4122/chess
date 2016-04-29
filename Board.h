#ifndef BOARD_H
#define BOARD_H
#include "Exceptions.h"
#include "Color.h"
#include "Figura.h"
#include<string>

class Board{
	struct Save{
		friend class Board;
   		private:
			Save *prev;
			int oldX, oldY, newX, newY;
			bool oldfm, newfm;
			bool castling;
			const Figura *oldF, *newF;
			Color capturedColor;
		public:
			Save(int,int,int,int, const Figura*, const Figura*, bool, Save*, Color);
	};

	const Figura *board[8][8];
	const Figura *whiteCaptured[15];
	const Figura *blackCaptured[15];
	int whiteCapturedIt;
	int blackCapturedIt;
	bool whiteTurn;
	Save *lastMove;
	int blackKingX, blackKingY, whiteKingY, whiteKingX;
	bool checked;
	bool castling;

	inline void resetColor();
	inline Color getColor(char);
	Figura * getFigure(char);
	bool checkTrack(int,int,int,int);
	Color checkKing();
	void undo(Save *);

	public:
		Board();
		void move(int, int, int, int);
		bool isWhiteTurn();
#ifdef LINUX_VERSION
		void setColors(std::string, std::string, std::string);
#else
		void setColors(int, int, int);
#endif
		void setWhiteKingPos(int, int);
		void setBlackKingPos(int, int);
		void save();
		void load();
		void undo();
		void print();
		~Board();
};
#endif
