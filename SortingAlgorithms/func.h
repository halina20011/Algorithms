#ifndef FUNC
#define FUNC

#include <stdio.h>
// for time
#include <time.h>

#include "../pixel.c"

void fillArray(int *array, int length, float increase){
    for(int i = 0; i < length; i++){
        array[i] = (i + 1) * increase;
    }
}

void shuffle(int *array, int length){
    unsigned seed = time(NULL);
    srand(seed);

    for(int i = 0; i < length; i++){
        int newIndex = rand() % length;
        if(i != newIndex){
            int temp = array[newIndex];
            // printf("Temp %d\n", temp);
            array[newIndex] = array[i];
            array[i] = temp;
        }
    }
}

void swap(int *x, int *y){
    int temp = *(x);
    *(x) = *(y);
    *(y) = temp;
}

void printArray(int *array, int length){
    for(int i = 0; i < length; i++){
        printf("%d ", array[i]);
    }

    printf("\n");
}

void drawNumbers(uint8_t **buffer, int *numbers, int length, int width, int indexItem1, int indexItem2){
    setColor(0, 0, 0, 255);
    fillBuffer(buffer);
    setColor(255, 255, 255, 255);

    for(int i = 0; i < length; i++){
        int x = i * width;
        if(indexItem1 == i || indexItem2 == i){
            setColor(255, 0, 0, 255);
            drawRectangle(buffer, x, WINDOW_HEIGHT - numbers[i], width, numbers[i]);
            setColor(255, 255, 255, 255);
        }
        else{
            drawRectangle(buffer, x, WINDOW_HEIGHT - numbers[i], width, numbers[i]);
        }
    }

    update(*buffer);
}

void drawFinalAnimation(uint8_t **buffer, int *numbers, int width, int length){
    drawNumbers(buffer, numbers, length, width, -1, -1);
    setColor(0, 255, 0, 255);
    for(int i = 0; i < length; i++){
        int x = i * width;
        drawRectangle(buffer, x, WINDOW_HEIGHT - numbers[i], width, numbers[i]);
        update(*buffer);
        SHOW;
        wait();
    }
    
    setColor(255, 255, 255, 255);
}

#endif
