#include "../func.h"

#define SELECTIONSORT {"Selection Sort", &selectionSort}

void selectionSort(uint8_t **buffer, int *numbers){
    int minIndex = 0;

    for(int i = 0; i < numbersSize - 1; i++){
        minIndex = i;
        for(int j = i + 1; j < numbersSize; j++){
            if(numbers[j] < numbers[minIndex]){
                minIndex = j;
            }
            if(buffer != NULL){
                SHOW;
                drawNumbers(buffer, numbers, numbersSize, numbersWidth, j, minIndex);
                wait();
            }
        }

        if(minIndex != i){
            swap(&numbers[minIndex], &numbers[i]);
        }
    }
}
