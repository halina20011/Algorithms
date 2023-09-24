#include "../../pixel.h"
#include "../func.h"

#define GNOMESORT {"Gnome Sort", &gnomeSort}

extern struct Pixel *p;

void gnomeSort(int *numbers, int numbersSize){
    int i = 0;

    while(i < numbersSize){
        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(i - 1);
        highlight(i);
        PixelWait();

        if(i == 0 || numbers[i - 1] <= numbers[i]){
            i++;
        }
        else{
            swapNumbers(i, i - 1);
            i--;
        }
    }

    drawFinalAnimation(numbers, numbersSize);
}
