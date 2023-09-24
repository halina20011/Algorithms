#include <math.h>

#include "../../pixel.h"
#include "../func.h"

#define STOOGESORT {"Stooge Sort", &stoogeSort}

extern struct Pixel *p;

void stoogeSortStack(int *numbers, int numbersSize){
    int i = 0;
    int j = numbersSize - 1;
    int pos = 1;

    int *stack = malloc(sizeof(int) * numbersSize);

    stack[0] = i;
    stack[1] = j;

    while(0 <= pos){
        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(i);
        highlight(j);
        PixelWait();

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
            swapNumbers(j, i);
        }
    }

    free(stack);
}

void stoogeSortRec(int *numbers, int numbersSize, int i){
    if(numbers[numbersSize - 1] < numbers[i]){
        swapNumbers(i, numbersSize - 1);
    }

    const int oneThird = (int)((numbersSize - i) / 3);
    
    drawNumbers();
    highlightRegion(i + oneThird, numbersSize - oneThird);
    pixelSetColor(p, 255, 0, 0, 255);
    highlight(i - 1);
    highlight(i);
    PixelWait();

    if(2 < numbersSize - i){
        stoogeSortRec(numbers, numbersSize - oneThird, i);
        stoogeSortRec(numbers, numbersSize, i + oneThird);
        stoogeSortRec(numbers, numbersSize - oneThird, i);
    }
}

void stoogeSort(int *numbers, int numbersSize){
    stoogeSortRec(numbers, numbersSize, 0);
    drawFinalAnimation(numbers, numbersSize);
}
