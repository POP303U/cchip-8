#include "cchip8.h"
#include "font.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ROM_START 0x200
#define FONT_START 0x50
#define FONTSET_SIZE 80
#define BINARY_READ "rb"

Chip8 *Chip8InitCpu() {
  Chip8 *chip8 = malloc(sizeof(Chip8));

  memset(chip8, 0, sizeof(Chip8));
  if (chip8 == NULL) {
    fprintf(stderr, "Chip8InitCpu: could not alloc");
  }

  chip8->I = 0;
  chip8->PC = 0x200; // Execution starts at 0x200
  chip8->SP = 0;
  chip8->soundTimer = 0;
  chip8->delayReg = 0;

  // Load Font into memory
  for (int i = 0; i < FONTSET_SIZE; i++) {
    chip8->memory[i + FONT_START] = font[i];
  }

  // RNG for chip8
  srand(time(NULL));

  return chip8;
}

// CPU execution cycle: FDE (Fetch, Decode, Execute)
uint16_t Chip8FetchInstruction(Chip8 *chip8) {
  uint8_t addr0 = chip8->memory[chip8->PC];
  uint8_t addr1 = chip8->memory[chip8->PC + 0x01];

  // shift two bytes into a short
  return ((uint16_t)addr0 << 8) | addr1;
}

void Chip8DecodeInstruction(Chip8 *chip8, uint16_t opcode) {
  // All of this is just extraction of values from opcodes
  uint16_t nnn = opcode & 0x0FFF;
  uint16_t n = opcode & 0x000F;
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  uint16_t kk = opcode & 0x00FF;
  uint16_t lbit_opcode = (opcode & 0xF000) >> 12;

  // Preincrement PC
  chip8->PC += 2;

  // For opcodes with a unique left byte
  switch (lbit_opcode) {
  default:
    printf("lbit: %X\n ", lbit_opcode);
    break;
  }
}
void Chip8ExecuteInstruction(Chip8 *chip8);
void Chip8UpdateState(Chip8 *chip8);

void Chip8LoadRom(struct Chip8 *chip8, uint8_t *rom, ulong size) {
  // Map rom into chip8 starting from 0x200
  for (ulong i = ROM_START; i < (ROM_START + size); i++) {
    chip8->memory[i] = rom[i - ROM_START];
    // moving around memory like this is hard
    // printf("chip8->memory[%02X] = rom[%02X], val(%02X)\n", ROM_START + i, i
    // - ROM_START, chip8->memory[i]);
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
  for (uint16_t i = 0; i < sz; i++) {
    ret[i] = buf[i];
  }

  free(buf);
  return ret;
}

void Chip8DumpMem(struct Chip8 *chip8) {
  int idx = 0;
  for (int i = 0; i <= 0x200; i++) {
    printf("0x%03X ", i * 16);
    for (int j = 0; j < 16; j++) {
      printf("%02X ", chip8->memory[idx]);
      idx++;
    }
    printf("\n");
    if (idx >= 0x900)
      break;
  }
}
