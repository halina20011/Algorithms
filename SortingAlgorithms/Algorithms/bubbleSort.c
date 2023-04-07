#include "../draw.c"

extern int ended;
extern int indexAnimation;

extern void printArray(int*, int);

void *bubbleSortInit(uint8_t **buffer, int *numbers, int width){
    int *i = malloc(sizeof(int));
    int *j = malloc(sizeof(int));
    int *sorted = malloc(sizeof(int));
    
    *(i) = 0;
    *(j) = 0;
    *(sorted) = 0;

    size_t paramSize = sizeof(uint8_t*) + sizeof(int*) * 4 + sizeof(int);
    printf("Param size: %zu\n", paramSize);

    void *param = malloc(paramSize);
    *((uint8_t***) (param)) = buffer;
    *((int**) (param + sizeof(uint8_t*))) = numbers;
    *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1)) = i;
    *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 2)) = j;
    *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 3)) = sorted;
    *((int*)  (param + sizeof(uint8_t*) + sizeof(int*) * 4)) = width;

    return param;
}

int bubbleSortFree(void *param){
    int *i          = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1));
    int *j          = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 2));
    int *sorted     = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 3));
    
    free(i);
    free(j);
    free(sorted);

    return 0;
}

void swap(int *x, int *y){
    int temp = *(x);
    *(x) = *(y);
    *(y) = temp;
}

// Example
// int *numbersCopy = malloc(sizeof(int) * length);
// memcpy(numbersCopy, numbers, sizeof(int) * length);
// bubbleSortAlg(numbersCopy, length);
// printArray(numbersCopy, length);
void bubbleSortAlg(int *numbers, int length){
    int isSorted = 1;

    for(int i = 0; i < length - 1; i++){
        isSorted = 1;

        for(int j = 0; j < length - i - 1; j++){
            if(numbers[j + 1] < numbers[j]){
                swap(&numbers[j], &numbers[j + 1]);
                isSorted = 0;
            }
        }

        if(isSorted == 1){
            break;
        }
    }

    printArray(numbers, length);
}

Uint32 bubbleSort(Uint32 interval, void *param){
    uint8_t **buffer = *((uint8_t***) param);
    int *numbers    = *((int**) (param + sizeof(uint8_t*)));
    int *i          = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1));
    int *j          = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 2));
    int *sorted     = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 3));
    int width       = *((int*)  (param + sizeof(uint8_t*) + sizeof(int*) * 4));

    int length = WINDOWWIDTH / width;

    // If array isn't yet sorted
    if(ended == 0){
        drawNumbers(buffer, numbers, length, width, *(j), *(j) + 1);
        // int *i = NULL;
        // printf("%i", *i);
        
        // Second foor lop
        if(*(i) < length){
            int n1 = numbers[*(j)];
            int n2 = numbers[*(j) + 1];

            if(n2 < n1){
                swap(&numbers[*(j)], &numbers[*(j) + 1]);
                *(sorted) = 0;
            }

            *(j) += 1;
        }
        else{
            ended = 1;
        }

        // Firts for loop
        // If j reatched the end
        if(*(j) == length - *(i) - 1){
            if(*(sorted) == 1){
                ended = 1;
            }

            *(sorted) = 1;
            *(i) += 1;
            *(j) = 0;
        }
    }
    else{
        if(indexAnimation++ < length){
            drawFinalAnimation(buffer, numbers, length, width);
        }
    }
    
    return interval;
}
