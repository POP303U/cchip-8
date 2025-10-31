#include "cchip8.h"
#include <stdbool.h>
#include <stdint.h>

#ifndef TESTS_H_
#define TESTS_H_

void TEST_framebuffer_smile(Chip8 *chip8);
void romToBin(uint8_t *buf, int bytes_to_read);
void debug(Chip8 *chip8);

#endif
