CC = gcc

CFLAGS = -Wall -Wextra -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

run:
	$(CC) main.c $(CFLAGS) -o main && ./main

compile:
	$(CC) main.c $(CFLAGS) -o main

clean:
	rm main
