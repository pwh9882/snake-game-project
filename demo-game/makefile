# Makefile
CC = g++
CPPFLAGS = -lncurses -std=c++14
OBJECTS = demo-game.o


demo-game : $(OBJECTS)
	$(CC) $(CPPFLAGS) $^ -o demo-game

%.o : %.cpp
	$(CC) $(CPPFLAGS) -c $^