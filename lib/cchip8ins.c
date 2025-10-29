#include "cchip8ins.h"
#include "cchip8.h"
#include <inttypes.h>
#include <stdint.h>
#include <string.h>

// The array with all the functions.
const Chip8Instruction chip8insTable[] = {
    NULL,       Chip8_1NNN, Chip8_2NNN, Chip8_3XKK, Chip8_4XKK, Chip8_5XY0,
    Chip8_6XKK, Chip8_7XKK, NULL,       Chip8_9XY0, Chip8_ANNN, Chip8_BNNN,
    Chip8_CXKK, Chip8_DXYN, NULL,       Chip8_8XY0, Chip8_8XY1, Chip8_8XY2,
    Chip8_8XY3, Chip8_8XY4, Chip8_8XY5, Chip8_8XY6, Chip8_8XY7, Chip8_8XYE,
    Chip8_00E0, Chip8_00EE, Chip8_EX9E, Chip8_EXA1, Chip8_FX07, Chip8_FX0A,
    Chip8_FX15, Chip8_FX18, Chip8_FX1E, Chip8_FX29, Chip8_FX33, Chip8_FX55,
    Chip8_FX65};

// This instruction is ignored by modern interpreters
void Chip8_0NNN(Chip8 *chip8) { return; }

// 0x00E0/CLS: Clear the display
void Chip8_00E0(Chip8 *chip8) {
  // Empty buffer
  memset(chip8->framebuffer, 0, sizeof(chip8->framebuffer));
}

// 0x00EE/RET: Return from a subroutine
void Chip8_00EE(Chip8 *chip8) {
  // Pop last address from the stack and use it as PC
  uint8_t addrstk = chip8->stack[chip8->SP];
  chip8->SP -= 1;
  chip8->PC = addrstk;
}

// 0x1NNN/JP addr: Jump to location NNN
void Chip8_1NNN(Chip8 *chip8) {
  // Set program counter to NNN
  chip8->PC = chip8->ins.nnn;
}

// 0x2NNN/CALL addr: Call subroutine at NNN
void Chip8_2NNN(Chip8 *chip8) {
  // Push PC onto the stack, then put NNN into PC
  chip8->SP += 1;
  chip8->stack[chip8->SP] = chip8->PC;
  chip8->PC = chip8->ins.nnn;
}

// 0x3XKK/SE Vx, byte: Skip next instruction if Vx == kk
void Chip8_3XKK(Chip8 *chip8) {
  // Increments PC by 2 if Vx == kk
  if (chip8->V[chip8->ins.x] == chip8->V[chip8->ins.kk]) {
    chip8->PC += 2;
  }
}

// 0x4XKK/SNE Vx, byte: Skip next instruction if Vx != kk
void Chip8_4XKK(Chip8 *chip8) {
  // Increments PC by 2 if Vx != kk
  if (chip8->V[chip8->ins.x] != chip8->V[chip8->ins.kk]) {
    chip8->PC += 2;
  }
}

// 0x5XY0/SE Vx, Vy: Skip next instruction if Vx = Vy
void Chip8_5XY0(Chip8 *chip8) {
  // Increments PC by 2 if Vx == Vy
  if (chip8->V[chip8->ins.x] == chip8->V[chip8->ins.y]) {
    chip8->PC += 2;
  }
}

// 0x6XKK/LD Vx, Byte: Set Vx = kk
void Chip8_6XKK(Chip8 *chip8) {
  // Put the value of kk into reg Vx
  chip8->V[chip8->ins.x] = chip8->ins.kk;
}

// 0x7XKK/ADD Vx, Byte: Set Vx += kk, set VF = carry
void Chip8_7XKK(Chip8 *chip8) {
  // Add kk to Vx with overflow flag
  if (((uint16_t)(chip8->V[chip8->ins.x] + chip8->ins.kk)) > 255) {
    chip8->V[0xF] = 0x1;
    chip8->V[chip8->ins.x] = 0xFF;
  }

  chip8->V[chip8->ins.x] += chip8->ins.kk;
}

// 0x8XY0/LD Vx, Vy: Set Vx = Vy
void Chip8_8XY0(Chip8 *chip8) {
  // Store the value of Vy in Vx
  chip8->V[chip8->ins.x] = chip8->V[chip8->ins.y];
}

// 0x8XY1/OR Vx, Vy: Set Vx = Vx OR Vy
void Chip8_8XY1(Chip8 *chip8) {
  // OR Vx and Vy and store the result in Vx
  chip8->V[chip8->ins.x] |= chip8->V[chip8->ins.y];
}

// 0x8XY2/AND Vx, Vy: Set Vx = Vx AND Vy
void Chip8_8XY2(Chip8 *chip8) {
  // AND Vx and Vy and store the result in Vx
  chip8->V[chip8->ins.x] &= chip8->V[chip8->ins.y];
}

// 0x8XY3/XOR Vx, Vy: Set Vx = Vx XOR Vy
void Chip8_8XY3(Chip8 *chip8) {
  // XOR Vx and Vy and store the result in Vx
  chip8->V[chip8->ins.x] ^= chip8->V[chip8->ins.y];
}

// 0x8XY4/ADD Vx, Vy: Set Vx += Vy, set VF = carry
void Chip8_8XY4(Chip8 *chip8) {
  // Add Vy to Vx with overflow flag
  if (((uint16_t)(chip8->V[chip8->ins.x] + chip8->ins.y)) > 255) {
    chip8->V[0xF] = 0x1;
  } else {
    chip8->V[0xF] = 0x0;
  }

  chip8->V[chip8->ins.x] += chip8->V[chip8->ins.y];
}

// 0x8XY5/SUB Vx, Vy: Set Vx = Vx - Vy, set VF = NOT borrow
void Chip8_8XY5(Chip8 *chip8) {
  // Sub to Vx with Vy with overflow flag
  if (chip8->V[chip8->ins.x] > chip8->V[chip8->ins.y]) {
    chip8->V[0xF] = 0x1;
  } else {
    chip8->V[0xF] = 0x0;
  }

  chip8->V[chip8->ins.x] -= chip8->V[chip8->ins.y];
}

// 0x8XY6/SHR Vx {, Vy}: Set Vx = Vx SHR 1
void Chip8_8XY6(Chip8 *chip8) {
  // If the most significant bit of Vx is 1, then set VF to 1 otherwise 0
  if (GETBIT(chip8->V[chip8->ins.x], 0) == 1) {
    chip8->V[0xF] = 0x1;
  } else {
    chip8->V[0xF] = 0x0;
  }

  chip8->V[chip8->ins.x] >>= 1;
}

// 0x8XY7/SUBN Vx, Vy: Set Vx = Vy - Vx, set VF = NOT borrow
void Chip8_8XY7(Chip8 *chip8) {
  // Sub to Vx with Vy - Vx with overflow flag
  if (chip8->V[chip8->ins.y] > chip8->V[chip8->ins.x]) {
    chip8->V[0xF] = 0x1;
  } else {
    chip8->V[0xF] = 0x0;
  }

  chip8->V[chip8->ins.x] = chip8->V[chip8->ins.y] - chip8->V[chip8->ins.x];
}

// 0x8XYE/SHL Vx {, Vy}: Set Vx = Vx SHL 1
void Chip8_8XYE(Chip8 *chip8) {
  // If the most significant bit of Vx is 1, then set VF to 1 otherwise 0
  if (GETBIT(chip8->V[chip8->ins.x], 7) == 1) {
    chip8->V[0xF] = 0x1;
  } else {
    chip8->V[0xF] = 0x0;
  }

  chip8->V[chip8->ins.x] <<= 1;
}

// 0x9XY0/SNE Vx, Vy: Skip next instruction if Vx != Vy
void Chip8_9XY0(Chip8 *chip8) {
  // Increase PC by 2 if Vx != Vy
  if (chip8->V[chip8->ins.x] != chip8->V[chip8->ins.y]) {
    chip8->PC += 2;
  }
}

// 0xANNN/LD I, addr: The value of register I is set to NNN.
void Chip8_ANNN(Chip8 *chip8) {
  // Sets I to the extracted right three bits
  chip8->I = chip8->ins.nnn;
}

// 0xBNNN/JP V0, addr: The Program counter is set to NNN plus the value of V0
void Chip8_BNNN(Chip8 *chip8) {
  uint16_t jmpaddr = chip8->ins.nnn + chip8->V[0];
  chip8->PC = jmpaddr;
}

void Chip8_CXKK(Chip8 *chip8) {}

// 0xDXYN/DRW Vx, Vy, nibble: Display n-byte starting at memory location I at
// (Vx, Vy)
void Chip8_DXYN(Chip8 *chip8) {
  uint8_t Vx = chip8->V[chip8->ins.x];
  uint8_t Vy = chip8->V[chip8->ins.y];

  for (uint8_t n = 0x0; n < chip8->ins.n; n++) {
    uint8_t Ibyte = chip8->memory[chip8->I + n];

    for (uint8_t i = 0; i < 8; i++) {
      uint16_t xy = (Vx + i) + (64 * Vy);

      // If a sprite collides with a pixel set VF = 1
      if (chip8->framebuffer[xy]) {
        chip8->V[0xF] = 0x1;
      }

      // Set each bit into the framebuffer (in big endian)
      chip8->framebuffer[xy] ^= GETBIT(SWAPBYTE(Ibyte), i);
    }
    Vy += 1;
  }
}

void Chip8_EX9E(Chip8 *chip8) {}
void Chip8_EXA1(Chip8 *chip8) {}
void Chip8_FX07(Chip8 *chip8) {}
void Chip8_FX0A(Chip8 *chip8) {}
void Chip8_FX15(Chip8 *chip8) {}
void Chip8_FX18(Chip8 *chip8) {}
void Chip8_FX1E(Chip8 *chip8) {}
void Chip8_FX29(Chip8 *chip8) {}
void Chip8_FX33(Chip8 *chip8) {}
void Chip8_FX55(Chip8 *chip8) {}
void Chip8_FX65(Chip8 *chip8) {}
