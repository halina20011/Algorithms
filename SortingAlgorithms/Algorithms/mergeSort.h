#include <stdbool.h>
#include <stdio.h>

#include "../../pixel.h"
#include "../func.h"

#define MERGESORT {"Merge Sort", mergeSortSpace}
#define MERGESORTNOSPACE {"Merge Sort, O(1) space", mergeSortNoSpace}

extern struct Pixel *p;

int min(int a, int b){
    return (a < b) ? a : b;
}

void merge(int *numbers, int numbersSize, int from, int mid, int right){
    int i = from;
    int j = mid;
    while(i < mid && j < right){
        if(numbers[i] < numbers[j]){
            drawNumbers();
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            highlight(j);
            PixelWait();
            i++;
        }
        else{
            int t = numbers[j];
            for(int x = j; x > i; x--){
                drawNumbers();
                pixelSetColor(p, 255, 0, 0, 255);
                highlight(x);
                highlight(x - 1);
                PixelWait();

                numbers[x] = numbers[x - 1];
            }

            numbers[i++] = t;
            j++;
            mid++;
        }
    }
}

void mergeWithSpace(int *numbers, int numbersSize, int *temp, int left, int mid, int right){
    int i = left;
    int j = mid;
    int tSize = 0;
    while(i < mid && j < right){
        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(j);
        highlight(i);
        PixelWait();
        if(numbers[i] < numbers[j]){
            temp[tSize++] = numbers[i++];
        }
        else{
            temp[tSize++] = numbers[j++];
        }
    }

    // copy rest to the temp array
    while(i < mid){
        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(i);
        PixelWait();
        temp[tSize++] = numbers[i++];
    }

    while(j < right){
        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(j);
        PixelWait();
        temp[tSize++] = numbers[j++];
    }

    // copy back
    for(int x = 0; x < tSize; x++){
        numbers[x + left] = temp[x];

        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(x + left);
        PixelWait();
    }
}

// bottom up
void mergeSort(int *numbers, int numbersSize, bool space){
    int *temp = NULL;
    if(space){
       temp = malloc(sizeof(int) * numbersSize);
    }
    // [2, 4, 8, 16, ...]
    for(int w = 1; w < numbersSize; w = 2 * w){
        // 1 [0, 2, 4, 6, ...]
        // 2 [0, 4, 8, 12, ...]
        // 4 [0, 8, 16, 24, ...]
        for(int i = 0; i < numbersSize; i += w * 2){
            int left = i;
            int mid = min(i + w, numbersSize);
            int right = min(i + w * 2, numbersSize);

            if(mid < right){
                if(space){
                    mergeWithSpace(numbers, numbersSize, temp, left, mid, right);
                }
                else{
                    merge(numbers, numbersSize, left, mid, right);
                }
            }
        }
    }

    free(temp);
    drawFinalAnimation(numbers, numbersSize);
}

void mergeSortSpace(int *numbers, int numbersSize){
    mergeSort(numbers, numbersSize, true);
}

void mergeSortNoSpace(int *numbers, int numbersSize){
    mergeSort(numbers, numbersSize, false);
}
