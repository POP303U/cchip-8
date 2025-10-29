#include "lib/cchip8.h"
#include "lib/cchip8ins.h"
#include "lib/font.h"
#include "lib/tests.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdint.h>

#define ROM_START 0x200
#define FB_WIDTH 64
#define FB_HEIGHT 32
#define SCALE 20

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Event event;

int main(int argc, char *argv[]) {
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

  // CHIP-8 STARTUP

  // Read rom file and get size
  uint8_t *rom = Chip8ReadRom("rom.ch8");
  uint16_t sz = Chip8RomSize("rom.ch8");

  // Create a cpu initiating values + font
  Chip8 *chip8 = Chip8InitCpu();

  // Map Rom into memory space
  Chip8LoadRom(chip8, rom, sz);

  // Debug
  Chip8DumpMem(chip8);

  int cycles = 0;
  while (true) {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
      break;
    }

    // Begin draw
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    Chip8FetchInstruction(chip8);

    // Draw framebuffer
    for (int y = 0; y < FB_HEIGHT; y++) {
      for (int x = 0; x < FB_WIDTH; x++) {
        if (chip8->framebuffer[y * FB_WIDTH + x]) {
          SDL_Rect rect = {x * SCALE, y * SCALE, SCALE, SCALE};
          SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Green
          SDL_RenderFillRect(renderer, &rect);
        }
      }
    } // End draw

    // FDES Execution cycle
    Chip8FetchInstruction(chip8);
    Chip8DecodeInstruction(chip8);
    Chip8ExecuteInstruction(chip8);

    printf("cycle: %d, Current instruction %04X, I: %X, SP: %X PC: %X\n", cycles, chip8->ins.opcode, chip8->I, chip8->SP,
           chip8->PC);
    for (int i = 0; i < 16; i++) {
      printf("V%X: %02X\n", i, chip8->V[i]);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16); // ~60 FPS
    cycles++;
    if (cycles > 6) {
      break;
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
