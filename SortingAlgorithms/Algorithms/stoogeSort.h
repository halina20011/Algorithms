#include <math.h>

#include "../draw.c"
#include "../func.h"

#define STOOGESORT {"Stooge Sort", &stoogeSort}

void stoogeSort(uint8_t **buffer, int *numbers){
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
            if(buffer != NULL){
                SHOW;
                drawNumbers(buffer, numbers, numbersSize, numbersWidth, i, j);
                wait();
            }
        }
    }

    free(stack);
}

void stoogeSortAlgRecursive(int *numbers, int length, int i){
    if(numbers[length - 1] < numbers[i]){
        swap(&numbers[i], &numbers[length - 1]);
    }

    if(2 < length - i){
        int oneThird = (int)((length - i) / 3);
        stoogeSortAlgRecursive(numbers, length - oneThird, i);
        stoogeSortAlgRecursive(numbers, length, i + oneThird);
        stoogeSortAlgRecursive(numbers, length - oneThird, i);
    }
}
