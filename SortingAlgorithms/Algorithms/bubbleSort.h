#include <stdbool.h>

#include "../func.h"

#define BUBBLESORT {"Bubble Sort", &bubbleSort}

void bubbleSort(uint8_t **buffer, int *numbers, int numbersSize){
    bool isSorted = true;

    for(int i = 0; i < numbersSize - 1; i++){
        isSorted = true;

        for(int j = 0; j < numbersSize - i - 1; j++){
            if(numbers[j + 1] < numbers[j]){
                swap(&numbers[j], &numbers[j + 1]);
                isSorted = false;
            }
            if(buffer != NULL){
                SHOW;
                drawNumbers(j, j + 1);
                wait();
            }
        }

        if(isSorted){
            break;
        }
    }
}
