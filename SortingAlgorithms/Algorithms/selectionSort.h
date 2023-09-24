#include "../func.h"
#include "../../pixel.h"

#define SELECTIONSORT {"Selection Sort", &selectionSort}

extern struct Pixel *p;

void selectionSort(int *numbers, int numbersSize){
    int minIndex = 0;

    for(int i = 0; i < numbersSize - 1; i++){
        minIndex = i;
        drawNumbers();
        for(int j = i + 1; j < numbersSize; j++){
            if(numbers[j] < numbers[minIndex]){
                minIndex = j;
            }

            drawNumbers();
            pixelSetColor(p, 255, 255, 0, 255);
            highlight(minIndex);
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            highlight(j);
            PixelWait();
        }

        pixelSetColor(p, 255, 255, 255, 255);
        highlight(numbersSize - 1);
        if(minIndex != i){
            swapNumbers(minIndex, i);
        }
    }

    drawFinalAnimation(numbers, numbersSize);
}
