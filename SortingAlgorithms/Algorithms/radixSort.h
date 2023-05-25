#include <limits.h>

#include "../draw.c"
#include "../func.h"

#include "../singleLinkedList.h"

#define RADIXSORT {"Radix Sort", &radixSort}

#define headsSize 10

void radixSort(int *numbers){
    int mIndex = 0;
    for(int i = 0; i < numbersSize; i++){
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
        for(int i = 0; i < numbersSize; i++){
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
}
