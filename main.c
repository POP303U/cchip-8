#include "lib/cchip8.h"
#include "lib/tests.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#define ROM_START 0x200
#define BYTE unsigned char
#define FB_WIDTH 64
#define FB_HEIGHT 32
#define SCALE 20

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Event event;

int main(void) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
                 SDL_GetError());
    return 3;
  }

  // Not getting this right caused so many bugs oml
  if (SDL_CreateWindowAndRenderer(FB_WIDTH * SCALE, FB_HEIGHT * SCALE, false,
                                  &window, &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't create window and renderer: %s", SDL_GetError());
    return 3;
  }

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  // Set render color to green ( rect will be rendered in this color )
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  // STARTUP

  // Read rom file and get size
  uint8_t *rom = Chip8ReadRom("rom.ch8");
  ulong sz = Chip8RomSize("rom.ch8");

  // Create a cpu
  struct Chip8 chip8 = Chip8CreateCpu();

  // Map Rom to memory space
  Chip8MapRom(&chip8, rom, sz);

  uint8_t framebuffer[64 * 32] = {0};
    for (ulong i = 0; i < sz; i++) {
      framebuffer[i] = rom[i];
    }

  while (true) {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
      break;
    }

    // Begin draw
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw framebuffer
    for (int y = 0; y < FB_HEIGHT; y++) {
      for (int x = 0; x < FB_WIDTH; x++) {
        if (framebuffer[y * FB_WIDTH + x]) {
          SDL_Rect rect = {x * SCALE, y * SCALE, SCALE, SCALE};
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
          SDL_RenderFillRect(renderer, &rect);
        }
      }
    } // End draw

    // Load ROM into 0x200 (Start of Chip-8 Programs)

    SDL_RenderPresent(renderer);
    SDL_Delay(16); // ~60 FPS
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
