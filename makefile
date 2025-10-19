CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = main.c rom.c
OUT = chip8

default:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) `sdl2-config --cflags --libs`

run:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) `sdl2-config --cflags --libs`
	./chip8
