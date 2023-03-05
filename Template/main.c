// gcc main.c $(sdl2-config --cflags --libs) -lm -o Build/main && ./Build/main

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "../pixel.c"

// for time() and clock()
#include <time.h>
#include <math.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

int generate(int *buffer){
    unsigned seed = time(NULL); // time(0);
    srand(seed);

    for(int y = 0; y < WINDOW_HEIGHT; y++){
        for(int x = 0; x < WINDOW_HEIGHT; x++){
            int index = 4 * (y * WINDOW_WIDTH + x);
            *(buffer + index + 0) = rand() % 255 + 1;
            *(buffer + index + 1) = rand() % 255 + 1;
            *(buffer + index + 2) = rand() % 255 + 1;
            *(buffer + index + 3) = 255;
        }
    }

    return 0;
}

int mainLoop(SDL_Renderer *renderer, int *buffer){
    clock_t start = clock(), diff;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    printf("Generating...\n");
    generate(buffer);

    printf("Drawing...\n");
    drawFromBuffer(renderer, buffer, WINDOW_WIDTH, WINDOW_HEIGHT);
    printf("Drawn...\n");

    SDL_RenderPresent(renderer);

    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec / 1000, msec % 1000);
    
    fflush(stdout);

    return 0;
}

int main(){
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    
    // Initialize SDL2
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    // Create a window and renderer
    window = SDL_CreateWindow("Template", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    int *buffer = malloc(sizeof(int) * WINDOW_WIDTH * WINDOW_HEIGHT * 4);
    mainLoop(renderer, buffer);

    int run = 1;
    while(run){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                SDL_Keycode key = event.key.keysym.sym;
                if(key == SDLK_q || key == SDLK_ESCAPE){
                    run = 0;
                }
                if(key == SDLK_RETURN){
                    mainLoop(renderer, buffer);
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
