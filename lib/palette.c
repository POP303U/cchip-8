#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "palette.h"

const uint8_t backgroundRGBA[] = {
  // Monochrome palette
  0x00,
  0x00,
  0x00,
  0xFF,

  // Monochrome palette (inverted)
  0xFF,
  0xFF,
  0xFF,
  0xFF,

  // Retro yellow
  0x99,
  0x66,
  0x01,
  0xFF,

  // Green palette
  0x00,
  0x00,
  0x00,
  0xFF,
};


const uint8_t foregroundRGBA[] = {
  // Monochrome palette
  0xFF,
  0xFF,
  0xFF,
  0xFF,

  // Monochrome palette (inverted)
  0x00,
  0x00,
  0x00,
  0xFF,

  // Retro yellow
  0xFF,
  0xCC,
  0x01,
  0xFF,

  // Green palette
  0x00,
  0xFF,
  0x00,
  0xFF,
};

PaletteSet *createPaletteSet() {
  // Used for looping through colors
  const size_t palettesCount = sizeof(backgroundRGBA) / 4;
  // Defines how much memory to allocate
  PaletteSet *paletteSet = malloc(sizeof(PaletteSet) + palettesCount * sizeof(Palette));

  if (paletteSet == NULL) {
    fprintf(stderr, "createPaletteSet: could not alloc");
  }

  paletteSet->count = palettesCount;
  
  // Copies every palette into the set based on size
  for (size_t i = 0; i < palettesCount; i++) {
    memcpy(paletteSet->palettes[i].bg, &backgroundRGBA[i * 4], 4);
    memcpy(paletteSet->palettes[i].fg, &foregroundRGBA[i * 4], 4);
  }

  return paletteSet;
}