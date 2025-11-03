#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

// Amount of keys
#define KEY_COUNT 16

typedef struct Keyboard {
  bool keys[KEY_COUNT];
  bool prevKeys[KEY_COUNT];
} Keyboard;

// Process input
void initKeyboard(Keyboard *kbd);
void readKey(Keyboard *kbd, const SDL_Event *event);

#endif
