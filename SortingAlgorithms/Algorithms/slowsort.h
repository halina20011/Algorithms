#include "../func.h"

#define SLOWSORT {"Slowsort", &slowsort}

void Slowsort(uint8_t *buffer, int *numbers, int left, int right){
    if(right <= left)
        return;

    const int m = left + (right - left) / 2;
    if(buffer != NULL){
        ProcessEvents()
        drawNumbers(m, m);
        highlightRegion(left, right);
        update(buffer);
        wait();
    }
    Slowsort(buffer, numbers, left, m);
    Slowsort(buffer, numbers, m + 1, right);
    if(numbers[right] < numbers[m]){
        swap(numbers + right, numbers + m);
    }

    Slowsort(buffer, numbers, left, right - 1);
}

void slowsort(uint8_t *buffer, int *numbers, int numbersSize){
    Slowsort(buffer, numbers, 0, numbersSize - 1);
}
