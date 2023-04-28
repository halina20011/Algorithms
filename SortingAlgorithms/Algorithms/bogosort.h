#include "../draw.c"
#include "../func.h"

extern uint8_t *buffer;
extern int *numbers;
extern const unsigned int width;
extern const int length;

extern bool ended;
extern int indexAnimation;

#define BOGOSORT {"Bogosort", &bogosort, NULL, &bogosortInit, &bogosortFree}

void *bogosortInit(){
    int *i = malloc(sizeof(int));
    int *sorted = malloc(sizeof(int));
    
    *(i) = 0;
    *(sorted) = 0;

    size_t paramSize = sizeof(int*) * 2;

    void *param = malloc(paramSize);
    *((int**) (param + sizeof(int*) * 0)) = i;
    *((int**) (param + sizeof(int*) * 1)) = sorted;

    return param;
}

int bogosortFree(void *param){
    int *i      = *((int**) (param + sizeof(int*) * 0));
    int *sorted = *((int**) (param + sizeof(int*) * 1));
    
    free(i);
    free(sorted);

    return 0;
}

Uint32 bogosort(Uint32 interval, void *param){
    int *i      = *((int**) (param + sizeof(int*) * 0));
    int *sorted = *((int**) (param + sizeof(int*) * 1));

    // If array isn't yet sorted
    if(ended == 0){
        drawNumbers(&buffer, numbers, length, width, *(i), *(i) + 1);
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
            drawFinalAnimation(&buffer, numbers, length, width);
        }
    }
    
    return interval;
}
