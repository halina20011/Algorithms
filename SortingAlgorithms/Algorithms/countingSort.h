#include "../func.h"

#define COUNTINGSORT {"Counting Sort", &countingSort}

void countingSort(uint8_t *buffer, int *numbers, int numbersSize){
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
        if(buffer != NULL){
            ProcessEvents();
            drawNumbers(i, i);
            highlight(minIndex);
            highlight(maxIndex);
            update(buffer);
            wait();
        }
    }

    int size = max - min + 1;

    int *count = calloc(size, sizeof(int));
    for(int i = 0; i < numbersSize; i++){
        count[numbers[i] - min] += 1;
        if(buffer != NULL){
            ProcessEvents();
            drawNumbers(i, i);
            update(buffer);
            wait();
        }
    }
    
    int index = 0;
    for(int c = 0; c < size; c++){
        for(int i = 0; i < count[c]; i++){
            numbers[index] = c + min;
            if(buffer != NULL){
                ProcessEvents();
                drawNumbers(index, index);
                update(buffer);
                wait();
            }
            index++;
        }
    }

    free(count);
}
