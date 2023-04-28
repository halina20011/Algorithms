#include "../draw.c"
#include "../func.h"

extern uint8_t *buffer;
extern int *numbers;
extern const unsigned int width;
extern const int length;

extern int indexAnimation;

#define GNOMESORT {"Gnome Sort", &gnomeSort, gnomeSortAlg, &gnomeSortInit, &gnomeSortFree}

void *gnomeSortInit(){
    int *i = malloc(sizeof(int));
    
    *(i) = 0;

    size_t paramSize = sizeof(int*);

    void *param = malloc(paramSize);
    *((int**) (param + sizeof(int*))) = i;

    return param;
}

int gnomeSortFree(void *param){
    int *i  = *((int**) (param + sizeof(int*)));
    
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
    int *i          = *((int**) (param + sizeof(int*)));

    // If array isn't yet sorted
    if(*(i) < length){
        drawNumbers(&buffer, numbers, length, width, *(i), *(i) + 1);
        
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
            drawFinalAnimation(&buffer, numbers, length, width);
        }
    }
    
    return interval;
}
