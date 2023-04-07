#ifndef DRAW
#define DRAW

#include "../pixel.c"

extern const int WINDOWWIDTH;
extern const int WINDOWHEIGHT;

extern int indexAnimation;

int drawFinalAnimation(uint8_t **buffer, int *numbers, int length, int width){
    setColor(0, 0, 0, 255);
    fillBuffer(buffer);
    setColor(255, 255, 255, 255);

    for(int i = 0; i < length; i++){
        int x = i * width;
        if(i <= indexAnimation){
            setColor(0, 255, 0, 255);
            drawRectangle(buffer, x, WINDOWHEIGHT - numbers[i], width, numbers[i]);
            setColor(255, 255, 255, 255);
        }
        else{
            drawRectangle(buffer, x, WINDOWHEIGHT - numbers[i], width, numbers[i]);
        }
    }
    
    update(*buffer);
    
    return 0;
}

int drawNumbers(uint8_t **buffer, int *numbers, int length, int width, int indexItem1, int indexItem2){
    setColor(0, 0, 0, 255);
    fillBuffer(buffer);
    setColor(255, 255, 255, 255);

    for(int i = 0; i < length; i++){
        int x = i * width;
        if(indexItem1 == i || indexItem2 == i){
            setColor(255, 0, 0, 255);
            drawRectangle(buffer, x, WINDOWHEIGHT - numbers[i], width, numbers[i]);
            setColor(255, 255, 255, 255);
        }
        else{
            drawRectangle(buffer, x, WINDOWHEIGHT - numbers[i], width, numbers[i]);
        }
    }

    update(*buffer);

    return 0;
}

#endif
