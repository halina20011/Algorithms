#ifndef DRAW
#define DRAW

#include "../pixel.c"

void drawFinalAnimation(uint8_t **buffer, int *numbers, int width, int length){
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

#endif
