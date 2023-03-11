#include "../draw.c"

void *gnomeSortInit(SDL_Renderer *renderer, int *numbers, int width){
    int *i = malloc(sizeof(int));
    
    *(i) = 0;

    size_t paramSize = sizeof(SDL_Renderer*) + sizeof(int*) * 2 + sizeof(int);
    printf("Param size: %zu\n", paramSize);

    void *param = malloc(paramSize);
    *((SDL_Renderer**) (param)) = renderer;
    *((int**) (param + sizeof(SDL_Renderer*))) = numbers;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1)) = i;
    *((int*)  (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2)) = width;

    return param;
}

int gnomeSortFree(void *param){
    int *i  = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1));
    
    free(i);

    return 0;
}

// Example
// int *numbersCopy = malloc(sizeof(int) * length);
// memcpy(numbersCopy, numbers, sizeof(int) * length);
// gnomeSortAlg(numbersCopy, length);
// printArray(numbersCopy, length);
int gnomeSortAlg(int *numbers, int length){
    int i = 0;

    while(i < length){
        if(i == -1){
            i += 1;
        }

        // correct order => one number forward
        if(numbers[i] < numbers[i + 1]){
            i += 1;
        }
        else{ // swap them and go one step backwards
            swap(&numbers[i], &numbers[i + 1]);
            i -= 1;
        }
        
    }

    return 0;
}

Uint32 gnomeSort(Uint32 interval, void *param){
    SDL_Renderer *renderer = *((SDL_Renderer**) param);
    int *numbers    = *((int**) (param + sizeof(SDL_Renderer*)));
    int *i          = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1));
    int width       = *((int*)  (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2));

    int length = WINDOW_WIDTH / width;

    // If array isn't yet sorted
    if(*(i) < length){
        drawNumbers(renderer, numbers, length, width, *(i), *(i) + 1);
        
        if(*(i) == -1){
            *(i) += 1;
        }

        if(numbers[*(i)] < numbers[*(i) + 1]){
            *(i) += 1;
        }
        else{
            swap(&numbers[*(i)], &numbers[*(i) + 1]);
            *(i) -= 1;
        }
    }
    else{
        if(indexAnimation++ < length){
            drawFinalAnimation(renderer, numbers, length, width);
        }
    }
    
    return interval;
}
