#include "../draw.c"
#include "../func.h"

extern uint8_t *buffer;
extern int *numbers;
extern const unsigned int width;
extern const int length;

extern int indexAnimation;

#define INSERTIONSORT {"Insertion Sort", &insertionSort, &insertionSortAlg, &insertionSortInit, &insertionSortFree}

void *insertionSortInit(){
    int *i = malloc(sizeof(int));
    int *j = malloc(sizeof(int));
    
    *(i) = 1;
    *(j) = *(i) - 1;

    size_t paramSize = sizeof(int*) * 2;

    void *param = malloc(paramSize);
    *((int**) (param + sizeof(int*) * 0)) = i;
    *((int**) (param + sizeof(int*) * 1)) = j;

    return param;
}

int insertionSortFree(void *param){
    int *i  = *((int**) (param + sizeof(int*) * 0));
    int *j  = *((int**) (param + sizeof(int*) * 1));
    
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
    int *i          = *((int**) (param + sizeof(int*) * 0));
    int *j          = *((int**) (param + sizeof(int*) * 1));

    // If array isn't yet sorted
    if(*(i) < length){
        drawNumbers(&buffer, numbers, length, width, *(j), *(j) + 1);
        
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
            drawFinalAnimation(&buffer, numbers, length, width);
        }
    }
    
    return interval;
}
