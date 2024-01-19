#include "algorithms.h"

void Slowsort(int *numbers, int left, int right){
    if(right <= left)
        return;

    const int m = left + (right - left) / 2;
    drawNumbers();
    highlightRegion(left, right);
    pixelSetColor(p, 255, 0, 0, 255);
    highlight(m);
    PixelWait();

    Slowsort(numbers, left, m);
    Slowsort(numbers, m + 1, right);

    if(numbers[right] < numbers[m]){
        swapNumbers(right, m);
    }

    Slowsort(numbers, left, right - 1);
}

void slowsort(int *numbers, int numbersSize){
    Slowsort(numbers, 0, numbersSize - 1);
}
