#include <limits.h>

#include "../../pixel.h"
#include "../func.h"
#include "../singleLinkedList.h"

#define RADIXSORT {"Radix Sort", &radixSort}

extern struct Pixel *p;

void radixSort(int *numbers, int numbersSize){
    int minIndex = 0;

    for(int i = 0; i < numbersSize; i++){
        drawNumbers();
        pixelSetColor(p, 255, 255, 0, 255);
        highlight(minIndex);
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(i);
        PixelWait();

        if(numbers[minIndex] < numbers[i]){
            minIndex = i;
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
            
            drawNumbers();
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            PixelWait();
        }

        int pIndex = 0;

        for(int n = 0; n < 10; n++){
            struct Node *curr = heads[n], *temp = NULL;
            while(curr){
                temp = curr;
                curr = curr->next;
                drawNumbers();
                pixelSetColor(p, 255, 0, 0, 255);
                highlight(pIndex);
                PixelWait();

                numbers[pIndex++] = temp->data;
                free(temp);
            }

            heads[n] = NULL;
            tails[n] = NULL;
        }
    }

    for(int i = 0; i < 10; i++){
        free(heads[i]);
        free(tails[i]);
    }

    free(heads);
    free(tails);

    drawFinalAnimation(numbers, numbersSize);
}
