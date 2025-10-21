#include <stdint.h>
#include <stdlib.h>

#ifndef CCHIP8_H_
#define CCHIP8_H_

typedef struct Chip8 Chip8;
struct Chip8 Chip8CreateCpu();
void Chip8MapRom(Chip8 chip8, uint8_t *rom, ulong size);

#endif
