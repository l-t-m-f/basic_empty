#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "SDL_FontCache.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define WIDTH 1280
#define HEIGHT 720
#define TARGET_FPS 60
#define TARGET_RATE (1.0f / (float)TARGET_FPS)
#define ASPECT_RATIO ((float)WIDTH / (float)HEIGHT)

extern void generate_random_number(int32_t *);
