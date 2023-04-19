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

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

// #define CAPTURE_ON true
#include "../pixel.c"
#include "../pngWrapper.c"

// For time() and clock()
#include <time.h>
// For usleep()
#include <unistd.h>
// For PRIu32 macro
#include <inttypes.h>
#include <math.h>

// Include all Sorting Algorithms 
#include "Algorithms/bubbleSort.c"
#include "Algorithms/selectionSort.c"
#include "Algorithms/bogosort.c"
#include "Algorithms/insertionSort.c"
#include "Algorithms/gnomeSort.c"
#include "Algorithms/oddevenSort.c"
#include "Algorithms/stoogeSort.c"
#include "Algorithms/radixSort.c"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const int WINDOWWIDTH = WINDOW_WIDTH;
const int WINDOWHEIGHT = WINDOW_HEIGHT;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

int capture = 0;
uint16_t captureIndex = 0;

const float frameRateTable[8] = {1, 2, 5, 10, 50, 100, 250, 500};
const short int frameRateMaxIndex = sizeof(frameRateTable) / sizeof(frameRateTable[0]);
unsigned short frameRateIndex = 5;

#define frameRate  ((float)frameRateTable[frameRateIndex])
#define frameDelay ((Uint32)1000 / (Uint32)frameRate)

int ended = 0;
int indexAnimation = 0;

typedef struct Algorithm{
    const char *name;
    const void *algorithm;
    const void *testAlgorithm;
    const void *init;
    const void *free;
} Algorithm;

Algorithm algorithms[] = {BUBBLESORT, SELECTIONSORT, BOGOSORT, INSERTIONSORT, GNOMESORT, ODDEVENSORT, STOOGESORT, RADIXSORT};

size_t sortingAlgorithmsLength = sizeof(algorithms) / sizeof(algorithms[0]);

void fillArray(int *array, int length, float increase){
    for(int i = 0; i < length; i++){
        *(array + i) = (i + 1) * increase;
    }
}

void shuffle(int *array, int length){
    unsigned seed = time(NULL);
    srand(seed);

    for(int i = 0; i < length; i++){
        int newIndex = rand() % length;
        if(i != newIndex){
            int temp = array[newIndex];
            // printf("Temp %d\n", temp);
            array[newIndex] = array[i];
            array[i] = temp;
        }
    }
}

void printArray(int *array, int length){
    for(int i = 0; i < length; i++){
        printf("%d ", array[i]);
    }

    printf("\n");
}

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

    short runAlgorithmIndex = -1;
    if(argc == 1){
        showOptions();
        return 1;
    }
    indexArg = (argc == 3) ? argv[2] : argv[1];
    flagsArg = (argc == 3) ? argv[1] : NULL;
    
    // Convert
    // runAlgorithmIndex = atoi(argv[1]);
    int s = sscanf(indexArg, "%d", &runAlgorithmIndex, sizeof(runAlgorithmIndex));

    // Input is not a number
    if(s == 0){
        runAlgorithmIndex = -1;
    }
    else if(runAlgorithmIndex == -1){
        runAlgorithmIndex = sortingAlgorithmsLength - 1;
    }
    else if(runAlgorithmIndex < -1 || sortingAlgorithmsLength <= runAlgorithmIndex){
        printf("Valid range is <0; %i)\n", sortingAlgorithmsLength);
        runAlgorithmIndex = 0;
    }

    if(flagsArg != NULL){
        if(memcmp(flagsArg, "-c", 2) == 0){
            capture = 2;
            printf("Capture is turned on\n");
        }
    }

    printf("Algorithm: \"%s\"\n", algorithms[runAlgorithmIndex].name);
    
    // Initialize SDL2
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    // Create a window and renderer
    window = SDL_CreateWindow("Sorting Algorithms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    uint8_t *buffer = NULL;
    int r = newPixelBuffer(&buffer);
    if(r){
        printf("Error allocating memory for pixel buffer\n");
        return 1;
    }

    const unsigned int width = 10;
    int length = WINDOW_WIDTH / width;
    float increase = (float)WINDOW_HEIGHT / (float)length;
    printf("Lengtht: %d increase: %f\n", length, increase);
    int *numbers = malloc(sizeof(int) * length);
    
    fillArray(numbers, length, increase);
    shuffle(numbers, length);

    void *(*initFunction)(uint8_t**, int*, int) = algorithms[runAlgorithmIndex].init;
    void *(*freeFunction)(void *param) = algorithms[runAlgorithmIndex].free;
    void *param = (*initFunction)(&buffer, numbers, width);
    const void *func = algorithms[runAlgorithmIndex].algorithm;
    
    // Schedule the first frame
    SDL_TimerID timer = SDL_AddTimer(frameDelay, func, param);
    
    int run = 1;
    while(run){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                SDL_Keycode key = event.key.keysym.sym;
                if(key == SDLK_q || key == SDLK_ESCAPE){
                    run = 0;
                }
            }
            else if(event.type == SDL_KEYUP){
                SDL_Keycode key = event.key.keysym.sym;
                
                int update = 0;
                if(key == SDLK_RETURN){
                    if(frameRateIndex != 6){
                        frameRateIndex = 6;
                        update = 1;
                    }
                }
                else if(key == SDLK_s || key == SDLK_DOWN){
                    if(0 < frameRateIndex){
                        frameRateIndex--;
                        update = 1;
                    }
                }
                else if(key == SDLK_w || key == SDLK_UP){
                    if(frameRateIndex < frameRateMaxIndex - 1){
                        frameRateIndex++;
                        update = 1;
                    }
                }

                if(update == 1){
                    SDL_RemoveTimer(timer);
                    timer = SDL_AddTimer(frameDelay, func, param);

                    // printf("Frame rate: %.2f\n", frameRate);
                    // printf("Frame delay: %"PRIu32"\n", frameDelay);
                }
            }
            else if(event.type == SDL_QUIT){
                run = 0;
            }
        }
    }

    printf("Timer\n");
    SDL_RemoveTimer(timer);
    // Wait for timer to execute last time
    usleep(frameDelay * 1000 * 2);

    printf("Renderer and window\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    printf("Free\n");
    free(numbers);
    (*freeFunction)(param);

    SDL_Quit();

    return 0;
}
