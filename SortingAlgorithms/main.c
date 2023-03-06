// gcc main.c $(sdl2-config --cflags --libs) -lm -o Build/main && ./Build/main

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "../pixel.c"

// for time() and clock()
#include <time.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const int frameRate = 60;
const int frameDelay = 1000 / frameRate;

int drawNumbers(SDL_Renderer *renderer, int *numbers, int length, int width, int _j){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int i = 0; i < length; i++){
        int x = i * width;
        if(_j == i || _j + 1 == i){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            drawRectangle(renderer, x, WINDOW_HEIGHT - numbers[i], width, numbers[i]);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        else{
            drawRectangle(renderer, x, WINDOW_HEIGHT - numbers[i], width, numbers[i]);
        }
    }

    SDL_RenderPresent(renderer);
    
    return 0;
}

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

void swap(int *x, int *y){
    int temp = *(x);
    *(x) = *(y);
    *(y) = temp;
}

Uint32 bubbleSort(Uint32 interval, void *param){
    SDL_Renderer *renderer = *((SDL_Renderer**) param);
    int *numbers = *((int**) (param + sizeof(SDL_Renderer*)));
    int width = *((int*) (param + sizeof(SDL_Renderer*) + sizeof(int*)));
    int *i = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int) + sizeof(int*) * 1));
    int *j = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int) + sizeof(int*) * 2));

    int length = WINDOW_WIDTH / width;

    drawNumbers(renderer, numbers, length, width, *(j));

    // TODO: add animation when its done
    if(*(i) < length){
        int n1 = numbers[*(j)];
        int n2 = numbers[*(j) + 1];
        if(n2 < n1){
            swap(&numbers[*(j)], &numbers[*(j) + 1]);
        }
    }

    if(*(i) < length - 1 && *(j) == length - *(i) - 1){
        *(i) += 1;
    }

    if(*(i) < length){
        if(*(j) < length - *(i) - 1){
            *(j) += 1;
        }
        else{
            *(j) = 0;
        }
    }


    // printf("R: %p\n", renderer);
    
    return interval;
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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    const unsigned int width = 10;
    int length = WINDOW_WIDTH / width;
    float increase = (float)WINDOW_HEIGHT / (float)length;
    printf("Lengtht: %d increase: %f\n", length, increase);
    int *numbers = malloc(sizeof(int) * length);

    fill(numbers, length, increase);
    shuffle(numbers, length);

    int i = 0;
    int j = 0;
    
    size_t paramSize = sizeof(SDL_Renderer*) + sizeof(int*) + sizeof(int) + sizeof(int*) * 2;
    printf("Param size: %zu\n", paramSize);

    void *param = malloc(paramSize);
    *((SDL_Renderer**) (param)) = renderer;
    *((int**) (param + sizeof(SDL_Renderer*))) = numbers;
    *((int*)  (param + sizeof(SDL_Renderer*) + sizeof(int*))) = width;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int) + sizeof(int*) * 1)) = &i;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int) + sizeof(int*) * 2)) = &j;

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
            else if(event.type == SDL_QUIT){
                run = 0;
            }
        }
    }

    printf("Timer\n");
    SDL_RemoveTimer(timer);

    printf("Param\n");
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
