#include "../draw.c"

extern int ended;
extern int indexAnimation;

void *oddevenSortInit(uint8_t **buffer, int *numbers, int width){
    int *i      = malloc(sizeof(int));
    int *sorted = malloc(sizeof(int));
    
    *(i) = 0;
    *(sorted) = 0;

    size_t paramSize = sizeof(uint8_t*) + sizeof(int*) * 3 + sizeof(int);
    printf("Param size: %zu\n", paramSize);

    void *param = malloc(paramSize);
    *((uint8_t***) (param)) = buffer;
    *((int**) (param + sizeof(uint8_t*))) = numbers;
    *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1)) = i;
    *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 2)) = sorted;
    *((int*)  (param + sizeof(uint8_t*) + sizeof(int*) * 3)) = width;

    return param;
}

int oddevenSortFree(void *param){
    int *i      = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1));
    int *sorted = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 2));
    
    free(i);
    free(sorted);

    return 0;
}

// Example
// int *numbersCopy = malloc(sizeof(int) * length);
// memcpy(numbersCopy, numbers, sizeof(int) * length);
// oddevenSortAlg(numbersCopy, length);
// printArray(numbersCopy, length);
int oddevenSortAlg(int *numbers, int length){
    int sorted = 0;

    while(sorted == 0){
        sorted = 1;

        for(int i = 0; i < length - 1; i += 2){
            if(numbers[i + 1] < numbers[i]){
                swap(&numbers[i], &numbers[i + 1]);
                sorted = 0;
            }
        }

        for(int i = 1; i < length - 2; i += 2){
            if(numbers[i + 1] < numbers[i]){
                swap(&numbers[i], &numbers[i + 1]);
                sorted = 0;
            }
        }
    }

    return 0;
}

Uint32 oddevenSort(Uint32 interval, void *param){
    uint8_t **buffer = *((uint8_t***) param);
    int *numbers    = *((int**) (param + sizeof(uint8_t*)));
    int *i          = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1));
    int *sorted     = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 2));
    int width       = *((int*)  (param + sizeof(uint8_t*) + sizeof(int*) * 3));

    int length = WINDOWWIDTH / width;

    // If array isn't yet sorted
    if(ended == 0){
        drawNumbers(buffer, numbers, length, width, *(i), *(i) + 1);
        int isEven = (*(i) % 2 == 0) ? 1 : 0;
        
        // check for end
        // if its even then the end will be one less
        // if its odd then the end will be two less
        if(isEven == 1 && length - 1 <= *(i) || isEven == 0 && length - 2 <= *(i)){
            // IsEven will be 1 so odd value and that is would next i should be
            *(i) = isEven;

            if(*(sorted) == 1 && isEven == 1){
                ended = 1;
            }

            *(sorted) = 1;
        }

        if(numbers[*(i) + 1] < numbers[*(i)]){
            swap(&numbers[*(i)], &numbers[*(i) + 1]);
            *(sorted) = 0;
        }

        *(i) += 2;
    }
    else{
        if(indexAnimation++ < length){
            drawFinalAnimation(buffer, numbers, length, width);
        }
    }
    
    return interval;
}
