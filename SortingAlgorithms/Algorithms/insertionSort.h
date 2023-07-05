#include "../func.h"

#define INSERTIONSORT {"Insertion Sort", &insertionSort}

void insertionSort(uint8_t **buffer, int *numbers, int numbersSize){
    for(int i = 1; i < numbersSize; i++){
        for(int j = i - 1; j >= 0; j--){
            if(buffer != NULL){
                SHOW;
                drawNumbers(j, j + 1);
                wait();
            }
            if(numbers[j + 1] < numbers[j]){
                swap(&numbers[j], &numbers[j + 1]);
            }
            else{
                break;
            }
        }
    }
}
