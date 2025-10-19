#include "rom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BINARY_READ "rb"
#define BYTE unsigned char

BYTE *Chip8ReadRom(const char *filename) {
  FILE *fptr = fopen(filename, BINARY_READ);

  if (fptr == NULL) {
    exit(-1);
  }

  // Get the file size
  fseek(fptr, 0L, SEEK_END);
  long sz = ftell(fptr);
  fseek(fptr, 0L, SEEK_SET);
  rewind(fptr);
  printf("fptr length: %0lu\n", sz);

  // Read into buf until eof
  BYTE *buf = malloc(sz);
  fread(buf, sz, 1, fptr);
  fclose(fptr);

  // Loop and read every byte into the program rom
  BYTE *ret = malloc(sz);
  if (!ret) {
    return NULL;
  }

  for (int i = 0; i < sz; i++) {
    ret[i] = buf[i];
    printf("%02X", buf[i]);
  }

  free(buf);
  return ret;
}
