#include "cchip8.h"
#include <stdint.h>
#include <stdio.h>

#define ROM_START 0x200
#define BINARY_READ "rb"

struct Chip8 Chip8CreateCpu() {
  uint8_t mem[4096] = {0};
  uint8_t vx[16] = {0};
  uint8_t stack[64] = {0};
  uint8_t fb[64 * 32] = {0};

  struct Chip8 ret = {mem[4096 - 1], vx[16 - 1], stack[64 - 1], fb[2048 - 1], 0, 0, 0, 0, 0};
  return ret;
}

void Chip8MapRom(struct Chip8 *chip8, uint8_t *rom, ulong size) {
    for (ulong i = ROM_START; i < (ROM_START + size); i++) {
      chip8->memory[i] = rom[i - ROM_START];
      // moving around memory like this is hard
      // printf("chip8->memory[%02X] = rom[%02X], val(%02X)\n", ROM_START + i, i - ROM_START, chip8->memory[i]);
    }
}

ulong Chip8RomSize(const char *filename) {
  FILE *fptr = fopen(filename, BINARY_READ);

  // Get the file size
  fseek(fptr, 0L, SEEK_END);
  long sz = ftell(fptr);
  fseek(fptr, 0L, SEEK_SET);
  rewind(fptr);

  fclose(fptr);

  return sz;
}

uint8_t *Chip8ReadRom(const char *filename) {
  FILE *fptr = fopen(filename, BINARY_READ);

  if (fptr == NULL) {
    exit(-1);
  }

  // Get the file size
  fseek(fptr, 0L, SEEK_END);
  long sz = ftell(fptr);
  fseek(fptr, 0L, SEEK_SET);
  rewind(fptr);

  // Read into buf until eof
  uint8_t *buf = malloc(sz);
  fread(buf, sz, 1, fptr);
  fclose(fptr);

  uint8_t *ret = malloc(sz);
  if (!ret) {
    return NULL;
  }

  // Loop and read every byte into the program rom
  for (u_int16_t i = 0; i < sz; i++) {
    ret[i] = buf[i];
  }

  free(buf);
  return ret;
}
