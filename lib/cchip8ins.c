#include "cchip8ins.h"
#include "cchip8.h"
#include <string.h>

// The array with all the functions.
const Chip8Instruction chip8insTable[] = {
    Chip8_1NNN, Chip8_2NNN, Chip8_3XKK, Chip8_4XKK, Chip8_5XY0, Chip8_6XKK,
    Chip8_7XKK, Chip8_ANNN, Chip8_BNNN, Chip8_CXKK, Chip8_DXYN};

// This instruction is ignored by modern interpreters
void Chip8_0NNN(Chip8 *chip8) { return; };

// 0x00E0: Clear the display
void Chip8_00E0(Chip8 *chip8) {
  // Empty buffer
  memset(chip8->framebuffer, 0, sizeof(chip8->framebuffer));
};

void Chip8_00EE(Chip8 *chip8) {};
void Chip8_1NNN(Chip8 *chip8) {};
void Chip8_2NNN(Chip8 *chip8) {};
void Chip8_3XKK(Chip8 *chip8) {};
void Chip8_4XKK(Chip8 *chip8) {};
void Chip8_5XY0(Chip8 *chip8) {};
void Chip8_6XKK(Chip8 *chip8) {};
void Chip8_7XKK(Chip8 *chip8) {};
void Chip8_8XY0(Chip8 *chip8) {};
void Chip8_8XY1(Chip8 *chip8) {};
void Chip8_8XY2(Chip8 *chip8) {};
void Chip8_8XY3(Chip8 *chip8) {};
void Chip8_8XY4(Chip8 *chip8) {};
void Chip8_8XY5(Chip8 *chip8) {};
void Chip8_8XY6(Chip8 *chip8) {};
void Chip8_8XY7(Chip8 *chip8) {};
void Chip8_8XYE(Chip8 *chip8) {};
void Chip8_9XY0(Chip8 *chip8) {};
void Chip8_ANNN(Chip8 *chip8) {};
void Chip8_BNNN(Chip8 *chip8) {};
void Chip8_CXKK(Chip8 *chip8) {};
void Chip8_DXYN(Chip8 *chip8) {};
void Chip8_EX9E(Chip8 *chip8) {};
void Chip8_EXA1(Chip8 *chip8) {};
void Chip8_FX07(Chip8 *chip8) {};
void Chip8_FX0A(Chip8 *chip8) {};
void Chip8_FX15(Chip8 *chip8) {};
void Chip8_FX18(Chip8 *chip8) {};
void Chip8_FX1E(Chip8 *chip8) {};
void Chip8_FX29(Chip8 *chip8) {};
void Chip8_FX33(Chip8 *chip8) {};
void Chip8_FX55(Chip8 *chip8) {};
void Chip8_FX65(Chip8 *chip8) {};
