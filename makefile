OBJS = main.o Game.o RandomPlayer.o MonteCarloPlayer.o HumanPlayer.o Viewer.o BoardCoord.o Graph.o Board.o UserAbort.o
CC = g++
DEBUG = -g
CFLAGS = -std=c++11 -Wall -c $(DEBUG)
LFLAGS = -std=c++11 -Wall $(DEBUG)
UPDATE = touch

a.exe : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o a.exe

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

Game.o : Game.h Game.cpp Board.h Player.h Piece.h
	$(CC) $(CFLAGS) Game.cpp

RandomPlayer.o : RandomPlayer.h RandomPlayer.cpp Player.h Board.h BoardCoord.h
	$(CC) $(CFLAGS) RandomPlayer.cpp

MonteCarloPlayer.o : MonteCarloPlayer.h MonteCarloPlayer.cpp Player.h Board.h BoardCoord.h Viewer.h
	$(CC) $(CFLAGS) MonteCarloPlayer.cpp

HumanPlayer.o : HumanPlayer.h HumanPlayer.cpp Player.h Board.h BoardCoord.h Viewer.h
	$(CC) $(CFLAGS) HumanPlayer.cpp
    
Viewer.o : Viewer.h Viewer.cpp Board.h
	$(CC) $(CFLAGS) Viewer.cpp

BoardCoord.o : BoardCoord.h BoardCoord.cpp
	$(CC) $(CFLAGS) BoardCoord.cpp

Graph.o : Graph.h Graph.cpp
	$(CC) $(CFLAGS) Graph.cpp

Board.o : Board.h Board.cpp Piece.h BoardCoord.h Graph.h
	$(CC) $(CFLAGS) Board.cpp

UserAbort.o : UserAbort.h UserAbort.cpp
	$(CC) $(CFLAGS) UserAbort.cpp

clean:
	rm -f *.o a.exe a.exe.stackdump *.h.gch
