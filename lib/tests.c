#include "cchip8.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define FB_WIDTH 64
#define FB_HEIGHT 32

void TEST_framebuffer_smile(Chip8 *chip8) {
  chip8->framebuffer[64 * 16 + 28] = true;
  chip8->framebuffer[64 * 16 + 32] = true;
  chip8->framebuffer[64 * 19 + 27] = true;
  chip8->framebuffer[64 * 20 + 28] = true;
  chip8->framebuffer[64 * 20 + 29] = true;
  chip8->framebuffer[64 * 20 + 30] = true;
  chip8->framebuffer[64 * 20 + 31] = true;
  chip8->framebuffer[64 * 20 + 32] = true;
  chip8->framebuffer[64 * 19 + 33] = true;

  // If fb looping works: Framebuffer index 2047 => x=63, y=31
  printf("Framebuffer index 2047 => x=%d, y=%d\n", 2047 % FB_WIDTH,
         2047 / FB_WIDTH);
}

void romToBin(uint8_t *buf, int bytes_to_read) {
  for (int i = 0; i < bytes_to_read; i++) {
    uint8_t byte = buf[i];
    //        printf("hex: %02X, dec: %03d, bin:", byte, byte);

    for (int bit = 0; bit < 8; bit++) {
      //            printf("%i", byte & 0X01);
      byte >>= 1;
    }
    //       printf("\n");
  }
}

void debug(Chip8 *chip8) {
  printf("0x%04X - %04X", chip8->PC, chip8->ins.opcode);
//  printf("cycle: %lld, Current instruction %04X, PC: %X, SP: %X I: %X\n", chip8->cycles, chip8->ins.opcode, chip8->PC, chip8->SP,
//         chip8->I);
  for (int i = 0; i < 16; i++) {
    printf("   V%X: %02X | ", i, chip8->V[i]);
  }
  printf("\n");
}
