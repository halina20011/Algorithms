#include "algorithms.h"

bool halver(int *numbers, int left, int right){
    bool swapped = false;

    while(left < right){
        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(left);
        highlight(right);
        PixelWait();

        if(numbers[right] < numbers[left]){
            swapNumbers(left, right);
            PixelWait();

            swapped = true;
        }

        left += 1;
        right -= 1;
    }
    
    // specific case for odd size
    if(left == right){
        if(numbers[right + 1] < numbers[left]){
            drawNumbers();
            swapNumbers(left, right + 1);
            PixelWait();

            swapped = true;
        }
    }

    return swapped;
}

bool circlePass(int *numbers, int left, int right){
    if(right <= left){
        return false;
    }

    const int mid = left + (right - left) / 2;

    bool swapped = halver(numbers, left, right);
    bool leftSwapped = circlePass(numbers, left, mid);
    bool rightSwapped = circlePass(numbers, mid + 1, right);

    return (swapped || leftSwapped || rightSwapped);
}

void circleSort(int *numbers, int size){
    while(circlePass(numbers, 0, size - 1)){}

    drawFinalAnimation(numbers, size);
}
