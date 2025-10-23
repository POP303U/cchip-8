#include "lib/cchip8.h"
#include "lib/rom.h"
#include "lib/tests.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define BYTE unsigned char
#define FB_WIDTH 64
#define FB_HEIGHT 32
#define SCALE 20

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Event event;

// This sadly has to be here so c loads it first
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
  Chip8 cpu = Chip8CreateCpu();

  // Map Rom to memory space
  Chip8MapRom(cpu, rom, sz);

  uint8_t framebuffer[64 * 32] = {0};
  TEST_framebuffer_smile(framebuffer);

  //  for (int i = 0x200; i < 0x400; i++) {
  //    printf("cpu adr: %X %s\n", i, &cpu.memory[i]);
  //  }

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
