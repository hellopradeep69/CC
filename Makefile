CC = gcc

CFLAGS = -Wall -Wextra -lraylib

run:
	$(CC) main.c $(CFLAGS) -o main && ./main

compile:
	$(CC) main.c $(CFLAGS) -o main
