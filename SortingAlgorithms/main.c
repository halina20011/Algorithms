// gcc main.c $(sdl2-config --cflags --libs) -lm -o Build/main && ./Build/main

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "../pixel.c"

// for time() and clock()
#include <time.h>
// For PRIu32 macro
#include <inttypes.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const float frameRateTable[] = {1, 2, 5, 10, 50, 100, 250, 500};
const short int frameRateMaxIndex = sizeof(frameRateTable) / sizeof(frameRateTable[0]);
unsigned short frameRateIndex = 5;

#define frameRate  ((float)frameRateTable[frameRateIndex])
#define frameDelay ((Uint32)1000 / (Uint32)frameRate)

int ended = 0;
int finalAnimation = 0;
int indexAnimation = 0;

// Include all Sorting Algorithms 
#include "bubbleSort.c"

int fill(int *array, int length, float increase){
    printf("Length: %d\n", length);
    
    for(int i = 0; i < length; i++){
        *(array + i) = (i + 1) * increase;
    }

    return 0;
}

int shuffle(int *array, int length){
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

    return 0;
}

int main(int argc, char **argv){
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    
    // Initialize SDL2
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    // Create a window and renderer
    window = SDL_CreateWindow("Sorting Algorithms", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    const unsigned int width = 10;
    int length = WINDOW_WIDTH / width;
    float increase = (float)WINDOW_HEIGHT / (float)length;
    printf("Lengtht: %d increase: %f\n", length, increase);
    int *numbers = malloc(sizeof(int) * length);

    fill(numbers, length, increase);
    shuffle(numbers, length);
    
    void *param = bubbleSortInit(renderer, numbers, width);
    void *func = bubbleSort;

    // Schedule the first frame
    SDL_TimerID timer = SDL_AddTimer(frameDelay, bubbleSort, param);
    
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

    printf("Param\n");
    bubbleSortFree(param);
    free(numbers);
    free(param);

    printf("Other\n");
    // TODO: fix this
    // This will sometime make "Segmentation fault (core dumped)" but only if there is drawNumbers in bubbleSort
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
