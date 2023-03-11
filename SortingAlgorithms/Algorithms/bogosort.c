#include "../draw.c"

extern int shuffle(int*, int);

void *bogosortInit(SDL_Renderer *renderer, int *numbers, int width){
    int *i = malloc(sizeof(int));
    int *sorted = malloc(sizeof(int));
    
    *(i) = 0;
    *(sorted) = 0;

    size_t paramSize = sizeof(SDL_Renderer*) + sizeof(int*) * 3 + sizeof(int);
    printf("Param size: %zu\n", paramSize);

    void *param = malloc(paramSize);
    *((SDL_Renderer**) (param)) = renderer;
    *((int**) (param + sizeof(SDL_Renderer*))) = numbers;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1)) = i;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2)) = sorted;
    *((int*)  (param + sizeof(SDL_Renderer*) + sizeof(int*) * 3)) = width;

    return param;
}

int bogosortFree(void *param){
    int *i      = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1));
    int *sorted = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2));
    
    free(i);
    free(sorted);

    return 0;
}

Uint32 bogosort(Uint32 interval, void *param){
    SDL_Renderer *renderer = *((SDL_Renderer**) param);
    int *numbers    = *((int**) (param + sizeof(SDL_Renderer*)));
    int *i          = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1));
    int *sorted     = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2));
    int width       = *((int*)  (param + sizeof(SDL_Renderer*) + sizeof(int*) * 3));

    int length = WINDOW_WIDTH / width;

    // If array isn't yet sorted
    if(ended == 0){
        drawNumbers(renderer, numbers, length, width, *(i), *(i) + 1);
        
        if(*(i) == 0){
            shuffle(numbers, length);
        }

        if(*(i) < length - 1){
            if(numbers[*(i)] < numbers[*(i) + 1]){
                *(i) += 1;
            }
            else{
                *(sorted) = 0;
                *(i) = 0;
            }   
        }
        
        if(length - 1 <= *(i)){
            if(*(sorted) == 1){
                ended = 1;
            }

            *(i) = 0;
        }
    }
    else{
        if(indexAnimation++ < length){
            drawFinalAnimation(renderer, numbers, length, width);
        }
    }
    
    return interval;
}
