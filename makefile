all: main

main: Board.o Figura.o main.o
	g++ -o main Board.o Figura.o main.o


Board.o: Board.cpp
	g++ -o Board.o -c Board.cpp

Figura.o: Figura.cpp
	g++ -o Figura.o -c Figura.cpp

main.o:
	g++ -o main.o -c main.cpp

clean:
	rm *.o
