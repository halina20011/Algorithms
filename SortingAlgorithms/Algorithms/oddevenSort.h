#include <stdbool.h>

#include "../func.h"

#define ODDEVENSORT {"Odd-even Sort", &oddevenSort}

void oddevenSort(uint8_t **buffer, int *numbers, int numbersSize){
    bool sorted = false;

    while(sorted == false){
        sorted = true;

        // odd part
        for(int i = 1; i < numbersSize - 1; i += 2){
            if(numbers[i + 1] < numbers[i]){
                swap(&numbers[i], &numbers[i + 1]);
                sorted = false;
            }
            if(buffer != NULL){
                SHOW;
                drawNumbers(i, i + 1);
                wait();
            }
        }

        // even part
        for(int i = 0; i < numbersSize; i += 2){
            if(numbers[i + 1] < numbers[i]){
                swap(&numbers[i], &numbers[i + 1]);
                sorted = false;
            }
            if(buffer != NULL){
                SHOW;
                drawNumbers(i, i + 1);
                wait();
            }
        }

    }
}
