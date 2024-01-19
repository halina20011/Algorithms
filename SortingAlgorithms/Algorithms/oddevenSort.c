#include "algorithms.h"

void oddevenSort(int *numbers, int numbersSize){
    bool sorted = false;

    while(!sorted){
        sorted = true;
    
        // even = 0, odd = 1
        for(int t = 0; t < 2; t++){
            for(int i = t; i < numbersSize - 1; i += 2){
                drawNumbers();
                pixelSetColor(p, 255, 0, 0, 255);
                highlight(i);
                highlight(i + 1);
                PixelWait();

                if(numbers[i + 1] < numbers[i]){
                    swapNumbers(i, i + 1);
                    sorted = false;
                }
            }

            if(sorted){
                break;
            }
        }
    }
}
