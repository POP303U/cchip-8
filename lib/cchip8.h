#include "keyboard.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef CCHIP8_H_
#define CCHIP8_H_

// Gets a bit from a byte at an index
#define GETBIT(byte, x) (((byte) >> (7 - (x))) & 1)

// Turns the input into a big-endian number
#define SWAPBYTE(i) ((i & 0xF0) >> 4) | ((i & 0x0F) << 4)

// Memory mapping
#define ROM_START 0x200
#define FONT_START 0x50
#define FONTSET_SIZE 80

// File reading
#define BINARY_READ "rb"

// Framebuffer variables
#define FB_WIDTH 64
#define FB_HEIGHT 32

typedef struct Instruction {
  uint16_t opcode;
  uint16_t lbit;
  uint16_t nnn;
  uint8_t n;
  uint8_t x;
  uint8_t y;
  uint16_t kk;
} Instruction;

// Structs have to defined here to know the size of them
typedef struct Chip8 {
  // Track if we have to stop execution
  bool running;

  // Current executing opcode
  Instruction ins;

  // Input device
  Keyboard kbd;

  // Track cycle count for debugging
  uint64_t cycles;

  // Memory space
  uint8_t memory[4096];

  // 16x8 Bytes V0-VF
  uint8_t V[16];

  // 64 Byte stack
  uint16_t stack[64];

  // Framebuffer
  uint8_t framebuffer[64 * 32];

  // Registers
  uint16_t I;
  uint16_t PC;
  uint8_t SP;
  uint8_t soundTimer;
  uint8_t delayReg;

} Chip8;

// Set up the Chip-8 struct and memory space for usage
Chip8 *Chip8InitCpu();

// File reading and creating a rom buffer
uint8_t *Chip8ReadRom(const char *filename);
long Chip8RomSize(const char *filename);

// Rom and memory mapping
void Chip8LoadRom(Chip8 *chip8, uint8_t *rom, uint16_t size);
void Chip8DumpMem(Chip8 *chip8);

// CPU execution cycle: FDE (Fetch, Decode, Execute)
// Fetch: Returns the current executing hex bytes like: 0xA2B4
// Decode: Destructs hex bytes (from fetch) into instruction calls and variables
void Chip8FetchInstruction(Chip8 *chip8);
void Chip8DecodeInstruction(Chip8 *chip8);
void Chip8ExecuteInstruction(Chip8 *chip8);
void Chip8UpdateState(Chip8 *chip8, uint64_t delta);

// Helper function for Chip8ExecuteInstruction, gets function out of funcptr
// table based on opcode
uint8_t Chip8GetOpcodeIndex(uint16_t opcode);

#endif
