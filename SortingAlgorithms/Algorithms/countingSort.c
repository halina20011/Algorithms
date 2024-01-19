#include "algorithms.h"

#define COUNTINGSORT {"Counting Sort", &countingSort}

void countingSort(int *numbers, int numbersSize){
    int min = INT_MAX, max = INT_MIN;
    int minIndex = 0, maxIndex = 0;
    for(int i = 0; i < numbersSize; i++){
        if(numbers[i] < min){
            min = numbers[i];
            minIndex = i;
        }
        if(max < numbers[i]){
            max = numbers[i];
            maxIndex = i;
        }

        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 0);
        highlight(i);
        pixelSetColor(p, 255, 255, 0, 0);
        highlight(minIndex);
        highlight(maxIndex);
        PixelWait();
    }

    int size = max - min + 1;

    int *count = calloc(size, sizeof(int));
    for(int i = 0; i < numbersSize; i++){
        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 0);
        highlight(i);
        PixelWait();

        count[numbers[i] - min] += 1;
    }
    
    int index = 0;
    for(int c = 0; c < size; c++){
        for(int i = 0; i < count[c]; i++){
            drawNumbers();
            pixelSetColor(p, 255, 0, 0, 0);
            highlight(index);
            PixelWait();

            numbers[index] = c + min;
            index++;
        }
    }

    free(count);
}
