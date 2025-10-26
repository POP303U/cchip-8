#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define FB_WIDTH 64
#define FB_HEIGHT 32

void TEST_framebuffer_smile(uint8_t framebuffer[]) {
  framebuffer[64 * 16 + 28] = true;
  framebuffer[64 * 16 + 32] = true;

  framebuffer[64 * 19 + 27] = true;
  framebuffer[64 * 20 + 28] = true;
  framebuffer[64 * 20 + 29] = true;
  framebuffer[64 * 20 + 30] = true;
  framebuffer[64 * 20 + 31] = true;
  framebuffer[64 * 20 + 32] = true;
  framebuffer[64 * 19 + 33] = true;

  // If fb looping works: Framebuffer index 2047 => x=63, y=31
  printf("Framebuffer index 2047 => x=%d, y=%d\n", 2047 % FB_WIDTH,
         2047 / FB_WIDTH);
}

// void fbSetPixel(uint8_t *fb[], int x, int y, bool pixel) { fb[x * y] = pixel;
// }

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
