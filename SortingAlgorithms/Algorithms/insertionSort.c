#include "algorithms.h"

void insertionSort(int *numbers, int numbersSize){
    for(int i = 1; i < numbersSize; i++){
        for(int j = i - 1; j >= 0; j--){
            drawNumbers();

            pixelSetColor(p, 255, 0, 0, 255);
            highlight(j);
            highlight(j + 1);

            PixelWait();

            if(numbers[j + 1] < numbers[j]){
                swapNumbers(j, j + 1);
            }
            else{
                break;
            }
        }
    }
}
