default: help

CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

run: compile # run and compile
	./main

compile: # compile
	$(CC) $(CFLAGS) src/main.c src/navigation.c -o main $(LDLIBS)

clean: main # clean
	rm main


.PHONY: help
help: # am i helpful ?
	@grep -E '^[a-zA-Z0-9 -]+:.*#'  Makefile | sort | while read -r l; do printf "\033[1;32m$$(echo $$l | cut -f 1 -d':')\033[00m:$$(echo $$l | cut -f 2- -d'#')\n"; done
