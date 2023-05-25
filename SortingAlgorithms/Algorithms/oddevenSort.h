#include <stdbool.h>

#include "../draw.c"
#include "../func.h"

#define ODDEVENSORT {"Odd-even Sort", &oddevenSort}

void oddevenSort(uint8_t **buffer, int *numbers){
    bool sorted = false;

    while(sorted == false){
        sorted = true;

        for(int i = 0; i < numbersSize - 1; i += 2){
            if(numbers[i + 1] < numbers[i]){
                swap(&numbers[i], &numbers[i + 1]);
                sorted = false;
                if(buffer != NULL){
                    SHOW;
                    drawNumbers(buffer, numbers, numbersSize, numbersWidth, i, i + 1);
                    wait();
                }
            }
        }

        for(int i = 1; i < numbersSize - 2; i += 2){
            if(numbers[i + 1] < numbers[i]){
                swap(&numbers[i], &numbers[i + 1]);
                sorted = false;
                if(buffer != NULL){
                    SHOW;
                    drawNumbers(buffer, numbers, numbersSize, numbersWidth, i, i + 1);
                    wait();
                }
            }
        }
    }
}
