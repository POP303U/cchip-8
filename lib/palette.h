#include <stdint.h>

#ifndef PALETTE_H_
#define PALETTE_H_

// RGBA Palette data
typedef struct Palette {
    uint8_t bg[4];
    uint8_t fg[4];
} Palette;

typedef struct PaletteSet {
    size_t count;
    Palette palettes[];
} PaletteSet;

// Actual color definitions
extern const uint8_t backgroundRGBA[];
extern const uint8_t foregroundRGBA[];

// Amount of palettes
extern const size_t palettesCount;

// Returns a pointer to a palette set
PaletteSet *createPaletteSet();

#endif
