#include "lib/rom.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

const uint8_t FB_WIDTH = 64;
const uint8_t FB_HEIGHT = 32;
const uint8_t SCALE = 20;

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Event event;

typedef struct Chip8 {
  // Memory space
  uint8_t memory[4096];

  // 16x8 Bytes V0-VF
  uint8_t V[16];

  // 64 Byte stack
  uint8_t stack[64];

  // Framebuffer
  uint8_t framebuffer[64 * 32];

  // Registers
  uint16_t I;
  uint16_t PC;
  uint8_t SP;
  uint8_t soundTimer;
  uint8_t delayReg;

} Chip8;

void TEST_framebuffer_cross(uint8_t framebuffer[]) {
  // Clear framebuffer
  memset(framebuffer, 0, FB_WIDTH * FB_HEIGHT);
}

void TEST_framebuffer_smile(uint8_t framebuffer[]) {
  framebuffer[64 * 16 + 28] = true;
  framebuffer[64 * 16 + 32] = true;

  framebuffer[64 * 19 + 27] = true;
  framebuffer[64 * 20 + 28] = true;
  framebuffer[64 * 20 + 29] = true;
  framebuffer[64 * 20 + 30] = true;
  framebuffer[64 * 20 + 31] = true;
  framebuffer[64 * 20 + 32] = true;
  framebuffer[64 * 19 + 33] = true;

  // If fb looping works: Framebuffer index 2047 => x=63, y=31
  printf("Framebuffer index 2047 => x=%d, y=%d\n", 2047 % FB_WIDTH,
         2047 / FB_WIDTH);
}

void fbSetPixel(uint8_t fb[], int x, int y, bool pixel) { fb[x * y] = pixel; }

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

  // TESTS
  uint8_t framebuffer[64 * 32] = {0};
  TEST_framebuffer_smile(framebuffer);
  // TEST_framebuffer_cross(framebuffer);

  // Read Rom into memory
  Chip8ReadRom("rom.ch8");

  while (1) {
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

    SDL_RenderPresent(renderer);
    SDL_Delay(16); // ~60 FPS
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
