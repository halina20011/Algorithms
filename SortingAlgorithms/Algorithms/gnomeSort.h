#include "../func.h"

#define GNOMESORT {"Gnome Sort", &gnomeSort}

void gnomeSort(uint8_t **buffer, int *numbers){
    int i = 0;

    while(i < numbersSize){
        if(i == 0 || numbers[i - 1] <= numbers[i]){
            i++;
        }
        else{            
            swap(&numbers[i], &numbers[i - 1]);
            i--;
        }
        if(buffer != NULL){
            SHOW;
            drawNumbers(buffer, numbers, numbersSize, numbersWidth, i - 1, i);
            wait();
        }
    }
}
