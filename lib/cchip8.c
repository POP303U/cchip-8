#include "cchip8.h"
#include "cchip8ins.h"
#include "tests.h"
#include "font.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Creates a new Chip8 struct to return a pointer to
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

  chip8->I = 0;              // Index reg
  chip8->PC = 0x200;         // Execution starts at 0x200
  chip8->SP = 0;             // Points to the top of the stack
  chip8->soundTimer = 0;     // Decrements each cycle by 1
  chip8->delayReg = 0;       // Used for games   
  chip8->cycles = 0;         // Track amount of cycles
  chip8->running = 1;        // Track if chip8 is running
  chip8->ins = *ins;         // Current instruction being executed
  chip8->kbd = *kbd;         // Stores keyboard input
  chip8->currentPalette = 0; // Stores currently used palette

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


  // Debug opcodes, setting cycle count to 0 lets it run forever
  debug(chip8, 0);

  // Pre-increment PC
  chip8->PC += 2;
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
}

// Reads the processed opcodes and runs the corresponding function 
// according to Chip8GetOpcodeIndex
void Chip8ExecuteInstruction(Chip8 *chip8) {

  // Increase cycle count
  chip8->cycles++;

  // Custom function that maps opcode to table entry
  uint16_t index = Chip8GetOpcodeIndex(chip8->ins.opcode);

  // Call function (with failsafe)
  if (chip8insTable[index] == NULL) {
      fprintf(stderr, "Access into NULL at chip8insTable[%d]\n", index);
      exit(1);
  }

  // Run opcode
  chip8insTable[index](chip8);
}

// Updates keys, timers and delay registers every frame
void Chip8UpdateState(Chip8 *chip8) {
  // Decrement delayReg for each instruction run
  if (chip8->delayReg > 0) {
    chip8->delayReg -= 1;
  }
  
  // Decrement soundTimer for each instruction run
  if (chip8->soundTimer > 0) {
    chip8->soundTimer -= 1;
  }

  // Load keys from last frame into array for 0xFX0A
  for (int i = 0; i < KEY_COUNT; i++) {
    chip8->kbd.prevKeys[i] = chip8->kbd.keys[i];
  }
  
  // A frame has passed
  chip8->frames++;
}

// Gets the index from the opcode to execute the current function needed
uint8_t Chip8GetOpcodeIndex(uint16_t opcode) {
  uint8_t high = (opcode >> 12) & 0xF;
  uint8_t low = opcode & 0xF;

  // For functions with an unique high bit (Indexes 1-13)
  if ((high >= 0x1 && high <= 0x7) || (high >= 0x9 && high <= 0xD)) {
    return high; 
  }

  switch (high) {
    case 0x0: return (opcode == 0x00E0) ? 24 :              // 00E0, 00E0 (indices 24-25)
                     (opcode == 0x00EE) ? 25 : 0;
    case 0x8: return (opcode & 0xF) == 0xE ? 23 : 15 + low; // 8XY0–8XYE  (indices 15–23)
    case 0xE: return (opcode & 0xFF) == 0x9E ? 26 : 27;     // EX9E, EXA1 (indices 26-27)
    case 0xF:
      switch (opcode & 0xFF) {
        // FX07-FX65 (indices 28-36)
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
  // Undefined instruction which is an access into NULL and will be caught
  return 0;
}

// Protected Read into memory
uint8_t Chip8ReadMem(Chip8 *chip8, uint16_t addr) {
  return (addr > 0xFFF) ? 0 : chip8->memory[addr];
}

// Protected Write into memory
void Chip8WriteMem(Chip8 *chip8, uint16_t addr, uint8_t value) {
  if (addr > 0xFFF) {
    return;
  } else {
    chip8->memory[addr] = value;
  }
}

// Load rom into memory space starting at 0x200
void Chip8LoadRom(Chip8 *chip8, uint8_t *rom, uint16_t size) {
  for (uint16_t i = ROM_START; i < (ROM_START + size); i++) {
    chip8->memory[i] = rom[i - ROM_START];
  }
}

// Gets the size of a file
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

// Dump memory map of the Chip8 into stdout, useful for debugging
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
