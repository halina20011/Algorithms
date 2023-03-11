#include "../draw.c"

void printArray(int *, int);

void *insertionSortInit(SDL_Renderer *renderer, int *numbers, int width){
    int *i = malloc(sizeof(int));
    int *j = malloc(sizeof(int));
    
    *(i) = 1;
    *(j) = *(i) - 1;

    size_t paramSize = sizeof(SDL_Renderer*) + sizeof(int*) * 3 + sizeof(int);
    printf("Param size: %zu\n", paramSize);

    void *param = malloc(paramSize);
    *((SDL_Renderer**) (param)) = renderer;
    *((int**) (param + sizeof(SDL_Renderer*))) = numbers;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1)) = i;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2)) = j;
    *((int*)  (param + sizeof(SDL_Renderer*) + sizeof(int*) * 3)) = width;

    return param;
}

int insertionSortFree(void *param){
    int *i  = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1));
    int *j  = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2));
    
    free(i);
    free(j);

    return 0;
}

// Example
// int *numbersCopy = malloc(sizeof(int) * length);
// memcpy(numbersCopy, numbers, sizeof(int) * length);
// insertionSortAlg(numbersCopy, length);
// printArray(numbersCopy, length);
int insertionSortAlg(int *numbers, int length){
    for(int i = 1; i < length; i++){
        for(int j = i - 1; j >= 0; j--){
            if(numbers[j + 1] < numbers[j]){
                swap(&numbers[j], &numbers[j + 1]);
            }
            else{
                break;
            }
        }
    }

    return 0;
}

Uint32 insertionSort(Uint32 interval, void *param){
    SDL_Renderer *renderer = *((SDL_Renderer**) param);
    int *numbers    = *((int**) (param + sizeof(SDL_Renderer*)));
    int *i          = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1));
    int *j          = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2));
    int width       = *((int*)  (param + sizeof(SDL_Renderer*) + sizeof(int*) * 3));

    int length = WINDOW_WIDTH / width;

    // If array isn't yet sorted
    if(*(i) < length){
        drawNumbers(renderer, numbers, length, width, *(j), *(j) + 1);
        
        if(*(j) < 0){
            *(i) += 1;
            *(j) = *(i) - 1;
        }
        else{
            if(numbers[*(j) + 1] < numbers[*(j)]){
                swap(&numbers[*(j)], &numbers[*(j) + 1]);
                *(j) -= 1;
            }
            else{ // Why to check numbers before when they should be sorted
                *(i) += 1;
                *(j) = *(i) - 1;
            }
        }

    }
    else{
        if(indexAnimation++ < length){
            drawFinalAnimation(renderer, numbers, length, width);
        }
    }
    
    return interval;
}
