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

#include <math.h>

#include "../pixel.c"
#include "../pngWrapper.c"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

const int WINDOWWIDTH = WINDOW_WIDTH;
const int WINDOWHEIGHT = WINDOW_HEIGHT;

SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;

float cenX = 0.0;
float cenY = 0.0;
float scale = 1.0;

int hsv2rgb(float H, float S, float V, int *r, int *g, int *b){
    if(H > 360 || H < 0 || S > 100 || S < 0 || V > 100 || V < 0){
        *r = 0;
        *g = 0;
        *b = 0;
        return 1;
    }

    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float _r, _g, _b;

    if(H >= 0 && H < 60){
        _r = C;
        _g = X;
        _b = 0;
    }
    else if(H >= 60 && H < 120){
        _r = X;
        _g = C;
        _b = 0;
    }
    else if(H >= 120 && H < 180){
        _r = 0;
        _g = C;
        _b = X;
    }
    else if(H >= 180 && H < 240){
        _r = 0;
        _g = X;
        _b = C;
    }
    else if(H >= 240 && H < 300){
        _r = X;
        _g = 0;
        _b = C;
    }
    else{
        _r = C;
        _g = 0;
        _b = X;
    }

    *r = (int)(_r + m) * 255;
    *g = (int)(_g + m) * 255;
    *b = (int)(_b + m) * 255;

    return 0;
}

long double distance(long double x1, long double y1, long double x2, long double y2){
    long double x = x2 - x1;
    long double y = y2 - y1;

    return hypot(x, y);
}

void pixelToPoint(int x, int y, long double *px, long double *py, int width, int height){
    *px = (x - width / 2.0) * (4.0 / width) * (1.0 / scale) + cenX;
    *py = (y - height / 2.0) * (4.0 / height) * (1.0 / scale) + cenY;
}

const float bounds = 2.0;
const int maxIterations = 100;

void calculatePoint(long double cx, long double cy, int *iterations, int *isIn){
    long double zx = 0;
    long double zy = 0;

    *iterations = 0;
    *isIn = 1;

    while(*iterations < maxIterations && *isIn == 1){
        long double tzx = zx * zx - zy * zy + cx;
        long double tzy = 2 * zx * zy + cy;
        zx = tzx;
        zy = tzy;
        *iterations += 1;
        long double d = distance(0.0, 0.0, zx, zy);
        if(bounds < d){
            *isIn = 0;
        }
    }
}

void mandelbrotSet(uint8_t **buffer, int width, int height){
    long double px = 0.0; 
    long double py = 0.0;
    

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int i = 4 * (y * width + x);

            int iterations = 0;
            int isIn = 0;
            
            pixelToPoint(x, y, &px, &py, width, height);
            calculatePoint(px, py, &iterations, &isIn);

            int r = 0; 
            int g = 0; 
            int b = 0;

            if(1 < iterations && isIn == 0){
                // float h = 150 + 200 - fmod(pow(iterations/70.0, 0.5) * 200, 255);
                // float s = 100;
                // float v = 100;
                // hsv2rgb(h, s, v, &r, &g, &b);
                r = 255; g = 255; b = 255;
            }
            else if(isIn == 0){
                r = 115; g = 115; b = 115;
            }

            *(*buffer + i + 0) = r;
            *(*buffer + i + 1) = g;
            *(*buffer + i + 2) = b;
            *(*buffer + i + 3) = 255;
        }
    }
}

int main(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    // Create a window and renderer
    window = SDL_CreateWindow("Mandelbrot set", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    uint8_t *buffer = NULL;
    int r = newPixelBuffer(&buffer);
    if(r){
        printf("Error allocating memory for pixel buffer\n");
        return 1;
    }

    mandelbrotSet(&buffer, WINDOW_WIDTH, WINDOW_HEIGHT);
    update(buffer);
    
    int run = 1;
    while(run){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                SDL_Keycode key = event.key.keysym.sym;
                if(key == SDLK_q || key == SDLK_ESCAPE){
                    run = 0;
                }
                else if(key == SDLK_s){
                    savePng("mandelbrotSet.png", buffer, WINDOW_WIDTH, WINDOW_HEIGHT);
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
