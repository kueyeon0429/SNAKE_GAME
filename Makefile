CC=g++

all:
	$(CC) -std=c++11 -o main.o snake.cpp snake.h kbhit.h -lncursesw
