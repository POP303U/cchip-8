#include "cchip8.h"
#include "cchip8ins.h"
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
  Instruction *ins = malloc(sizeof(Instruction));

  memset(chip8, 0, sizeof(Chip8));
  memset(ins, 0, sizeof(Instruction));
  if (chip8 == NULL || ins == NULL) {
    fprintf(stderr, "Chip8InitCpu: could not alloc");
  }

  chip8->I = 0;
  chip8->PC = 0x200; // Execution starts at 0x200
  chip8->SP = 0;
  chip8->soundTimer = 0;
  chip8->delayReg = 0;
  chip8->ins = *ins;

  // Load Font into memory
  for (int i = 0; i < FONTSET_SIZE; i++) {
    chip8->memory[i + FONT_START] = font[i];
  }

  // RNG for chip8
  srand(time(NULL));

  return chip8;
}

// CPU execution cycle: FDE (Fetch, Decode, Execute)
// Fetch Instruction from memory and return it
void Chip8FetchInstruction(Chip8 *chip8) {
  uint8_t addr0 = chip8->memory[chip8->PC];
  uint8_t addr1 = chip8->memory[chip8->PC + 0x01];

  // shift two bytes into a short to create a opcode
  uint16_t opcode = ((uint16_t)addr0 << 8) | addr1;

  // Save current executing opcode into the chip8
  chip8->ins.opcode = opcode;
}

// Decode the current Instruction and fill Instruction struct
void Chip8DecodeInstruction(Chip8 *chip8) {
  // Currently processed opcode
  uint16_t opcode = chip8->ins.opcode;

  // All of this is just extraction of values from opcodes
  chip8->ins.nnn = opcode & 0x0FFF;
  chip8->ins.n = opcode & 0x000F;
  chip8->ins.x = (opcode & 0x0F00) >> 8;
  chip8->ins.y = (opcode & 0x00F0) >> 4;
  chip8->ins.kk = opcode & 0x00FF;

  // Assign for the funcptr table
  chip8->ins.lbit = (opcode & 0xF000) >> 12;
  //printf("nnn: %X\nn: %X\nx: %X\ny: %X\nkk: %X",chip8->ins.nnn,chip8->ins.n,chip8->ins.x,chip8->ins.y,chip8->ins.kk);
}

// Compares extracted bits to Funcptr table and executes them
// (This does not work for opcodes with multiple signatures)
void Chip8ExecuteInstruction(Chip8 *chip8) {
  // Preincrement PC
  chip8->PC += 2;

  // Left bit indicates which instruction to execute
  const uint8_t lbit = chip8->ins.lbit;

  // For opcodes with a unique left byte
  switch (lbit) {
    case 1 ... 7: 
      chip8insTable[lbit](chip8);
    default:
    break;
  }
}

void Chip8UpdateState(Chip8 *chip8);

void Chip8LoadRom(struct Chip8 *chip8, uint8_t *rom, uint16_t size) {
  // Map rom into chip8 starting from 0x200
  for (uint16_t i = ROM_START; i < (ROM_START + size); i++) {
    chip8->memory[i] = rom[i - ROM_START];
    // moving around memory like this is hard
    // printf("chip8->memory[%02X] = rom[%02X], val(%02X)\n", ROM_START + i, i
    // - ROM_START, chip8->memory[i]);
  }
}

long Chip8RomSize(const char *filename) {
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
