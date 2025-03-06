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
    // Map rom into chip8 starting from 0x200
    for (ulong i = ROM_START; i < (ROM_START + size); i++) {
      chip8->memory[i] = rom[i - ROM_START];
      // moving around memory like this is hard
      // printf("chip8->memory[%02X] = rom[%02X], val(%02X)\n", ROM_START + i, i - ROM_START, chip8->memory[i]);
    }
}

void Chip8MapFont(struct Chip8 *chip8, const uint8_t *font) {
    // Map font into chip8 starting from 0x000
    for (ulong i = 0x000; i < ROM_START; i++) {
      chip8->memory[i] = font[i];
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

void Chip8DumpMem(struct Chip8 *chip8) {
    int idx = 0;
    for (int i = 0; i <= 0x200; i++) {
        printf("0x%03X ", i * 16); 
        for (int j = 0; j <= 8; j++) {
            printf("%02X%02X ", chip8->memory[idx], chip8->memory[idx + 1]);
            idx++;
        }
        printf("\n");
        if (idx >= 0x900)
            break;
    }
}
