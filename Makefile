
reversi.exe: reversi.cpp reversi.h
	g++ -Wall -o reversi reversi.cpp

run: reversi.exe
	./reversi.exe

clean:
	rm reversi.exe

