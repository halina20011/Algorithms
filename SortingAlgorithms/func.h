#ifndef FUNC
#define FUNC

#include <stdio.h>
#include <stdbool.h>
// for time
#include <time.h>

#include "../pixel.c"

extern int numberWidth;
extern uint8_t *buffer;
extern int *numbers;
extern int numbersSize;
extern bool HIGHLIGHT;

void fillArray(int *array, int arraySize, double increase){
    for(int i = 0; i < arraySize; i++){
        array[i] *= increase;
    }
}

void shuffleArray(int *array, int length){
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

void divisotors(int *array, int number, int *size){
    int s = 0;
    const int sqrtN = sqrt(number);
    for(int i = 2; i <= sqrtN; i++){
        if(WINDOW_WIDTH % i == 0){
            if(array){
                array[s] = i;
            }
            s++;
            if(i != sqrtN){
                if(array){
                    array[s] = number / i;
                }
                s++;
            }
        }
    }

    *size = s;
}

int searchInArray(int *array, int arraySize, int target){
    int left = 0;
    int right = arraySize;
    while(left <= right){
        const int m = left + (right - left) / 2;

        if(array[m] == target){
            return m;
        }
        else if(array[m] < target){
            left = m + 1;
        }
        else{
            right = m - 1;
        }
    }

    return -1;
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printArray(int *array, int arraySize){
    for(int i = 0; i < arraySize; i++){
        printf("%d ", array[i]);
    }

    printf("\n");
}

void drawNumbers(int indexItem1, int indexItem2){
    setColor(0, 0, 0, 255);
    fillBuffer(buffer);
    setColor(255, 255, 255, 255);

    for(int i = 0; i < numbersSize; i++){
        int x = i * numberWidth;
        if(indexItem1 == i || indexItem2 == i){
            setColor(255, 0, 0, 255);
            drawRectangle(&buffer, x, WINDOW_HEIGHT - numbers[i], numberWidth, numbers[i]);
            setColor(255, 255, 255, 255);
        }
        else{
            drawRectangle(&buffer, x, WINDOW_HEIGHT - numbers[i], numberWidth, numbers[i]);
        }
    }
}

void highlight(int index){
    if(0 <= index && index < numbersSize){
        int x = index * numberWidth;
        setColor(255, 0, 0, 255);
        drawRectangle(&buffer, x, WINDOW_HEIGHT - numbers[index], numberWidth, numbers[index]);
        setColor(255, 255, 255, 255);
    }
}

void highlightRegion(int left, int right){
    if(!HIGHLIGHT)
        return;
    setColor(0, 0, 255, 255);
    const int end = (right < numbersSize) ? right : numbersSize;
    for(int i = left; i < end; i++){
        int x = i * numberWidth;
        drawRectangle(&buffer, x, 0, numberWidth, WINDOW_HEIGHT - numbers[i]);
    }
    setColor(255, 255, 255, 255);
}

void drawFinalAnimation(){
    drawNumbers(-1, -1);
    setColor(0, 255, 0, 255);
    for(int i = 0; i < numbersSize; i++){
        int x = i * numberWidth;
        drawRectangle(&buffer, x, WINDOW_HEIGHT - numbers[i], numberWidth, numbers[i]);
        ProcessEvents();
        update(buffer);
        wait();
    }
    
    setColor(255, 255, 255, 255);
}

#endif
