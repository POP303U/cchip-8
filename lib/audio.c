#include "audio.h"

// Callback which generates a sound based on data->frequency
void audioCallback(void *userdata, uint8_t *stream, int len) {
  Chip8Audio *data = (Chip8Audio* )userdata;
  int16_t *buffer = (int16_t *)stream;
  int samples = len / sizeof(uint16_t);

  int period = data->sampleRate / data->frequency;

  for (int i = 0; i < samples; i++) {
    buffer[i] = (data->phase < period / 2 ? 3000 : -3000);
    data->phase++;
    if (data->phase >= period) {
      data->phase = 0;
    }
  }
}

// Create a new Audio object which uses callbacks to play audio
Chip8Audio* Chip8InitAudio() {
  // Init audio subsystem
  if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
    printf("SDL_InitSubSystem error: %s\n", SDL_GetError());
    exit(1);
  }

  Chip8Audio *chip8Audio = malloc(sizeof(Chip8Audio));

  if (chip8Audio == NULL) {
    fprintf(stderr, "Chip8InitAudio: could not alloc");
  }

  memset(chip8Audio, 0, sizeof(Chip8Audio));

  chip8Audio->phase = 0.0;
  chip8Audio->frequency = 329.63;      // E4 Note
  chip8Audio->sampleRate = 48000.0;    // 48khz playback
  chip8Audio->amplitude = 16000.0;     // 50% Volume

  SDL_AudioSpec desired = {0};
  desired.freq = (int)chip8Audio->sampleRate; // Samples per second
  desired.format = AUDIO_S16SYS;              // Audio data format
  desired.channels = 1;                       // Number of channels (mono)
  desired.samples = 512;                      // Audio buffer size in sample FRAMES
  desired.callback = audioCallback;           // Callback that feeds the audio device
  desired.userdata = chip8Audio;              // Userdata passed to callback

  // Open audio device
  SDL_AudioSpec obtained;
  chip8Audio->device = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, 0);
  if (!chip8Audio->device) {
      printf("SDL_OpenAudioDevice error: %s\n", SDL_GetError());
      exit(1);
  }

  // Start paused
  chip8Audio->initialized = true;
  SDL_PauseAudioDevice(chip8Audio->device, 1);

  return chip8Audio;
}

void Chip8PlayAudio(Chip8Audio *chip8Audio)  {
  if (chip8Audio->initialized) {
    SDL_PauseAudioDevice(chip8Audio->device, 0);
  }
}

void Chip8PauseAudio(Chip8Audio *chip8Audio) {
  if (chip8Audio->initialized) {
    SDL_PauseAudioDevice(chip8Audio->device, 1);
  }
}

// Cleanup after exiting
void Chip8DestroyAudio(Chip8Audio *chip8Audio) {
  if (chip8Audio->initialized) {
    SDL_CloseAudioDevice(chip8Audio->device);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    chip8Audio->initialized = false;
  }
}