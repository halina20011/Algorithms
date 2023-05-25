#include "../draw.c"
#include "../func.h"

#define GNOMESORT {"Gnome Sort", &gnomeSort}

void gnomeSort(uint8_t **buffer, int *numbers){
    int i = 0;

    while(i < numbersSize){
        if(i == -1){
            i += 1;
        }

        // correct order => one number forward
        if(numbers[i] < numbers[i + 1]){
            i += 1;
        }
        else{ // swap them and go one step backwards
            swap(&numbers[i], &numbers[i + 1]);
            i -= 1;
            if(buffer != NULL){
                SHOW;
                drawNumbers(buffer, numbers, numbersSize, numbersWidth, i, i + 1);
                wait();
            }
        }
    }
}
