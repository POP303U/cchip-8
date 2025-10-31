#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include "keyboard.h"

/* Keypad layout:
1 2 3 4
q w e r
a s d f
y x c v 
*/

static const SDL_Scancode keyMap[KEY_COUNT] = {
  SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
  SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
  SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
  SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V};

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
    if (sc == keyMap[i]) {
      kbd->keys[i] = pressed;
      break;
    }
  }
}