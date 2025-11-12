#include "cchip8.h"
#include <inttypes.h>
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

// Prints out chip8 info for debugging
void debug(Chip8 *chip8, uint64_t cyclecount) {
  if (chip8->cycles > cyclecount && cyclecount != 0) {
    printf("Cycle count exhausted\n");
    exit(1);
  }

  // format:
  // [<frame%256>:<cycle%65536>] <pc>:<opcode> V=[<registers>] I=<index> SP=<stackpointer>
  printf("[%04" PRIu64 ":%04" PRIu64 "] %04x:%04x V=[%02x %02x %02x %02x  %02x %02x %02x %02x  %02x %02x %02x %02x  %02x %02x %02x %02x] I=%04x SP=%x",
  chip8->frames, chip8->cycles, chip8->PC, chip8->ins.opcode, chip8->V[0], chip8->V[1], chip8->V[2], chip8->V[3], chip8->V[4], chip8->V[5], chip8->V[6], chip8->V[7], chip8->V[8], chip8->V[9], chip8->V[0xA], chip8->V[0xB], chip8->V[0xC], chip8->V[0xD], chip8->V[0xE], chip8->V[0xF], chip8->I, chip8->SP);

  printf("\n");
}
