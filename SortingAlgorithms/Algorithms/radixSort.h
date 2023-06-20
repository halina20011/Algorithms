#include <limits.h>

#include "../draw.c"
#include "../func.h"

#include "../singleLinkedList.h"

#define RADIXSORT {"Radix Sort", &radixSort}

void radixSort(uint8_t **buffer, int *numbers){
    int minIndex = 0;
    for(int i = 0; i < numbersSize; i++){
        if(numbers[minIndex] < numbers[i]){
            minIndex = i;
        }
        if(buffer != NULL){
            SHOW;
            drawNumbers(buffer, numbers, numbersSize, numbersWidth, minIndex, i);
            wait();
        }
    }
    const int max = numbers[minIndex];

    // make lookup table
    struct Node **heads = malloc(sizeof(struct Node*) * 10);
    for(int i = 0; i < 10; i++){
        struct Node *head = (struct Node*)malloc(sizeof(struct Node));
        head->next = NULL;
        heads[i] = head;
    }

    for(int d = 1; max / d > 0; d *= 10){
        for(int i = 0; i < numbersSize; i++){
            // nth digit = (N // 10^(D-n)) % 10
            // extract digit that will be used as a key
            int n = ((int)(numbers[i] / d)) % 10;
            add(heads[n], numbers[i]);
            if(buffer != NULL){
                SHOW;
                drawNumbers(buffer, numbers, numbersSize, numbersWidth, i, i);
                wait();
            }
        }

        int pIndex = 0;

        for(int n = 0; n < 10; n++){
            int d = delete((heads + n), 1);
            while(d != INT_MIN){
                numbers[pIndex++] = d;
                d = delete((heads + n), 1);
                if(buffer != NULL){
                    SHOW;
                    drawNumbers(buffer, numbers, numbersSize, numbersWidth, pIndex, pIndex);
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
