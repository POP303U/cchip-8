#include "cchip8.h"
#include "cchip8ins.h"
#include "font.h"
#include "keyboard.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Chip8 *Chip8InitCpu() {
  Chip8 *chip8 = malloc(sizeof(Chip8));
  Instruction *ins = malloc(sizeof(Instruction));
  Keyboard *kbd = malloc(sizeof(Keyboard));

  if (chip8 == NULL || ins == NULL) {
    fprintf(stderr, "Chip8InitCpu: could not alloc");
  }

  memset(chip8, 0, sizeof(Chip8));
  memset(ins, 0, sizeof(Instruction));
  memset(kbd, 0, sizeof(Keyboard));

  chip8->I = 0;          // Index reg
  chip8->PC = 0x200;     // Execution starts at 0x200
  chip8->SP = 0;         // Points to the top of the stack
  chip8->soundTimer = 0; // Decrements each cycle by 1
  chip8->delayReg = 0;   
  chip8->cycles = 0;     // Track amount of cycles
  chip8->running = 1;    // Track if chip8 is running
  chip8->ins = *ins;     // Current instruction being executed
  chip8->kbd = *kbd;     // Stores keyboard input

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
}

// Reads the processed opcodes and fetches the corresponding function to
// execute, funcptr tables only work with unique lbit signatures...
void Chip8ExecuteInstruction(Chip8 *chip8) {
  // Preincrement PC
  chip8->PC += 2;

  // Increase cycle count
  chip8->cycles++;

  // Custom function that maps opcode to table entry
  uint16_t index = Chip8GetOpcodeIndex(chip8->ins.opcode);

  // Call function (with failsafe)
  if (chip8insTable[index] == NULL) {
      fprintf(stderr, "Access into NULL at chip8insTable[%d]\n", index);
      fprintf(stderr, "Probably encountered an invalid instruction\n");
      exit(1);
  }

  // Run opcode
  chip8insTable[index](chip8);
}

void Chip8UpdateState(Chip8 *chip8, uint64_t delta) {
  // Decrement delayReg by the amount of instructions?
  chip8->delayReg -= delta;
}

uint8_t Chip8GetOpcodeIndex(uint16_t opcode) {
  uint8_t high = (opcode >> 12) & 0xF;
  uint8_t low = opcode & 0xF;

  switch (high) {
    case 0x0: return (opcode == 0x00E0) ? 24 :
                     (opcode == 0x00EE) ? 25 : 0;
    case 0x1: return 1;
    case 0x2: return 2;
    case 0x3: return 3;
    case 0x4: return 4;
    case 0x5: return 5;
    case 0x6: return 6;
    case 0x7: return 7;
    case 0x8: return 15 + low;   // 8XY0–8XYE = indices 15–23
    case 0x9: return 9;
    case 0xA: return 10;
    case 0xB: return 11;
    case 0xC: return 12;
    case 0xD: return 13;
    case 0xE: return (opcode & 0xFF) == 0x9E ? 26 : 27;
    case 0xF:
      switch (opcode & 0xFF) {
        case 0x07: return 28;
        case 0x0A: return 29;
        case 0x15: return 30;
        case 0x18: return 31;
        case 0x1E: return 32;
        case 0x29: return 33;
        case 0x33: return 34;
        case 0x55: return 35;
        case 0x65: return 36;
      }
  } 
  // Access into null and will be caught
  return 0;
}

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
