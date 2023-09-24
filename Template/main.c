// gcc main.c $(sdl2-config --cflags --libs) -lm -lpng -o Build/main && ./Build/main

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

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "../pixel.h"
#include "../pngWrapper.c"

// for time() and clock()
#include <time.h>
#include <math.h>

void mainLoop(struct Pixel *p){
    clock_t start = clock(), diff;

    // generate
    unsigned seed = time(NULL);
    srand(seed);

    for(int y = 0; y < p->height; y++){
        for(int x = 0; x < p->width; x++){
            int i = 4 * (y * p->width + x);
            p->buffer[i + 0] = rand() % 255 + 1;
            p->buffer[i + 1] = rand() % 255 + 1;
            p->buffer[i + 2] = rand() % 255 + 1;
            p->buffer[i + 3] = 255;
        }
    }

    pixelUpdate(p);

    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec / 1000, msec % 1000);
    
    fflush(stdout);
}

int main(){
    struct Pixel *p = pixelInit("Template", 600, 600);

    mainLoop(p);

    while(1){
        int r = pixelEvents(p);
        if(r == PIXEL_EXIT){
            break;
        }
        else if(r == PIXEL_RUN){
            mainLoop(p);
        }
    }

    pixelFree(p);

    return 0;
}
