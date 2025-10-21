#include "cchip8.h"
#include <stdint.h>

// This sadly also has to be here
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

struct Chip8 Chip8CreateCpu() {
  uint8_t mem[4096] = {0};
  uint8_t vx[16] = {0};
  uint8_t stack[16] = {0};
  uint8_t fb[64 * 32] = {0};

  struct Chip8 ret = {mem[4096], vx[16], stack[64], fb[64 * 32], 0, 0, 0, 0, 0};
  return ret;
}

void Chip8MapRom(Chip8 chip8, uint8_t *rom, ulong size) {
  for (int i = 0x200; i < size; i++) {
    chip8.memory[i] = rom[i - 0x200];
  }
}
