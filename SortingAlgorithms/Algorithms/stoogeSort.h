#include <math.h>

#include "../func.h"

#define STOOGESORT {"Stooge Sort", &stoogeSort}

void stoogeSort(uint8_t **buffer, int *numbers, int numbersSize){
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
            SHOW;
            drawNumbers(i, j);
            wait();
        }
    }

    free(stack);
}

void stoogeSort2(uint8_t **buffer, int *numbers, int numbersSize, int i){
    if(numbers[numbersSize - 1] < numbers[i]){
        swap(&numbers[i], &numbers[numbersSize - 1]);
    }
    if(buffer != NULL){
        SHOW;
        drawNumbers(i - 1, i);
        wait();
    }

    if(2 < numbersSize - i){
        int oneThird = (int)((numbersSize - i) / 3);
        stoogeSort2(buffer, numbers, numbersSize - oneThird, i);
        stoogeSort2(buffer, numbers, numbersSize, i + oneThird);
        stoogeSort2(buffer, numbers, numbersSize - oneThird, i);
    }
}
