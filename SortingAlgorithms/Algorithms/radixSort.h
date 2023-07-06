#include <limits.h>

#include "../func.h"
#include "../singleLinkedList.h"

#define RADIXSORT {"Radix Sort", &radixSort}

void radixSort(uint8_t *buffer, int *numbers, int numbersSize){
    int minIndex = 0;
    for(int i = 0; i < numbersSize; i++){
        if(numbers[minIndex] < numbers[i]){
            minIndex = i;
        }
        if(buffer != NULL){
            ProcessEvents()
            drawNumbers(minIndex, i);
            update(buffer);
            wait();
        }
    }
    const int max = numbers[minIndex];

    // make lookup table
    struct Node **heads = malloc(sizeof(struct Node*) * 10);
    struct Node **tails = malloc(sizeof(struct Node*) * 10);
    for(int i = 0; i < 10; i++){
        heads[i] = NULL;
        tails[i] = NULL;
    }

    for(int d = 1; max / d > 0; d *= 10){
        for(int i = 0; i < numbersSize; i++){
            // nth digit = (N // 10^(D-n)) % 10
            // extract digit that will be used as a key
            int n = ((int)(numbers[i] / d)) % 10;
            add(heads + n, tails + n, numbers[i]);
            if(buffer != NULL){
                ProcessEvents()
                drawNumbers(i, i);
                update(buffer);
                wait();
            }
        }

        int pIndex = 0;

        for(int n = 0; n < 10; n++){
            int d = deleteFirst((heads + n));
            while(d != INT_MIN){
                numbers[pIndex++] = d;
                d = deleteFirst((heads + n));
                if(buffer != NULL){
                    ProcessEvents()
                    drawNumbers(pIndex, pIndex);
                    update(buffer);
                    wait();
                }
            }
        }
    }

    for(int i = 0; i < 10; i++){
        free(heads[i]);
    }

    free(heads);
}
