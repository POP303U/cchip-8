#include "cchip8.h"
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#ifndef AUDIO_H_
#define AUDIO_H_

typedef struct Chip8Audio {
  SDL_AudioDeviceID device;
  bool initialized;
  double phase;
  double frequency;
  double sampleRate;
  double amplitude;
} Chip8Audio;

// Callback to feed audio data into device
void audioCallback(void *userdata, uint8_t *stream, int len);

// Create AudioDevice to use
Chip8Audio* Chip8InitAudio();

// Play and Pause Audio
void Chip8PlayAudio(Chip8Audio *chip8Audio);
void Chip8PauseAudio(Chip8Audio *chip8Audio);

#endif
