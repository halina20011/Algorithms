#include "../draw.c"
#include "../func.h"

#define INSERTIONSORT {"Insertion Sort", &insertionSort}

void insertionSort(uint8_t **buffer, int *numbers){
    for(int i = 1; i < numbersSize; i++){
        for(int j = i - 1; j >= 0; j--){
            if(numbers[j + 1] < numbers[j]){
                swap(&numbers[j], &numbers[j + 1]);
                if(buffer != NULL){
                    SHOW;
                    drawNumbers(buffer, numbers, numbersSize, numbersWidth, i, j);
                    wait();
                }
            }
            else{
                break;
            }
        }
    }
}
