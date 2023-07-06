#include <stdbool.h>

#include "../func.h"

#define COCKTAILSORT {"Cocktail Sort", &cocktailSort}

void cocktailSort(uint8_t *buffer, int *numbers, int numbersSize){
    bool sorted = false;
    int start = 0;
	int end = numbersSize - 1;

    while(!sorted){
		sorted = true;
		for(int i = start; i < end; i++){
            if(numbers[i + 1] < numbers[i]){
                swap(&numbers[i], &numbers[i + 1]);
                sorted = false;
            }
            if(buffer != NULL){
                ProcessEvents()
                drawNumbers(i, i + 1);
                update(buffer);
                wait();
            }
		}
		end--;

		if(sorted){
            break;
        }

		for(int i = end; i > start; i--){
            if(numbers[i] < numbers[i - 1]){
                swap(&numbers[i], &numbers[i - 1]);
                sorted = false;
            }
            if(buffer != NULL){
                ProcessEvents()
                drawNumbers(i - 1, i);
                update(buffer);
                wait();
            }
		}
		start++;
	}
}
