#include "algorithms.h"

void cocktailSort(int *numbers, int numbersSize){
    bool sorted = false;
    int start = 0, end = numbersSize - 1;

    while(!sorted){
		sorted = true;
		for(int i = start; i < end; i++){
            drawNumbers();
            highlightRegion(start, end);
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            highlight(i + 1);
            PixelWait();

            if(numbers[i + 1] < numbers[i]){
                swapNumbers(i, i + 1);
                sorted = false;
            }
		}
		end--;

		if(sorted){
            break;
        }

		for(int i = end; i > start; i--){
            drawNumbers();
            highlightRegion(start, end);
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            highlight(i - 1);
            PixelWait();

            if(numbers[i] < numbers[i - 1]){
                swapNumbers(i, i - 1);
                sorted = false;
            }
		}

		start++;
	}
}
