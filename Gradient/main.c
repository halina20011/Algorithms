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

#include "../pixel.c"
#include "../pngWrapper.c"

#include "../gradient.h"
#include "../rgba.h"

// for time() and clock()
#include <time.h>
#include <math.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 100

const int WINDOWWIDTH = WINDOW_WIDTH;
const int WINDOWHEIGHT = WINDOW_HEIGHT;

SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;

int generate(uint8_t **buffer){
    struct RGBA colors[4] = {black, red, green, white};
    int positions[4] = {0, 200, 400, WINDOW_WIDTH};

    struct RGBA **gradient = NULL;
    makeGradient(WINDOW_WIDTH, &gradient, colors, positions, 4);
    
    for(int y = 0; y < WINDOW_HEIGHT; y++){
        for(int x = 0; x < WINDOW_WIDTH; x++){
            int i = 4 * (y * WINDOW_WIDTH + x);

            *(*buffer + i + 0) = gradient[x]->r;
            *(*buffer + i + 1) = gradient[x]->g;
            *(*buffer + i + 2) = gradient[x]->b;
            *(*buffer + i + 3) = gradient[x]->a;
        }
    }
    clearGradient(&gradient, WINDOW_WIDTH);

    return 0;
}

int mainLoop(uint8_t **buffer){
    clock_t start = clock(), diff;
    generate(buffer);

    update(*buffer);

    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec / 1000, msec % 1000);
    
    fflush(stdout);

    return 0;
}

int main(){
    // Initialize SDL2
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    // Create a window and renderer
    window = SDL_CreateWindow("Template", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    uint8_t *buffer = NULL;
    int r = newPixelBuffer(&buffer);
    if(r){
        printf("Error allocating memory for pixel buffer\n");
        return 1;
    }

    mainLoop(&buffer);

    int run = 1;
    while(run){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                SDL_Keycode key = event.key.keysym.sym;
                if(key == SDLK_q || key == SDLK_ESCAPE){
                    run = 0;
                }
                else if(key == SDLK_RETURN){
                    mainLoop(&buffer);
                }
                else if(key == SDLK_s){
                    savePng("gradient.png", buffer, WINDOW_WIDTH, WINDOW_HEIGHT);
                    printf("Image saved\n");
                }
            }
            else if(event.type == SDL_QUIT){
                run = 0;
            }
        }
    }

    free(buffer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
