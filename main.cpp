//WERSJA NA LINUXA!!!
//Dla Windows skasowac ponizsza linie
#define LINUX_VERSION

#include<iostream>
#include<fstream>
#include<ctime>
#include "Figura.cpp"
#include "Board.cpp"
#include "Timer.cpp"

#ifdef LINUX_VERSION
#define CLEAR_COMMAND "clear"
#else
#define CLEAR_COMMAND "cls"
#endif

using namespace std;

string helpMessage=
	"help\t-\twyswietl ta wiadomosc\n"
	"quit\t-\t\n"
	"exit\t-\twyjscie\n"
	"colors\t-\tzmiana kolorow \n"
	"save\t-\tzapisz gre\n"
	"load\t-\tzaladuj gre\n"
	"surr\t-\tpoddanie sie (Wpisz jesli jest mat lub pat)\n"
	"restart\t-\treset gry\n"
	"undo\t-\tcofnij\n"
	"report\t-\tzostaw wiadomosc\n"
	"Aby przesunac figure nalezy wpisac sekwencje <obecnyRzadObecnaKolumnaNowyRzadNowaKolumna>, np.a2a4";

void printLogo(){
	ifstream in;
	in.open("logo");
	string buff;
	system(CLEAR_COMMAND);
	while(!in.eof()){
		getline(in, buff);
		cout<<buff<<endl<<flush;
		//system("sleep 1s");
	}
	cout<<"\tDawid Brewinski\n";
	cout<<"\tMichal Pompa\n";
	cout<<"\tMichal Wilczak\n";
	cout<<endl;
	cout<<"\thttps://github.com/david4122/chess.git\n"<<endl;

	cout<<"Wpisz \"help\", jesli uruchamiasz po raz pierwszy."<<endl;
#ifdef LINUX_VERSION
	cout<<"Naisnij \"enter\"."<<endl;
	getline(cin, buff);
#else
	system("PAUSE");
#endif
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

void report(string s){
	ofstream out("reports", ios_base::app);
	out.write(s.c_str(), s.length());
	out<<"\n---------------------------------------------------\n";
	out.close();
}

void printMessage(const char *c){
	cout<<c<<"\nNacisnij \"enter\""<<endl;
	getchar();
}

int main(){
	printLogo();
	Board *board=new Board();
	int currX, currY, newX, newY;
	string input;
	Timer total;
	do{
		try{
			//system(CLEAR_COMMAND);
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
				cout<<"KODY: \"x;y;zm\", gdzie x to styl (0 do 8), y to kolor czcionki (30 do 38), a z to kolor podswietlenia (40 do 48). \"0m\" wylacza kolor.\n";
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
			if(input=="report"){
				cout<<"wpisz wiadomosc do autora:\n";
				string res;
				getline(cin, res);
				report(res);
				continue;
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
	printMessage((string("Ta partia trwala ")+(string)total).c_str());
	end();
}
