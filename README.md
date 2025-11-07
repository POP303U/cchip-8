# Chip-8 Emulator

A Chip-8 Emulator written in C and SDL2.

Personal attempt at Emulation Development, not an accurate emulator.

_CHIP-8 is an interpreted programming language developed by Joseph Weisbecker in the 1970s. It was made to allow video games to be more easily programmed for 8-bit microcomputers at the time, and runs on a CHIP-8 virtual machine._

## Screenshots
<img alt="chip8_tetris" src="assets/tetris.png">
<img alt="chip8_pong2" src="assets/pong2.png">

## Building

### Dependencies

+ **C99 Compiler**
  + **Linux:** GCC version 9.0+
  + **Windows:** MinGW-w64 GCC 9.0+

+ **SDL2**
  + Install for your platform: https://wiki.libsdl.org/SDL2/Installation


If you are on Windows install these packages inside of MSYS2 MinGW64.
```sh
pacman -S mingw-w64-x86_64-make mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2
```

### Compiling

> [!Caution]
> If you are on Windows, use the MSYS2 MinGW64 Shell to execute the following commands.

**Linux and Windows**
```sh
$ make <output>
```

## Usage
```sh
$ ./chip8 <rom>
```

## Development
To test and debug the emulator use ./build for logging:
```sh
./build <rom>
```

## Keyboard Layout:

### Chip8 Keypad:
|   |   |   |   |
|---|---|---|---|
| 1 | 2 | 3 | C |
| 4 | 5 | 6 | D |
| 7 | 8 | 9 | E |
| A | 0 | B | F |

### Emulator Keyboard Mapping:
|   |   |   |   |
|---|---|---|---|
| 1 | 2 | 3 | 4 |
| Q | W | E | R |
| A | S | D | F |
| Z | X | C | V |