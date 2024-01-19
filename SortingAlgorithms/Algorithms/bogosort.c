#include "algorithms.h"

void bogosort(int *numbers, int numbersSize){
    bool sorted = false;

    // if array isn't yet sorted
    while(!sorted){
        sorted = true;
        shuffleArray(numbers, numbersSize);
        for(int i = 0; i < numbersSize - 1; i++){
            if(numbers[i + 1] < numbers[i]){
                sorted = false;
                break;
            }

            drawNumbers(i, i + 1);
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            highlight(i + 1);
            PixelWait();
        }
    }
}
