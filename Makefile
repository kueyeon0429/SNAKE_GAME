CC=g++

all:
	$(CC) -std=c++11 -o snake.c snake.h kbhit.c -lncursesw
