#include "rom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BINARY_READ "rb"
#define BYTE unsigned char

ulong Chip8RomSize(const char *filename) {
  FILE *fptr = fopen(filename, BINARY_READ);

  // Get the file size
  fseek(fptr, 0L, SEEK_END);
  long sz = ftell(fptr);
  fseek(fptr, 0L, SEEK_SET);
  rewind(fptr);

  fclose(fptr);

  return sz;
}

BYTE *Chip8ReadRom(const char *filename) {
  FILE *fptr = fopen(filename, BINARY_READ);

  if (fptr == NULL) {
    exit(-1);
  }

  u_int16_t sz = Chip8RomSize(filename);

  // Read into buf until eof
  BYTE *buf = malloc(sz);
  fread(buf, sz, 1, fptr);
  fclose(fptr);

  // Loop and read every byte into the program rom
  BYTE *ret = malloc(sz);
  if (!ret) {
    return NULL;
  }

  //  for (u_int16_t i = 0; i < sz; i++) {
  //    ret[i] = buf[i];
  //    printf("%02X", buf[i]);
  //  }

  free(buf);
  return ret;
}
