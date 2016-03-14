#include "Board.h"
#include<iostream>
#include<fstream>

#ifndef LINUX_VERSION
#include<Windows.h>
#endif

Board::Save::Save(int oldX, int oldY, int newX, int newY, const Figura *oldF, const Figura *newF, bool castling, Save *prev, Color color=UNDEFINED){
	this->oldX=oldX;
	this->oldY=oldY;
	this->newX=newX;
	this->newY=newY;
	this->oldF=oldF;
	this->newF=newF;
	this->oldfm=oldF->firstMove;
	this->newfm=newF->firstMove;
	this->castling=castling;
	this->prev=prev;
	this->capturedColor=color;
	std::cout<<(capturedColor==WHITE?"WHITE":"BLACK")<<std::endl;
}

Board::Board(){
	this->whiteTurn=true;
	for(int x=0;x<8;x++)
		for(int y=0;y<8;y++)
			board[x][y]=new Figura(UNDEFINED);
	for(int i=0;i<8;i++)
		board[6][i]=new Pion(BLACK);

	board[7][0]=new Wieza(BLACK);
	board[7][7]=new Wieza(BLACK);
	board[7][1]=new Skoczek(BLACK);
	board[7][6]=new Skoczek(BLACK);
	board[7][2]=new Goniec(BLACK);
	board[7][5]=new Goniec(BLACK);
	board[7][4]=new Krol(BLACK);
	Figura::setWhiteKingPos(0,4);
	board[7][3]=new Hetman(BLACK);

	for(int i=0;i<8;i++){
		board[1][i]=new Pion(WHITE);
	}
	board[0][0]=new Wieza(WHITE);
	board[0][7]=new Wieza(WHITE);
	board[0][1]=new Skoczek(WHITE);
	board[0][6]=new Skoczek(WHITE);
	board[0][2]=new Goniec(WHITE);
	board[0][5]=new Goniec(WHITE);
	board[0][4]=new Krol(WHITE);
	Figura::setBlackKingPos(7,4);
	board[0][3]=new Hetman(WHITE);

	this->lastMove=NULL;
	this->checked=false;

	for(int i=0;i<15;i++){
		whiteCaptured[i]=NULL;
		blackCaptured[i]=NULL;
	}
	whiteCapturedIt=0;
	blackCapturedIt=0;

	this->castling=false;
}

bool Board::checkTrack(int x, int y, int newX, int newY){
	if(x==newX){
		for(int i=(y>newY?newY:y)+1;i<(y>newY?y:newY);i++){
			if(board[x][i]->getChar()!='+')
				return false;
		}
		return true;
	} else if(y==newY){
		for(int i=((x>newX)?newX:x)+1;i<((x>newX)?x:newX);i++){
			if(board[i][y]->getChar()!='+')
				return false;
		}
		return true;
	} else if(std::abs(x-newX)==std::abs(y-newY)){
		for(int i=x,j=y;i!=newX&&j!=newY;i+=(x>newX?(-1):1), j+=(y>newY?(-1):1)){
			if((i==x)&&(j==y))
				continue;
			if(board[i][j]->getChar()!='+')
				return false;
		}
		return true;
	} else
		return false;
}

Color Board::checkKing(){
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++){
			if(board[i][j]->getColor()!=UNDEFINED){
				if(board[i][j]->isWhite()){
					if((tolower(board[i][j]->getChar())=='p'?false:(board[i][j]->checkMove(i-Figura::blackKingX(), j-Figura::blackKingY())))
					   &&(tolower(board[i][j]->getChar())=='s'?true:checkTrack(i,j, Figura::blackKingX(), Figura::blackKingY()))){
						return BLACK;
					}
					if((std::abs(Figura::blackKingY()-j)==1)&&(i==Figura::blackKingX()-1)){
						return BLACK;
					}
				} else{
					if((tolower(board[i][j]->getChar())=='p'?false:(board[i][j]->checkMove(i-Figura::whiteKingX(), j-Figura::whiteKingY())))
					   &&(tolower(board[i][j]->getChar())=='s'?true:checkTrack(i,j, Figura::whiteKingX(), Figura::whiteKingY()))){
						return WHITE;
					}
					if((std::abs(Figura::whiteKingY()-j)==1)&&(i==Figura::whiteKingX()+1)){
						return WHITE;
					}
				}
			}

		}
	return UNDEFINED;
}

void Board::undo(Save *save){
    if(save==NULL)
        throw new NoPrevSave();
	board[save->oldX][save->oldY]=save->oldF;
	board[save->newX][save->newY]=save->newF;
	board[save->newX][save->newY]->firstMove=save->newfm;
	board[save->oldX][save->oldY]->firstMove=save->oldfm;
	this->castling=save->castling;
	if(save->capturedColor!=UNDEFINED){
		if(save->capturedColor==WHITE){
			std::cout<<"WHITE__";
			this->whiteCapturedIt--;
		}
		else{
			std::cout<<"BLACK__";
			this->blackCapturedIt--;
		}
	}
    if(board[save->oldX][save->oldY]->getChar()=='K')
        Figura::setWhiteKingPos(save->oldX, save->oldY);
    else if(board[save->oldX][save->oldY]->getChar()=='k')
        Figura::setBlackKingPos(save->oldX, save->oldY);
	this->lastMove=save->getPrev();
	whiteTurn=!whiteTurn;
}

void Board::undo(){
    Board::undo(lastMove);
	if(this->castling){
		Board::undo(lastMove);
		whiteTurn=!whiteTurn;
	}
}

Figura * chooseNewFigure(Color color){
	while(true){
		std::cout<<"Wybierz figure.\n";
		std::cout<<"\t1)Hetman\n"
			"\t2)Goniec\n"
			"\t3)Skoczek\n"
			"\t4)Wieza\n"
			"Podaj numer: ";
		int choose;
		std::cin>>choose;
		if((choose<1)&&(choose>4)){
			std::cout<<"Podana przez Ciebie liczba jest troszeczke poza zakresem. (zakres nalezy <1,4>, gdzie liczba wprowadzana nalezy do calkowitych)";
			continue;
		}
		switch(choose){
			case 1: return new Hetman(color);
				break;
			case 2: return new Goniec(color);
				break;
			case 3: return new Skoczek(color);
				break;
			case 4: return new Wieza(color);
				break;
			default:
				std::cout<<"Wpisano nieprawidlowa liczbe.\n";
				continue;
		}
	}
}

void Board::move(int x, int y, int newX, int newY){
	x--;
	newX--;
	if(x>7||x<0||y<0||y>7||newX<0||newX>7||newY<0||newY>7)
		throw new RangeException();
	if(board[x][y]->getChar()=='+')
		throw new NullField();
	if(whiteTurn!=board[x][y]->isWhite())
		throw new TurnException();
	if((tolower(board[x][y]->getChar())!='s')&&!checkTrack(x, y, newX, newY))
		throw new TrackNotEmptyException();
	Color checkColor;
	const Figura *tmp;
	if(!(board[x][y]->checkMove(x-newX, y-newY))){
		if(tolower(board[x][y]->getChar())=='p'){
			if(std::abs(x-newX)==1&&std::abs(y-newY)==1){
				if((board[newX][newY]->getChar())=='+')
					throw new MoveException();
			} else
				throw new MoveException();
		}

		else if((x==newX)&&(std::abs(y-newY)==2)&&(tolower(board[x][y]->getChar())=='k')){
			int diff;
			if(tolower(board[newX][newY+1]->getChar())=='w')
				diff=1;
			else if(tolower(board[newX][newY-2]->getChar())=='w')
				diff=-2;
			else
				throw new MoveException();
			if(((diff==1)&&board[newX][newY-1]->getChar()!='+')||((diff==-2)&&board[newX][newY+1]->getChar()!='+'))
				throw new FieldIsBusyException();
			if((board[x][y]->firstMove)&&(board[newX][newY+diff]->firstMove)){
                if(board[x][y]->getChar()=='k')
                    Figura::setBlackKingPos(x,y+(diff==-2?-1:1));
                else
                    Figura::setWhiteKingPos(x,y+(diff==-2?-1:1));
                if((checkColor=checkKing())!=UNDEFINED){
                    if(board[x][y]->getChar()=='k')
                        Figura::setBlackKingPos(x,y);
                    else
                        Figura::setWhiteKingPos(x,y);
                    throw new Check("Szach na polu, przez ktore przechodzi krol.");
                }

				this->lastMove=new Save(x, y, newX, newY, board[x][y], board[newX][newY], castling,  lastMove);
				tmp=board[x][y];
				tmp->firstMove=false;
				board[x][y]=board[newX][newY];
				board[newX][newY]=tmp;
				this->castling=true;
				this->lastMove=new Save(newX, newY+diff, newX, newY+(diff==-2?1:-1), board[newX][newY+diff], board[newX][newY+(diff==-2?1:-1)], castling, lastMove);
                tmp=board[newX][newY+diff];
				tmp->firstMove=false;
				board[newX][newY+diff]=board[newX][newY+(diff==-2?1:-1)];
				board[newX][newY+(diff==-2?1:-1)]=tmp;
                if(board[newX][newY]->getChar()=='K')
                    Figura::setWhiteKingPos(newX, newY);
                else if(board[newX][newY]->getChar()=='k')
                    Figura::setBlackKingPos(newX, newY);
				if((checkColor=checkKing())!=UNDEFINED){
                    undo(lastMove);
                    throw new Check();
				}
				this->whiteTurn=!whiteTurn;
                return;
			}
			throw new MoveException();
		} else 
			throw new MoveException();
	}
	if(board[newX][newY]->getChar()!='+'){
		if((board[x][y]->getColor()==board[newX][newY]->getColor())||((tolower(board[x][y]->getChar())=='p')&&tolower(board[newX][newY]->getChar())=='p'&&y==newY))
			throw new FieldIsBusyException();
		if(board[newX][newY]->getColor()==WHITE){
			this->lastMove=new Save(x,y,newX, newY, board[x][y], board[newX][newY], castling, lastMove, WHITE);
			whiteCaptured[whiteCapturedIt++]=board[newX][newY];
		}
		else if(board[newX][newY]->getColor()==BLACK){
			this->lastMove=new Save(x,y,newX, newY, board[x][y], board[newX][newY], castling, lastMove, BLACK);
			blackCaptured[blackCapturedIt++]=board[newX][newY];
		}
		board[newX][newY]=new Figura(UNDEFINED);
	} else
		this->lastMove=new Save(x,y,newX, newY, board[x][y], board[newX][newY], castling, lastMove);

	tmp=board[newX][newY];
	board[newX][newY]=board[x][y];
	board[x][y]=tmp;
	if(board[newX][newY]->firstMove)
		board[newX][newY]->firstMove=false;
	if(board[newX][newY]->getChar()=='K')
		Figura::setWhiteKingPos(newX,newY);
	if(board[newX][newY]->getChar()=='k')
		Figura::setBlackKingPos(newX,newY);

	if(tolower(board[newX][newY]->getChar())=='p'){
		if(newX==0||newX==7)
			board[newX][newY]=chooseNewFigure(board[newX][newY]->getColor());
	}
	whiteTurn=!whiteTurn;
	if((checkColor=checkKing())!=UNDEFINED){
		if(whiteTurn==board[newX][newY]->isWhite()){
			undo(lastMove);
			throw new Check();
		}
		if(checked){
			undo(lastMove);
			throw new Check();
		} else{
			if(checkColor==board[newX][newY]->getColor()){
				undo(lastMove);
				if(board[x][y]->getChar()=='K')
					Figura::setWhiteKingPos(x,y);
				if(board[x][y]->getChar()=='k')
					Figura::setBlackKingPos(x,y);
				throw new Check();
			}
			this->checked=true;
			throw new Check();
		}
	} else
		this->checked=false;
}

#ifndef LINUX_VERSION
inline void Board::setColors(int newBlack, int newWhite, int newDefault){
	Figura::setColorCodes(newBlack, newWhite, newDefault);
}
#else
inline void Board::setColors(std::string newWhite, std::string newBlack, std::string newDefault){
	Figura::setColorCodes("\033["+newBlack, "\033["+newWhite, "\033["+newDefault);
}
#endif

void Board::save(){
	std::ofstream out;
	out.open(".save");
	out<<whiteTurn<<std::endl;
	for(int x=0;x<8;x++)
		for(int y=0;y<8;y++)
			if(board[x][y]->getChar()!='+')
				out<<x<<y<<((std::string)*(board[x][y]))<<std::endl;
	out<<"E"<<std::endl;
	for(int i=0;i<whiteCapturedIt;i++)
		out<<((std::string)*(whiteCaptured[i]))<<std::endl;
	out<<"E"<<std::endl;
	for(int i=0;i<blackCapturedIt;i++)
		out<<((std::string)*(blackCaptured[i]))<<std::endl;
	out.close();
}



inline Color Board::getColor(char c){
	if(c>='a'&&c<='z')
		return BLACK;
	if(c>='A'&&c<='Z')
		return WHITE;
	throw new IOException("Zapis uszkodzony.");
}

Figura * Board::getFigure(char c){
	switch(tolower(c)){
		case 'p':
			return new Pion(getColor(c));
		case 'k':
			return new Krol(getColor(c));
		case 'h':
			return new Hetman(getColor(c));
		case 'g':
			return new Goniec(getColor(c));
		case 's':
			return new Skoczek(getColor(c));
		case 'w':
			return new Wieza(getColor(c));
		default:
			throw new IOException("Zapis uszkodzony(wczytywanie figury).");
	}
}

inline void arrcpy(const Figura *src[][8], const Figura *dest[][8]){
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
			dest[i][j]=src[i][j];
}

void Board::load(){
	std::ifstream in;
	in.open(".save");
	if(!in.good()){
		in.close();
		throw new IOException("Brak zapisu.");
	}
	bool turn;
	in>>turn;
	const Figura *restored[8][8];
	for(int x=0;x<8;x++)
		for(int y=0;y<8;y++)
			restored[x][y]=new Figura(UNDEFINED);
	std::string buff;
	in>>buff;
	const Figura *f;
	whiteCapturedIt=blackCapturedIt=0;
	while(buff!="E"){
		f=getFigure(buff[2]);
		restored[buff[0]-'0'][buff[1]-'0']=f;
		f->firstMove=(buff[3]=='1');
		if(f->getChar()=='k')
			Figura::setBlackKingPos(buff[0]-'0', buff[1]-'0');
		else if(f->getChar()=='K')
			Figura::setWhiteKingPos(buff[0]-'0', buff[1]-'0');
		in>>buff;
	}
	in>>buff;
	while(buff!="E"){
		whiteCaptured[whiteCapturedIt++]=getFigure(buff[0]);
		in>>buff;
	}
	in>>buff;
	while(!in.eof()){
		blackCaptured[blackCapturedIt++]=getFigure(buff[0]);
		in>>buff;
	}
	in.close();
	arrcpy(restored, board);
	this->whiteTurn=turn;
}

inline void Board::resetColor(){
#ifndef LINUX_VERSION
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#else
	std::cout<<"\033[0m";
#endif
}

bool Board::isWhiteTurn(){
	return this->whiteTurn;
}

void Board::print(){
	resetColor();
	std::cout<<"   A B C D E F G H\n";
	std::cout<<"   ---------------\n";

	for(int x=7;x>=0;x--){
		std::cout<<x+1<<'|'<<' ';
		for(int y=0;y<8;y++){
#ifndef LINUX_VERSION
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), board[x][y]->getColorCode());
			std::cout<<board[x][y]->getChar()<<' ';
#else
			std::cout<<board[x][y]->getColorCode()<<board[x][y]->getChar()<<' ';
#endif
		}
		resetColor();
		std::cout<<'|'<<x+1;
		if(x<whiteCapturedIt){
#ifndef LINUX_VERSION
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), whiteCaptured[x]->getColorCode());
			std::cout<<'\t'<<(whiteCaptured[x]->getChar());
#else
			std::cout<<'\t';
			for(int i=x;i<whiteCapturedIt;i+=8)
				std::cout<<whiteCaptured[x]->getColorCode()<<whiteCaptured[x]->getChar()<<' ';
#endif
			resetColor();
		} else
			std::cout<<'\t';

		if(x<blackCapturedIt
				){
#ifndef LINUX_VERSION
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), blackCaptured[x]->getColorCode());
			std::cout<<'\t'<<(blackCaptured[x]->getChar());
#else
			std::cout<<'\t';
			for(int i=x;i<blackCapturedIt;i+=8)
				std::cout<<blackCaptured[x]->getColorCode()<<blackCaptured[x]->getChar()<<' ';
#endif
			resetColor();
		}

		std::cout<<std::endl;
	}
	std::cout<<"   ---------------\n";
	std::cout<<"   A B C D E F G H\n\n";
}

Board::~Board(){
	for(int x=0;x<8;x++)
		for(int y=0;y<8;y++)
			delete board[x][y];
}
