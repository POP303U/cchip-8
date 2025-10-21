#include <stdbool.h>
#include <stdint.h>

#ifndef TESTS_H_
#define TESTS_H_

void TEST_framebuffer_cross(uint8_t framebuffer[]);
void TEST_framebuffer_smile(uint8_t framebuffer[]);
void fbSetPixel(uint8_t fb[], int x, int y, bool pixel);

#endif
