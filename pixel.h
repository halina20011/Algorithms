// Copyright (C) 2023  halina20011
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef PIXEL
#define PIXEL

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define PIXEL_CONTINUE 0
#define PIXEL_EXIT 1
#define PIXEL_RUN 2

#ifdef CAPTURE_ON
    #include "pngWrapper.c"
    #include <sys/types.h>
    #include <sys/stat.h>
    extern int capture;
    extern uint16_t captureIndex;
#endif

struct Pixel{
    uint8_t *buffer;
    size_t width, height, size;
    uint8_t *rgba;
    SDL_Texture *texture;

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    
    struct timespec sleepDuration;
    size_t sleepIndex;

    bool run;
};

#define ever ;;

struct Pixel *pixelInit(const char *windowName, size_t width, size_t height);

void pixelSetIndexSpeed(struct Pixel *p, size_t index);

void pixelSetSpeed(struct Pixel *p, bool speed);
int waitFor(struct Pixel *p, SDL_Keycode code);
int pixelEvents(struct Pixel *p);

void pixelWait(struct Pixel *p);

void pixelWaitExit(struct Pixel *p);

void pixelFree(struct Pixel *p);
void pixelSetColor(struct Pixel *p, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void pixelUpdate(struct Pixel *p);

#define PixelWait() {\
    pixelUpdate(p); \
    if(pixelEvents(p) == PIXEL_EXIT){\
        exit(EXIT_SUCCESS); \
    }\
    pixelWait(p); \
} \

void pixelClear(struct Pixel *p);
void pixelFill(struct Pixel *p);

void pixelDraw(struct Pixel *p, int x, int y);
void drawLine(struct Pixel *p, int x0, int y0, int x1, int y1);
double calculateResolution(int radius);
void drawCircle(struct Pixel *p, int x, int y, int radius);
void drawRectangle(struct Pixel *p, int x, int y, int width, int height);

#endif
