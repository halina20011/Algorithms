#include "../draw.c"

extern int ended;
extern int indexAnimation;

#define GNOMESORT {"Gnome Sort", &gnomeSort, gnomeSortAlg, &gnomeSortInit, &gnomeSortFree}

void *gnomeSortInit(uint8_t **buffer, int *numbers, int width){
    int *i = malloc(sizeof(int));
    
    *(i) = 0;

    size_t paramSize = sizeof(uint8_t*) + sizeof(int*) * 2 + sizeof(int);
    printf("Param size: %zu\n", paramSize);

    void *param = malloc(paramSize);
    *((uint8_t***) (param)) = buffer;
    *((int**) (param + sizeof(uint8_t*))) = numbers;
    *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1)) = i;
    *((int*)  (param + sizeof(uint8_t*) + sizeof(int*) * 2)) = width;

    return param;
}

int gnomeSortFree(void *param){
    int *i  = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1));
    
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
    uint8_t **buffer = *((uint8_t***) param);
    int *numbers    = *((int**) (param + sizeof(uint8_t*)));
    int *i          = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1));
    int width       = *((int*)  (param + sizeof(uint8_t*) + sizeof(int*) * 2));

    int length = WINDOWWIDTH / width;

    // If array isn't yet sorted
    if(*(i) < length){
        drawNumbers(buffer, numbers, length, width, *(i), *(i) + 1);
        
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
            drawFinalAnimation(buffer, numbers, length, width);
        }
    }
    
    return interval;
}
