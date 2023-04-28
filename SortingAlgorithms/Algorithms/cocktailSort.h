#include <stdbool.h>

#include "../draw.c"
#include "../func.h"

extern uint8_t *buffer;
extern int *numbers;
extern const unsigned int width;
extern const int length;

extern bool ended;
extern int indexAnimation;

#define COCKTAILSORT {"Cocktail Sort", &cocktailSort, &cocktailSortAlg, &cocktailSortInit, &cocktailSortFree}

void *cocktailSortInit(){
    int *i = malloc(sizeof(int));
    int *start = malloc(sizeof(int));
    int *end = malloc(sizeof(int));

    bool *direction = malloc(sizeof(bool));
    bool *sorted = malloc(sizeof(bool));
    
    *i = 0;
    *start = 0;
    *end = length - 1;
    
    *direction = 0;
    *sorted = true;

    size_t paramSize = sizeof(int*) * 3 + sizeof(bool*) * 2;

    void *param = malloc(paramSize);
    *((int**) (param + sizeof(int*) * 0)) = i;
    *((int**) (param + sizeof(int*) * 1)) = start;
    *((int**) (param + sizeof(int*) * 2)) = end;
    *((bool**) (param + sizeof(int*) * 3)) = direction;
    *((bool**)  (param + sizeof(int*) * 3 + sizeof(bool*))) = sorted;

    return param;
}

int cocktailSortFree(void *param){
    int *i          = *((int**) (param + sizeof(int*) * 0));
    int *start      = *((int**) (param + sizeof(int*) * 1));
    int *end        = *((int**) (param + sizeof(int*) * 2));
    bool *direction = *((bool**) (param + sizeof(int*) * 3));
    bool *sorted    = *((bool**) (param + sizeof(int*) * 3 + sizeof(bool*)));
    
    free(i);
    free(start);
    free(end);
    free(direction);
    free(sorted);

    return 0;
}

void cocktailSortAlg(int *numbers, int length){
    bool sorted = 0;
    int start = 0;
	int end = length - 1;

    while(!sorted){
		sorted = true;
		for(int i = start; i < end; i++){
            if(numbers[i + 1] < numbers[i]){
                swap(&numbers[i], &numbers[i+1]);
                sorted = false;
            }
		}
		end--;

		if(sorted)
            break;

		for(int i = end; i > start; i--){
            if(numbers[i] < numbers[i - 1]){
                swap(&numbers[i], &numbers[i - 1]);
                sorted = false;
            }
		}
		start++;
	}

    printArray(numbers, length);
}

Uint32 cocktailSort(Uint32 interval, void *param){
    int *i          = *((int**) (param + sizeof(int*) * 0));
    int *start      = *((int**) (param + sizeof(int*) * 1));
    int *end        = *((int**) (param + sizeof(int*) * 2));
    bool *direction = *((bool**) (param + sizeof(int*) * 3));
    bool *sorted    = *((bool**) (param + sizeof(int*) * 3 + sizeof(bool*)));
    
    // If array isn't yet sorted
    if(ended == 0){
        // direction false = from left to right
        //           true  = from right to left
        if(*direction == 0){
            drawNumbers(&buffer, numbers, length, width, *(i), *(i) + 1);
            // on the start of the loop
            if(*i < *end){
                if(numbers[*i + 1] < numbers[*i]){
                    swap(&numbers[*i], &numbers[*i + 1]);
                    *sorted = false;
                }

                *i += 1;
            }
            if(*i == *end){
                if(*sorted == true)
                    ended = 1;
                
                *sorted = true;
                *direction = 1;
                *end -= 1;
                *i = *end;
            }
        }
        else{
            drawNumbers(&buffer, numbers, length, width, *(i) - 1, *(i));
            if(*start < *i){
                if(numbers[*i] < numbers[*i - 1]){
                    swap(&numbers[*i], &numbers[*i - 1]);
                    *sorted = false;
                }
                
                *i -= 1;
            }
            if(*i == *start){
                if(*sorted == true)
                    ended = 1;
                *sorted = true;
                *direction = 0;
                *start += 1;
                *i = *start;
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
