#include "../draw.c"

extern uint8_t *buffer;
extern int *numbers;
extern const unsigned int width;
extern const int length;

extern int finalAnimation;

#define SELECTIONSORT {"Selection Sort", &selectionSort, &selectionSortAlg, &selectionSortInit, &selectionSortFree}

void *selectionSortInit(){
    int *i = malloc(sizeof(int));
    int *j = malloc(sizeof(int));
    int *minIndex = malloc(sizeof(int));
    
    *(i) = 0;
    *(j) = *(i) + 1;
    *(minIndex) = 0;

    size_t paramSize = sizeof(int*) * 3;

    void *param = malloc(paramSize);
    *((int**) (param + sizeof(int*) * 0)) = i;
    *((int**) (param + sizeof(int*) * 1)) = j;
    *((int**) (param + sizeof(int*) * 2)) = minIndex;

    return param;
}

int *selectionSortFree(void *param){
    int *i          = *((int**) (param + sizeof(int*) * 0));
    int *j          = *((int**) (param + sizeof(int*) * 1));
    int *minIndex   = *((int**) (param + sizeof(int*) * 2));
    
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
    int *i          = *((int**) (param + sizeof(int*) * 0));
    int *j          = *((int**) (param + sizeof(int*) * 1));
    int *minIndex   = *((int**) (param + sizeof(int*) * 2));
   
    // If it is not sorted procede
    if(*(i) < length){
        drawNumbers(&buffer, numbers, length, width, *(i), *(j));

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
            drawFinalAnimation(&buffer, numbers, length, width);
        }
    }

    return interval;
}
