#include <stdbool.h>

#include "../../pixel.h"
#include "../func.h"

#define CYCLESORT {"Cycle Sort", &cycleSort}

void move(int *numbers, int size, int *position){
    int pos = *position;
    int org = pos;
    int item = numbers[pos];
    for(int i = *position + 1; i < size; i++){
        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(pos);
        highlight(i);
        PixelWait();

        if(numbers[i] < item){
            pos++;
            drawNumbers();
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(pos);
            highlight(i);
            PixelWait();
        }
    }
    *position = pos;
}

void skipDup(int *numbers, int size, int *position){
    while(numbers[*position] == numbers[*position + 1]){
        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(*position);
        PixelWait();
        *position += 1;
    }
}

void cycleSort(int *numbers, int size){
    int writes = 0;
    for(int i = 0; i < size - 1; i++){
        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(i);
        PixelWait();

        int item = numbers[i];
        int position = i;
        move(numbers, size, &position);
        if(position == i){
            continue;
        }
        skipDup(numbers, size, &position);

        if(position != i){
            drawNumbers();
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            swapNumbers(i, position);
            writes++;
            PixelWait();
        }

        while(position != i){
            position = i;
            move(numbers, size, &position);
            skipDup(numbers, size, &position);
            if(item != numbers[position]){
                drawNumbers();
                pixelSetColor(p, 255, 0, 0, 255);
                highlight(i);
                swapNumbers(i, position);
                writes++;
                PixelWait();
            }
        }
    }
    printf("writes: %i\n", writes);

    drawFinalAnimation();
}
