#include "../draw.c"

void *selectionSortInit(SDL_Renderer *renderer, int *numbers, int width){
    int *i = malloc(sizeof(int));
    int *j = malloc(sizeof(int));
    int *minIndex = malloc(sizeof(int));
    
    *(i) = 0;
    *(j) = *(i) + 1;
    *(minIndex) = 0;

    size_t paramSize = sizeof(SDL_Renderer*) + sizeof(int*) * 4 + sizeof(int);
    printf("Param size: %zu\n", paramSize);

    void *param = malloc(paramSize);
    *((SDL_Renderer**) (param)) = renderer;
    *((int**) (param + sizeof(SDL_Renderer*))) = numbers;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1)) = i;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2)) = j;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 3)) = minIndex;
    *((int*)  (param + sizeof(SDL_Renderer*) + sizeof(int*) * 4)) = width;

    return param;
}

int *selectionSortFree(void *param){
    int *i          = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1));
    int *j          = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2));
    int *minIndex   = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 3));
    
    free(i);
    free(j);
    free(minIndex);

    return 0;
}

// Example
// int *numbersCopy = malloc(sizeof(int) * length);
// memcpy(numbersCopy, numbers, sizeof(int) * length);
// selectionSortAlg(numbersCopy, length);
// printArray(numbersCopy, length);
int selectionSortAlg(int *numbers, int length){
    int minIndex = 0;

    for(int i = 0; i < length - 1; i++){
        minIndex = i;
        for(int j = i + 1; j < length; j++){
            if(numbers[j] < numbers[minIndex]){
                minIndex = j;
            }
        }

        if(minIndex != i){
            swap(&numbers[minIndex], &numbers[i]);
        }
    }

    return 0;
}

Uint32 selectionSort(Uint32 interval, void *param){
    SDL_Renderer *renderer = *((SDL_Renderer**) param);
    int *numbers    = *((int**) (param + sizeof(SDL_Renderer*)));
    int *i          = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1));
    int *j          = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2));
    int *minIndex   = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 3));
    int width       = *((int*)  (param + sizeof(SDL_Renderer*) + sizeof(int*) * 4));

    int length = WINDOW_WIDTH / width;
   
    // If it is not sorted procede
    if(*(i) < length){
        drawNumbers(renderer, numbers, length, width, *(i), *(j));

        // If there are still some items that werent schecked in second foor lop procede
        if(*(j) < length){
            if(numbers[*(j)] < numbers[*(minIndex)]){
                *(minIndex) = *(j);
            }

            *(j) += 1;
        }

        // Increase i and set j if the second foor lop ended
        if(length <= *(j)){
            // Swap smallest number if
            if(*(minIndex) != *(i)){
                swap(&numbers[*(i)], &numbers[*(minIndex)]);
            }
            
            *(i) += 1;
            *(minIndex) = *(i);
            *(j) = *(i) + 1;
        }
    }
    else{
        if(indexAnimation++ < length){
            drawFinalAnimation(renderer, numbers, length, width);
        }
    }

    return interval;
}
