#include "../draw.c"

#include <stdbool.h>

extern int ended;
extern int indexAnimation;

extern void printArray(int*, int);

#define COCKTAILSORT {"Cocktail Sort", &cocktailSort, &cocktailSortAlg, &cocktailSortInit, &cocktailSortFree}

void *cocktailSortInit(uint8_t **buffer, int *numbers, int width){
    int *i = malloc(sizeof(int));
    int *start = malloc(sizeof(int));
    int *end = malloc(sizeof(int));

    bool *direction = malloc(sizeof(bool));
    bool *sorted = malloc(sizeof(bool));
    
    int length = WINDOWWIDTH / width;
    
    *i = 0;
    *start = 0;
    *end = length - 1;
    
    *direction = 0;
    *sorted = true;

    size_t paramSize = sizeof(uint8_t*) + sizeof(int*) * 4 + sizeof(bool*) * 2 + sizeof(int);
    printf("Param size: %zu\n", paramSize);

    void *param = malloc(paramSize);
    *((uint8_t***) (param)) = buffer;
    *((int**) (param + sizeof(uint8_t*))) = numbers;
    *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1)) = i;
    *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 2)) = start;
    *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 3)) = end;
    *((bool**) (param + sizeof(uint8_t*) + sizeof(int*) * 4)) = direction;
    *((bool**)  (param + sizeof(uint8_t*) + sizeof(int*) * 4 + sizeof(bool*))) = sorted;
    *((int*)  (param + sizeof(uint8_t*) + sizeof(int*) * 4 + sizeof(bool*) * 2)) = width;

    return param;
}

int cocktailSortFree(void *param){
    int *i          = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1));
    int *start      = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 2));
    int *end        = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 3));
    bool *direction = *((bool**) (param + sizeof(uint8_t*) + sizeof(int*) * 4));
    bool *sorted    = *((bool**) (param + sizeof(uint8_t*) + sizeof(int*) * 4 + sizeof(bool*)));
    
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
    uint8_t **buffer = *((uint8_t***) param);
    int *numbers    = *((int**) (param + sizeof(uint8_t*)));
    int *i          = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 1));
    int *start      = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 2));
    int *end        = *((int**) (param + sizeof(uint8_t*) + sizeof(int*) * 3));
    bool *direction = *((bool**) (param + sizeof(uint8_t*) + sizeof(int*) * 4));
    bool *sorted    = *((bool**) (param + sizeof(uint8_t*) + sizeof(int*) * 4 + sizeof(bool*)));
    int width       = *((int*)  (param + sizeof(uint8_t*) + sizeof(int*) * 4 + sizeof(bool*) * 2));

    int length = WINDOWWIDTH / width;
    
    // If array isn't yet sorted
    if(ended == 0){
        // direction false = from left to right
        //           true  = from right to left
        if(*direction == 0){
            drawNumbers(buffer, numbers, length, width, *(i), *(i) + 1);
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
            drawNumbers(buffer, numbers, length, width, *(i) - 1, *(i));
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
            drawFinalAnimation(buffer, numbers, length, width);
        }
    }
    
    return interval;
}
