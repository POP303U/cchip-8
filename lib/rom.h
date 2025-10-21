#include <sys/types.h>

#ifndef ROM_H_
#define ROM_H_

unsigned char *Chip8ReadRom(const char *filename);
ulong Chip8RomSize(const char *filename);

#endif
