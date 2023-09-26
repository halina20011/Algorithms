#include <stdbool.h>

#include "../../pixel.h"
#include "../func.h"

#define COMBSORT {"Comb sort", &combSort}

void combSort(int *numbers, int size){
    int gap = size;
    double shrink = 1.3;
    bool sorted = false;

    while(!sorted){
        gap /= shrink;
        if(gap <= 1){
            gap = 1;
            sorted = true;
        }

        for(int i = 0; i + gap < size; i++){
            drawNumbers();
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            highlight(i + gap);
            PixelWait();

            if(numbers[i + gap] < numbers[i]){
                swapNumbers(i, i + gap);
                sorted = false;
            }
        }
    }

    drawFinalAnimation(numbers, size);
}
