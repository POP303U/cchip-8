#include "lib/cchip8.h"
#include "lib/cchip8ins.h"
#include "lib/font.h"
#include "lib/keyboard.h"
#include "lib/tests.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdint.h>

#define ROM_START 0x200
#define SCALE 20
#define MS_PER_FRAME 10
#define INS_PER_FRAME 16

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Event event;

int main(int argc, char *argv[]) {
  // Load game from first arg
  const char *game = argv[1];

  if (!game || argc < 1) {
    fprintf(stderr, "Usage: ./chip8 <ROM>");
    exit(2);
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return 3;
  }

  // Set Window size based on size of pixels
  if (SDL_CreateWindowAndRenderer(FB_WIDTH * SCALE, FB_HEIGHT * SCALE, false, &window, &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
    return 3;
  }

  // Set Window Titles
  SDL_SetWindowTitle(window, "Chip-8 Emulator in C and SDL2");

  // CHIP-8 STARTUP

  // Read rom file and get size
  uint8_t *rom = Chip8ReadRom(game);
  uint16_t sz = Chip8RomSize(game);

  // Create a cpu initiating values + font
  Chip8 *chip8 = Chip8InitCpu();

  // Map Rom into memory space
  Chip8LoadRom(chip8, rom, sz);

  // Initialize keyboard input
  initKeyboard(&(chip8->kbd));

  // Start cpu emulation with deltatime
  int64_t start, delta = 0;
  while (chip8->running) {
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
      chip8->running = false;
    }

    // Start counting ticks for frame timing
    start = SDL_GetTicks64();

    // Wait until enough frames have passed to execute the next instruction
    for (int i = 0; i < INS_PER_FRAME; i++) {
      // Take in keyboard input and read into an array
      readKey(&(chip8->kbd), &event);

      // FDE Execution cycle
      Chip8FetchInstruction(chip8);
      Chip8DecodeInstruction(chip8);
      Chip8ExecuteInstruction(chip8);

      // Approxmiation of draw instruction, draw is very expensive so exit
      if ((chip8->ins.opcode & 0xF000) == 0xD000) {
        break;        
      }
    }

    // Update timers, delays
    Chip8UpdateState(chip8);

    // Calculate deltatime to keep running at constant speed
    delta = (MS_PER_FRAME - (SDL_GetTicks64() - start));
    if (delta > 0) {
      SDL_Delay(delta);
    }
    delta = 0;

    // Begin draw
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Draw framebuffer
    for (int y = 0; y < FB_HEIGHT; y++) {
      for (int x = 0; x < FB_WIDTH; x++) {
        if (chip8->framebuffer[y * FB_WIDTH + x]) {
          SDL_Rect rect = {x * SCALE, y * SCALE, SCALE, SCALE};
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
          SDL_RenderFillRect(renderer, &rect);
        }
      }
    }

    // End draw
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
