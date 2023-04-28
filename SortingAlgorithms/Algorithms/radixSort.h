#include <limits.h>

#include "../draw.c"
#include "../singleLinkedList.h"
#include "../func.h"

extern uint8_t *buffer;
extern int *numbers;
extern const unsigned int width;
extern const int length;

extern bool ended;
extern int indexAnimation;
extern void printArray(int*, int);

#define RADIXSORT {"Radix Sort", &radixSort, &radixSortAlg, &radixSortInit, &radixSortFree}

#define headsSize 10

void *radixSortInit(){
    int *i = malloc(sizeof(int));
    int *d = malloc(sizeof(int));
    int *m = malloc(sizeof(int));
    int *headsIndex = malloc(sizeof(int));
    
    *(i) = 0;
    *(d) = -1;
    *(m) = -1;
    *(headsIndex) = 0;

    struct Node **heads = malloc(sizeof(struct Node*) * headsSize);
    for(int i = 0; i < 10; i++){
        struct Node *head = malloc(sizeof(struct Node));
        head->next = NULL;
        *(heads + i) = head;
    }

    size_t paramSize = sizeof(int*) * 4 + sizeof(struct Node*) * headsSize;

    void *param = malloc(paramSize);
    *((int**) (param + sizeof(int*) * 0)) = i;
    *((int**) (param + sizeof(int*) * 1)) = d;
    *((int**) (param + sizeof(int*) * 2)) = m;
    *((int**) (param + sizeof(int*) * 3)) = headsIndex;
    *((struct Node***) (param + sizeof(int*) * 4)) = heads;

    return param;
}

int radixSortFree(void *param){
    int *i          = *((int**) (param + sizeof(int*) * 0));
    int *d          = *((int**) (param + sizeof(int*) * 1));
    int *m          = *((int**) (param + sizeof(int*) * 2));
    int *headsIndex = *((int**) (param + sizeof(int*) * 3));
    struct Node **n = *((struct Node***) (param + sizeof(int*) * 4));
    
    free(i);
    free(d);
    free(m);
    free(headsIndex);
    
    for(int i = 0; i < headsSize; i++){
        free(*(n + i));
    }

    free(n);

    return 0;
}

// Example
// int *numbersCopy = malloc(sizeof(int) * length);
// memcpy(numbersCopy, numbers, sizeof(int) * length);
// radixSortAlg(numbersCopy, length);
// printArray(numbersCopy, length);
int radixSortAlg(int *numbers, int length){
    int mIndex = 0;
    for(int i = 0; i < length; i++){
        if(numbers[mIndex] < numbers[i]){
            mIndex = i;
        }
    }
    int max = numbers[mIndex];

    // Make lookup table
    struct Node **heads = malloc(sizeof(struct Node*) * 10);
    for(int i = 0; i < 10; i++){
        struct Node *head = (struct Node*)malloc(sizeof(struct Node));
        // head->data = NULL;
        head->next = NULL;

        *(heads + i) = head;
    }

    for(int d = 1; max / d > 0; d *= 10){
        for(int i = 0; i < length; i++){
            // nth digit = (N // 10^(D-n)) % 10
            int n = ((int)(numbers[i] / d)) % 10;
            add(*(heads + n), numbers[i]);
        }

        int *p = numbers;

        for(int n = 0; n < 10; n++){
            // printNodes(*(heads + n));
            int d = delete((heads + n), 1);
            while(d != INT_MIN){
                *p++ = d;
                d = delete((heads + n), 1);
            }
        }
    }

    for(int i = 0; i < 10; i++){
        free(*(heads + i));
    }
    free(heads);

    printArray(numbers, length);

    return 0;
}

Uint32 radixSort(Uint32 interval, void *param){
    int *i              = *((int**) (param + sizeof(int*) * 0));
    int *d              = *((int**) (param + sizeof(int*) * 1));
    int *m              = *((int**) (param + sizeof(int*) * 2));
    int *headsIndex     = *((int**) (param + sizeof(int*) * 3));
    struct Node **heads = *((struct Node***) (param + sizeof(int*) * 4));

    // loop and find maximum number (d < 0)
    // loop throw all n digets (0 < d)
    // swap all numbers

    // If array isn't yet sorted
    if(ended == 0){
        if(*d < 0){
            drawNumbers(&buffer, numbers, length, width, *(i), *(i));

            if(*i < length){
                if(*m < *(numbers + *i)){
                    *m = *(numbers + *i);
                }

                (*i)++;
            }
            else{
                *i = 0;
                *d = 1;
            }
        }
        else{
            // First go throw all items and save them into memory
            // after reaching the ended i will be set to -length
            // l = 10; i = -10;
            // ni = length + -10;
            // index will be increasing until 0 
            // when all the numbers in memory should have been moved

            // when i is <0; length)
            if(-1 < *i && *i < length){
                drawNumbers(&buffer, numbers, length, width, *(i), *(i));

                int n = ((int)(*(numbers + *i) / *d)) % 10;
                add(*(heads + n), *(numbers + *i));
                (*i)++;
                if(length <= *i){
                    *i = -length;
                }
            }
            else if(*i < 0){
                // Just convert the negative number to positiv index
                int ni = length + *i;
                drawNumbers(&buffer, numbers, length, width, ni, ni);
                
                // Try to delete first item from current head in linked list 
                // if it returns INT_MIN it means the current head is empty
                // so while the headsIndex is in range search until value is found
                int v = delete((heads + *headsIndex), 1);
                while(v == INT_MIN && *headsIndex < headsSize){
                    (*headsIndex)++;
                    v = delete((heads + *headsIndex), 1);
                }
                if(v != INT_MIN){
                    numbers[ni] = v;
                }

                (*i)++;
                if(*i == 0){
                    *headsIndex = 0;
                    (*d) *= 10;

                    if(*m < *d){
                        ended = 1;
                    }
                }
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
