#include "../func.h"

#define BOGOSORT {"Bogosort", &bogosort}

void bogosort(uint8_t **buffer, int *numbers){
    bool sorted = false;
    // If array isn't yet sorted
    while(!sorted){
        sorted = true;
        shuffle(numbers, numbersSize);
        for(int i = 0; i < numbersSize - 1; i++){
            if(numbers[i + 1] < numbers[i]){
                sorted = false;
                break;
            }
            if(buffer != NULL){
                SHOW;
                drawNumbers(buffer, numbers, numbersSize, numbersWidth, i, i + 1);
                wait();
            }
        }
    }
}
