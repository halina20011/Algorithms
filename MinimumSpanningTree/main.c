// ./build.sh && ./Build/main

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

// TODO: boruvka's algorithm 
// TODO: prim's algorithm

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "../pixel.c"
#include "../pngWrapper.c"

// for time() and clock()
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

const int WINDOWWIDTH = WINDOW_WIDTH;
const int WINDOWHEIGHT = WINDOW_HEIGHT;

uint8_t *buffer = NULL;
SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;

#define frameDelay ((Uint32)1000 / (Uint32)10)

void wait(){
    SDL_Delay(frameDelay);
}

bool processEvents(){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_KEYDOWN){
            SDL_Keycode key = event.key.keysym.sym;
            if(key == SDLK_q || key == SDLK_ESCAPE){
                return false;
            }
        }
        else if(event.type == SDL_QUIT){
            return false;
        }
    }

    return true;
}

#include "./graph.h"

#include "./kruskal.c"

struct Vertex **randomVertices(int maxX, int maxY, size_t size){
    unsigned seed = time(NULL); // time(0);
    srand(seed);

    struct Vertex **vertices = malloc(sizeof(struct Vertex*) * size);
    for(size_t i = 0; i < size; i++){
        vertices[i] = vertexInit(rand() % maxX, rand() % maxY);
    }

    return vertices;
}

void freeVertices(struct Vertex **vertices, size_t size){
    for(size_t i = 0; i < size; i++){
        free(vertices[i]);
    }

    free(vertices);
}

int main(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    window = SDL_CreateWindow("minimum spanning tree", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(newPixelBuffer(&buffer)){
        printf("Error allocating memory for pixel buffer\n");
        return 1;
    }
    
    const size_t verticesSize = 40;
    struct Vertex **vertices = randomVertices(WINDOW_WIDTH, WINDOW_HEIGHT, verticesSize);
    
    kruskalsAlgorithm(buffer, vertices, verticesSize);
    
    freeVertices(vertices, verticesSize);
        
    free(buffer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
