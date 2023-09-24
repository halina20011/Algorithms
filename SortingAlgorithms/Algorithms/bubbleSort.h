#include <stdbool.h>

#include "../../pixel.h"
#include "../func.h"

#define BUBBLESORT {"Bubble Sort", &bubbleSort}

extern struct Pixel *p;

void bubbleSort(int *numbers, int numbersSize){
    bool isSorted = true;

    for(int i = 0; i < numbersSize - 1; i++){
        isSorted = true;

        for(int j = 0; j < numbersSize - i - 1; j++){
            drawNumbers();

            pixelSetColor(p, 255, 0, 0, 255);
            highlight(j);
            highlight(j + 1);

            PixelWait();

            if(numbers[j + 1] < numbers[j]){
                swapNumbers(j, j + 1);
                isSorted = false;
            }
        }

        if(isSorted){
            break;
        }
    }
    
    drawFinalAnimation(numbers, numbersSize);
}
