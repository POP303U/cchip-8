#include <stdint.h>
#include <stdlib.h>

#ifndef CCHIP8_H_
#define CCHIP8_H_

// Structs have to defined here to know the size of them
typedef struct Chip8 {
  // Memory space
  uint8_t memory[4096];

  // 16x8 Bytes V0-VF
  uint8_t V[16];

  // 64 Byte stack
  uint8_t stack[64];

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
struct Chip8 Chip8CreateCpu();

// File reading and creating a rom buffer
uint8_t *Chip8ReadRom(const char *filename);
ulong Chip8RomSize(const char *filename);

// Rom and memory mapping
void Chip8MapRom(Chip8 *chip8, uint8_t *rom, ulong size);
void Chip8MapFont(Chip8 *chip8, const uint8_t *font);
void Chip8DumpMem(Chip8 *chip8);

// CPU execution cycle: FDE (Fetch, Decode, Execute)
void Chip8FetchInstruction(Chip8 *chip8);
void Chip8DecodeInstruction(Chip8 *chip8);
void Chip8ExecuteInstruction(Chip8 *chip8);

#endif
