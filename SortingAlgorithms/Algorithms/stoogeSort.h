#include <math.h>

#include "../func.h"

#define STOOGESORT {"Stooge Sort", &stoogeSort}

void stoogeSortStack(uint8_t *buffer, int *numbers, int numbersSize){
    int i = 0;
    int j = numbersSize - 1;
    int pos = 1;

    int *stack = malloc(sizeof(int) * numbersSize);

    stack[0] = i;
    stack[1] = j;

    while(0 <= pos){
        j = stack[pos--];
        i = stack[pos--];

        if(2 < j - i + 1){
            int oneThird = ((j - i + 1) / 3);
            stack[++pos] = i;
            stack[++pos] = j - oneThird;

            stack[++pos] = i + oneThird;
            stack[++pos] = j;

            stack[++pos] = i;
            stack[++pos] = j - oneThird;
        }

        if(numbers[j] < numbers[i]){
            swap(&numbers[j], &numbers[i]);
        }
        if(buffer != NULL){
            ProcessEvents()
            drawNumbers(i, j);
            update(buffer);
            wait();
        }
    }

    free(stack);
}

void stoogeSortRec(uint8_t *buffer, int *numbers, int numbersSize, int i){
    if(numbers[numbersSize - 1] < numbers[i]){
        swap(numbers + i, numbers + (numbersSize - 1));
    }
    const int oneThird = (int)((numbersSize - i) / 3);

    if(buffer != NULL){
        ProcessEvents()
        drawNumbers(i - 1, i);
        highlightRegion( i + oneThird, numbersSize - oneThird);
        update(buffer);
        wait();
    }

    if(2 < numbersSize - i){
        stoogeSortRec(buffer, numbers, numbersSize - oneThird, i);
        stoogeSortRec(buffer, numbers, numbersSize, i + oneThird);
        stoogeSortRec(buffer, numbers, numbersSize - oneThird, i);
    }
}

void stoogeSort(uint8_t *buffer, int *numbers, int numbersSize){
    stoogeSortRec(buffer, numbers, numbersSize, 0);
}
