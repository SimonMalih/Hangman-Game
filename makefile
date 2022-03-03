EX1: main.o Gamer.o Manager.o Person.o ReadWords.o Highlight.o HangmanGame.o MyList.o
	g++ main.o Gamer.o Manager.o Person.o ReadWords.o Highlight.o HangmanGame.o Mylist.o -o EX1
main.o: main.cpp
	g++ -c main.cpp
Gamer.o: Gamer.cpp Gamer.h
	g++ -c Gamer.cpp
Manager.o: Manager.cpp Manager.h
	g++ -c Manager.cpp
Person.o: Person.cpp Person.h
	g++ -c Person.cpp
ReadWords.o: ReadWords.cpp ReadWords.h
	g++ -c ReadWords.cpp
Highlight.o: Highlight.cpp Highlight.h
	g++ -c Highlight.cpp
HangmanGame.o: HangmanGame.cpp
	g++ -c HangmanGame.cpp
Mylist.o: MyList.cpp
	g++ -c MyList.cpp
clean:
	rm *.o EX1