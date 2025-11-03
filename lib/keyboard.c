#include "keyboard.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdint.h>

/* Keypad layout:

  kbd        chip8
1 2 3 4     1 2 3 C
Q W E R --> 4 5 6 D
A S D F     7 8 9 E
Y X C V     A 0 B F

*/
static const SDL_Scancode scanMap[KEY_COUNT] = {
    SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
    SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
    SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V};

// Maps the scancode to the chip8 layout
static const uint8_t keyMap[KEY_COUNT] = {0x0, 0x1, 0x2, 0xC, 0x3, 0x4,
                                          0x5, 0xD, 0x6, 0x7, 0x8, 0xE,
                                          0xA, 0x0, 0xB, 0xF};

// Initialize keyboard for usage
void initKeyboard(Keyboard *kbd) {
  for (int i = 0; i < KEY_COUNT; i++) {
    kbd->keys[i] = false;
  }
}

// Read keydown state into array of KEYS
void readKey(Keyboard *kbd, const SDL_Event *event) {
  if (event->type != SDL_KEYDOWN && event->type != SDL_KEYUP) {
    return;
  }

  bool pressed = (event->type == SDL_KEYDOWN);
  SDL_Scancode sc = event->key.keysym.scancode;

  for (int i = 0; i < KEY_COUNT; i++) {
    if (sc == scanMap[i]) {
      kbd->keys[keyMap[i]] = pressed;
      break;
    }
  }
}
