CC = gcc
# i hate how these compiler options come from gcc bugs
CFLAGS = -std=c99 -Wpedantic -Wall -Wextra -O2
SRC = main.c lib/cchip8.c lib/cchip8ins.c lib/tests.c lib/font.c lib/keyboard.c
OBJ = $(SRC:.c=.o)
OUT = chip8

all: $(OBJ)

$(OUT):
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) `sdl2-config --cflags --libs`

run:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) `sdl2-config --cflags --libs`
	./chip8 $(FILE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUT)
	rm log
