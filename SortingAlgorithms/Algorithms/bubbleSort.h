#include <stdbool.h>
#include "../draw.c"

#define BUBBLESORT {"Bubble Sort", &bubbleSort}

void bubbleSort(uint8_t **buffer, int *numbers){
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
                drawNumbers(buffer, numbers, numbersSize, numbersWidth, j, j + 1);
                wait();
            }
        }

        if(isSorted){
            break;
        }
    }
}
