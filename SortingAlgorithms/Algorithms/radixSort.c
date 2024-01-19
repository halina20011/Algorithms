#include "algorithms.h"

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
    struct List **list = malloc(sizeof(struct List*) * 10);
    for(int i = 0; i < 10; i++){
        list[i] = newSinglyLinkedList();
    }

    for(int d = 1; max / d > 0; d *= 10){
        for(int i = 0; i < numbersSize; i++){
            // nth digit = (N // 10^(D-n)) % 10
            // extract digit that will be used as a key
            int n = ((int)(numbers[i] / d)) % 10;
            listPushConvert(list[n], int, numbers[i]);
            
            drawNumbers();
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            PixelWait();
        }

        int pIndex = 0;

        for(int n = 0; n < 10; n++){
            struct Node *curr = list[n]->head, *temp = NULL;
            while(curr){
                temp = curr;
                curr = curr->next;
                drawNumbers();
                pixelSetColor(p, 255, 0, 0, 255);
                highlight(pIndex);
                PixelWait();

                numbers[pIndex++] = *(int*)temp->val;
            }

            singlyLinkedListClear(list[n], true);
        }
    }

    for(int i = 0; i < 10; i++){
        freeSinglyLinkedList(list[i], true);
    }

    free(list);
}
