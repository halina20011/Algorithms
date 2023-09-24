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

#include "../pixel.h"

#include "../gradient.h"
#include "../rgba.h"

// for time() and clock()
#include <time.h>
#include <math.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 100

void generate(struct Pixel *p){
    struct RGBA colors[4] = {black, red, green, white};
    int positions[4] = {0, 200, 400, WINDOW_WIDTH};

    struct RGBA **gradient = NULL;
    makeGradient(WINDOW_WIDTH, &gradient, colors, positions, 4);
    
    for(int y = 0; y < WINDOW_HEIGHT; y++){
        for(int x = 0; x < WINDOW_WIDTH; x++){
            int i = 4 * (y * WINDOW_WIDTH + x);

            p->buffer[i + 0] = gradient[x]->r;
            p->buffer[i + 1] = gradient[x]->g;
            p->buffer[i + 2] = gradient[x]->b;
            p->buffer[i + 3] = gradient[x]->a;
        }
    }

    clearGradient(&gradient, WINDOW_WIDTH);
}

void mainLoop(struct Pixel *p){
    clock_t start = clock(), diff;
    generate(p);
    
    pixelUpdate(p);

    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec / 1000, msec % 1000);
    
    fflush(stdout);
}

int main(){
    struct Pixel *p = pixelInit("gradient", WINDOW_WIDTH, WINDOW_HEIGHT);

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
