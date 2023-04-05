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

#include <stdlib.h>
#include <time.h>

#include "../pixel.c"
#include "../pngWrapper.c"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

const int WINDOWWIDTH = WINDOW_WIDTH;
const int WINDOWHEIGHT = WINDOW_HEIGHT;

SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;

void drawPoints(uint8_t **buffer, int **field, int columns, int rows, int resolution){
    for(int i = 0; i < columns; i++){
        for(int j = 0; j < rows; j++){
            if(1 <= field[i][j]){
                drawPixel(buffer, j * resolution, i * resolution, 255, 0, 0, 255);
            }
        }
    }
}

int getState(int a, int b, int c, int d){
    return d * 8 + c * 4 + b * 2 + a * 1;
}

void drawIsolines(uint8_t **buffer, int **field, int m, int n, int resolution){
    for(int i = 0; i < m - 1; i++){
        for(int j = 0; j < n - 1; j++){
            int x = j * resolution;
            int y = i * resolution;

            int a[2] = {x + resolution / 2, y                 };
            int b[2] = {x + resolution    , y + resolution / 2};
            int c[2] = {x + resolution / 2, y + resolution    };
            int d[2] = {x                 , y + resolution / 2};

            // int state = getState(arr);
            int A = field[i][j];
            int B = field[i][j + 1];
            int C = field[i + 1][j + 1]; 
            int D = field[i + 1][j];
            
            int state = getState(D, C, B, A);

            switch(state){
                case 0:
                    break;
                case 1:
                    drawLine(buffer, c[0], c[1], d[0], d[1]);
                    break;
                case 2:
                    drawLine(buffer, b[0], b[1], c[0], c[1]);
                    break;
                case 3:
                    drawLine(buffer, b[0], b[1], d[0], d[1]);
                    break;
                case 4:
                    drawLine(buffer, a[0], a[1], b[0], b[1]);
                    break;
                case 5:
                    drawLine(buffer, a[0], a[1], d[0], d[1]);
                    drawLine(buffer, b[0], b[1], c[0], c[1]);
                    break;
                case 6:
                    drawLine(buffer, a[0], a[1], c[0], c[1]);
                    break;
                case 7:
                    drawLine(buffer, a[0], a[1], d[0], d[1]);
                    break;
                case 8:
                    drawLine(buffer, a[0], a[1], d[0], d[1]);
                    break;
                case 9: 
                    drawLine(buffer, a[0], a[1], c[0], c[1]);
                    break;
                case 10: 
                    drawLine(buffer, a[0], a[1], b[0], b[1]);
                    drawLine(buffer, d[0], d[1], c[0], c[1]);
                    break;
                case 11: 
                    drawLine(buffer, a[0], a[1], b[0], b[1]);
                    break;
                case 12: 
                    drawLine(buffer, b[0], b[1], d[0], d[1]);
                    break;
                case 13: 
                    drawLine(buffer, b[0], b[1], c[0], c[1]);
                    break;
                case 14: 
                    drawLine(buffer, d[0], d[1], c[0], c[1]);
                    break;
                case 15:
                    break;
            }
        }
    }
}

int **makeArray(int columns, int rows){
    int **field = (int**)malloc(sizeof(int *) * columns);
    
    for(int i = 0; i < columns; i++){
        *(field + i) = (int*)malloc(sizeof(int) * rows);
    }

    return field;
}

void freeArray(int **array, int width){
    for(int i = 0; i < width; i++){
        free(array[i]);
    }

    free(array);
}

void marchingSquares(uint8_t **buffer, int **field, int columns, int rows, int resolution){
    unsigned seed = time(NULL); // time(0);
    srand(seed);
    
    // Generate new patern
    for(int y = 0; y < columns; y++){
        for(int x = 0; x < rows; x++){
            // Generate random value from 0 to 1
            field[y][x] = rand() % 2;
        }
    }

    // Debugging
    // for(int i = 0; i < columns; i++){
    //     for(int j = 0; j < rows; j++){
    //         printf("%i,", field[i][j]);
    //     }
    //     printf("\n");
    // }

    setColor(0, 0, 0, 255);
    fill(buffer);

    // Set new color
    setColor(255, 0, 0, 255);
    // drawPoints(buffer, field, columns, rows, resolution);
    drawIsolines(buffer, field, columns, rows, resolution);
    update(*buffer);
}

int main(int argc, char **argv){
    int resolution = 10;
    int columns = 1 + WINDOW_WIDTH / resolution;
    int rows = 1 + WINDOW_HEIGHT / resolution;
    
    int **field = makeArray(columns, rows);

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    // Create a window and renderer
    window = SDL_CreateWindow("Marching Squares", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    uint8_t *buffer = NULL;
    int r = newPixelBuffer(&buffer);
    if(r){
        printf("Error allocating memory for pixel buffer\n");
        return 1;
    }

    marchingSquares(&buffer, field, columns, rows, resolution);

    int run = 1;
    while(run){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                SDL_Keycode key = event.key.keysym.sym;
                if(key == SDLK_q || key == SDLK_ESCAPE){
                    run = 0;
                }
                else if(key == SDLK_RETURN){
                    marchingSquares(&buffer, field, columns, rows, resolution);
                }
                else if(key == SDLK_s){
                    savePng("marchingSquares.png", buffer, WINDOW_WIDTH, WINDOW_HEIGHT);
                    printf("Image saved\n");
                }
            }
            else if(event.type == SDL_QUIT){
                run = 0;
            }
        }
    }
    
    freeArray(field, columns);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
