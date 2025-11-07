CC = gcc
CFLAGS = -std=c99 -Wpedantic -Wall -Wextra -O2
STATIC = "" # comes from ./build
SRC = main.c lib/cchip8.c lib/cchip8ins.c lib/tests.c lib/font.c lib/keyboard.c
BIN = bin/*
OUT = chip8
OBJ = $(SRC:.c=.o)
LOG = $(SRC:.log)

all: $(OBJ)

$(OUT):
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(STATIC) `sdl2-config --cflags --libs`

run:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(STATIC) `sdl2-config --cflags --libs`
	./chip8 $(FILE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUT) $(LOG) $(BIN)
