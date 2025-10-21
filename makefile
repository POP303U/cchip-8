CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = main.c lib/rom.c lib/cchip8.c lib/tests.c
OBJ = $(SRC:.c=.o)
OUT = chip8

all: $(OBJ)

$(OUT):
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) `sdl2-config --cflags --libs`

run:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) `sdl2-config --cflags --libs`
	./chip8

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUT)
