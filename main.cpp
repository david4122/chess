//WERSJA NA LINUXA!!!
//ZMIENIC W version.info
#include "version.info"

#include<iostream>
#include<fstream>
#include "Figura.h"
#include "Board.h"

using namespace std;

const char *helpMessage=
	"help\t-\twyswietl ta wiadomosc\n"
	"quit\t-\t\n"
	"exit\t-\twyjscie\n"
	"colors\t-\tzmiana kolorow \n"
	"save\t-\tzapisz gre\n"
	"load\t-\tzaladuj gre\n"
	"surr\t-\tpoddanie sie (Wpisz jesli jest mat lub pat)\n"
	"restart\t-\treset gry\n"
	"undo\t-\tcofnij\n"
	"Aby przesunac figure nalezy wpisac sekwencje <obecnaKolumnaObecnyRzadNowaKolumnaNowyRzad>, np.a2a4";

void printLogo(){
	ifstream in;
	in.open("logo");
	string buff;
	system(CLEAR_COMMAND);
	while(!in.eof()){
		getline(in, buff);
		cout<<buff<<endl<<flush;
	}
	in.close();
	cout<<"\tDawid Brewinski\n";
	cout<<"\tMichal Pompa\n";
	cout<<"\tMichal Wilczak\n";
	cout<<endl;
	cout<<"\thttps://github.com/david4122/chess.git\n"<<endl;

	cout<<"Wybierz \"Pomoc\", jesli uruchamiasz po raz pierwszy."<<endl;
}

void end(){
	system(CLEAR_COMMAND);
	ifstream in("logo");
	string buff;
	while(!in.eof()){
		getline(in, buff);
		cout<<buff<<endl;
	}
	cout<<"\tDawid Brewinski\n";
	cout<<"\tMichal Pompa\n";
	cout<<"\tMichal Wilczak\n";
	cout<<"\tOlaf(grafika)\n";
	cout<<endl;
	cout<<"Kod do wgladu pod adresem internetowym https://github.com/david4122/chess.git\n"<<endl;
	in.close();

}

inline int convert(char c){
	int x=(c-'0'>=0&&c-'0'<=9)?(c-'0'):(tolower(c)-'a');
	if(x>8)
		throw new IOException("FORMAT WEJSCIA: <kolumna><rzad><nowaKolumna><nowyRzad>, np. a2a4");
	return x;
}

void printMessage(const char *c){
	cout<<c<<endl;
#ifndef LINUX_VERSION
	system("PAUSE");
#else
	getchar();
#endif
}

void playFromFile(){
	ifstream moves("moves");
	Board *board=new Board();
	string line;
	int currX, currY, newX, newY;
	try{
		do {
			moves>>line;
			if(!moves){
				cout<<"KONIEC PARTII\n";
				getchar();
				break;
			}
			system(CLEAR_COMMAND);
			cout<<"->"<<line<<"<-"<<endl;
			currY=convert(line[0]);
			currX=convert(line[1]);
			newY=convert(line[2]);
			newX=convert(line[3]);
			try {
				board->move(currX, currY, newX, newY);
			} catch(Check *e){
				cout<<"\tSZACH\n";
			}
			board->print();
			for(int i=0;i<200000000;i++);
		} while(true);
	} catch(Exception *e){
		printMessage("FILE CORRUPTED");
	}
	moves.close();
}

void play(){
	Board *board=new Board();
	int currX, currY, newX, newY;
	string input;
	do{
		try{
			system(CLEAR_COMMAND);
			cout<<((board->isWhiteTurn())?"BIALE":"CZARNE")<<endl;
			board->print();
			cout<<"<XYnewXnewY> ";
			getline(cin, input);
			if(input=="")
				continue;
			if(input=="help"){
				cout<<helpMessage;
				getchar();
				continue;
			}
			if(input=="restart"){
				delete board;
                board=new Board();
                continue;
			}
			if(input=="undo"){
                board->undo();
                continue;
			}
			if(input=="quit"||input=="exit")
				break;
			if(input=="colors"){
#ifdef LINUX_VERSION
				cout<<"KODY: \"x;y;z\", gdzie x to styl (0 do 8), y to kolor czcionki (30 do 38), a z to kolor podswietlenia (40 do 48). \"0\" wylacza kolor.\n";
				string newWhite;
				string newBlack;
				string newDefault;
#else
				cout<<"KODY: od 0 do 15 zmienia sam kolor, od 16 do 255 zmienia kolor i tlo\n";
                int newWhite;
                int newBlack;
                int newDefault;
#endif
				cout<<"KOD DLA BIALYCH: ";
				cin>>newWhite;
				cout<<"KOD DLA CZARNYCH: ";
				cin>>newBlack;
				cout<<"KOD DOMYSLNY: ";
				cin>>newDefault;
				board->setColors(newWhite, newBlack, newDefault);
				continue;
			}
			if(input=="save"){
				board->save();
				continue;
			}
			if(input=="load"){
				board->load();
				continue;
			}
			if(input=="surr"){
                throw new GameOver();
			}
			if(input.length()!=4){
				throw new IOException("FORMAT WEJSCIA: <kolumna><rzad><nowaKolumna><nowyRzad>, np. a2a4");
			}
			currY=convert(input[0]);
			currX=convert(input[1]);
			newY=convert(input[2]);
			newX=convert(input[3]);

			board->move(currX, currY, newX, newY);
		} catch(TurnException *e){
			printMessage("Nie twoja kolej!");
		} catch(FieldIsBusyException *e){
			printMessage("To pole jest zajete.");
		} catch(MoveException *e){
			printMessage("Nie mozesz tego zrobic.");
		} catch(RangeException *e){
			printMessage("Pole poza plansza.");
		} catch(IOException *e){
			printMessage(e->getReason().c_str());
		} catch(TrackNotEmptyException *e){
            printMessage("Tylko skoczek moze przeskakiwac figury.");
		} catch(Check *e){
		    printMessage(e->getReason().c_str());
		} catch(NoPrevSave *e){
            printMessage("Brak wczesniejszego zapisu.");
		} catch(NullField *e){
			printMessage("To pole jest puste.");
		} catch(GameOver *e){
            cout<<"Gracz "<<(board->isWhiteTurn()?"bialy":"czarny")<<" przegral."<<endl;
			getchar();
            break;
		}
	} while(true);
	delete board;
}

int main(){
	bool flag=false;
	do {
		printLogo();
		cout<<"1) Graj\n"
			"2) Wyswietl partie pokazowa\n"
			"3) Pomoc\n"
			"4) Wyjscie\n";
		if(flag){
			cout<<"Wybierz cyfre od 1 do 4!";
			flag=false;
		}
		int choise;
		cin>>choise;
		cin.ignore(1);
		switch(choise){
			case 1:
				play();
				break;
			case 2:
				playFromFile();
				break;
			case 3:
				printMessage(helpMessage);
				break;
			case 4:
				goto et;
			default:
				flag=true;
		}
	} while(true);
et:
	end();
}
