// gcc main.c $(sdl2-config --cflags --libs) -lm -lpng -o Build/main && ./Build/main [indexAnimation]

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

#include <stdio.h>
#include <stdbool.h>
// for usleep()
#include <unistd.h>
// for PRIu32 macro
#include <inttypes.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

// #define CAPTURE_ON true
#include "../pixel.c"
#include "../pngWrapper.c"
#include "func.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const int WINDOWWIDTH = WINDOW_WIDTH;
const int WINDOWHEIGHT = WINDOW_HEIGHT;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

uint8_t *buffer = NULL;
int *numbers = NULL;

#define numbersWidth 10
#define numbersSize (WINDOW_WIDTH / numbersWidth)
const float increase = (float)WINDOW_HEIGHT / (float)numbersSize;

int capture = 0;
// int capture = 2;
uint16_t captureIndex = 0;

bool ended = 0;
int indexAnimation = 0;

struct Algorithm{
    const char *name;
    const void *algorithm;
};

const float frameRateTable[8] = {1, 2, 5, 10, 50, 100, 250, 500};
const short int frameRateMaxIndex = sizeof(frameRateTable) / sizeof(frameRateTable[0]);
unsigned short frameRateIndex = 5;

bool process(){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_KEYDOWN){
            SDL_Keycode key = event.key.keysym.sym;
            if(key == SDLK_q || key == SDLK_ESCAPE){
                return false;
            }
        }
        else if(event.type == SDL_KEYUP){
            SDL_Keycode key = event.key.keysym.sym;
            
            if(key == SDLK_RETURN){
                if(frameRateIndex != 6){
                    frameRateIndex = 6;
                }
            }
            else if(key == SDLK_s || key == SDLK_DOWN){
                if(0 < frameRateIndex){
                    frameRateIndex--;
                }
            }
            else if(key == SDLK_w || key == SDLK_UP){
                if(frameRateIndex < frameRateMaxIndex - 1){
                    frameRateIndex++;
                }
            }
        }
        else if(event.type == SDL_QUIT){
            return false;
        }
    }

    return true;
}

#define frameRate  ((float)frameRateTable[frameRateIndex])
#define frameDelay ((Uint32)1000 / (Uint32)frameRate)

void wait(){
    SDL_Delay(frameDelay);
}

#define SHOW \
    do{ \
        bool result = (*process)(); \
        if(result == 0){ \
            SDL_DestroyRenderer(renderer); \
            SDL_DestroyWindow(window); \
            SDL_Quit(); \
            exit(0); \
        } \
    } while(0)

#define WAIT \
    do{ \
        wait(); \
    } while(0)

// include all Sorting Algorithms 
#include "Algorithms/bubbleSort.h"
#include "Algorithms/selectionSort.h"
#include "Algorithms/bogosort.h"
#include "Algorithms/insertionSort.h"
#include "Algorithms/gnomeSort.h"
#include "Algorithms/oddevenSort.h"
#include "Algorithms/stoogeSort.h"
#include "Algorithms/radixSort.h"
#include "Algorithms/cocktailSort.h"
#include "Algorithms/heapsort.h"

struct Algorithm algorithms[] = {
    BUBBLESORT, INSERTIONSORT, GNOMESORT, ODDEVENSORT, STOOGESORT, COCKTAILSORT, BOGOSORT, SELECTIONSORT, RADIXSORT, HEAPSORT
};

size_t sortingAlgorithmsLength = sizeof(algorithms) / sizeof(algorithms[0]);

void showOptions(){
    printf("Please select correct algorithm to run\n");
    printf("Index\t\tAlgorithm Name\n");
    printf("   -1\t\t[Run the last algorithm]\n");

    for(int i = 0; i < sortingAlgorithmsLength; i++){
        printf("%5d\t\t%s\n", i, algorithms[i].name);
    }
}

int main(int argc, char **argv){
    char *flagsArg = NULL;
    char *indexArg = NULL;

    int runAlgorithmIndex = -1;
    if(argc == 1){
        showOptions();
        return 1;
    }
    indexArg = (argc == 3) ? argv[2] : argv[1];
    flagsArg = (argc == 3) ? argv[1] : NULL;
    
    // parse the user input
    // runAlgorithmIndex = atoi(argv[1]);
    int s = sscanf(indexArg, "%i", &runAlgorithmIndex);

    // if input is not a number
    if(s == 0){
        runAlgorithmIndex = -1;
    }
    else if(runAlgorithmIndex == -1){
        runAlgorithmIndex = sortingAlgorithmsLength - 1;
    }
    else if(runAlgorithmIndex < -1 || sortingAlgorithmsLength <= runAlgorithmIndex){
        printf("Valid range is <0; %li)\n", sortingAlgorithmsLength);
        runAlgorithmIndex = 0;
    }

    if(flagsArg != NULL){
        if(memcmp(flagsArg, "-c", 2) == 0){
            capture = 2;
            printf("Capture is turned on\n");
        }
    }

    printf("Algorithm: \"%s\"\n", algorithms[runAlgorithmIndex].name);
    
    // initialize SDL2
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    // create a window and renderer
    window = SDL_CreateWindow("Sorting Algorithms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    int r = newPixelBuffer(&buffer);
    if(r){
        printf("Error allocating memory for pixel buffer\n");
        return 1;
    }

    numbers = malloc(sizeof(int) * numbersSize);
    if(numbers == NULL){
        printf("Error allocating memory for numbers buffer\n");
        return 1;
    }

    fillArray(numbers, numbersSize, increase);
    shuffle(numbers, numbersSize);

    const void (*algorithm)(uint8_t **buffer, int *numbers) = algorithms[runAlgorithmIndex].algorithm;

    // int *numbersCopy = malloc(sizeof(int) * numbersSize);
    // memcpy(numbersCopy, numbers, sizeof(int) * numbersSize);
    // algorithm(NULL, numbersCopy);
    // printArray(numbersCopy, );
    // free(numbersCopy);
    
    algorithm(&buffer, numbers);

    drawFinalAnimation(&buffer, numbers, numbersWidth, numbersSize);

    // wait for user to exit
    while(process()){}

    printf("Renderer and window\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
