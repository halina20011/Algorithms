#include <stdbool.h>

#include "../func.h"

#define MERGESORT {"Merge Sort", mergeSortSpace}
#define MERGESORTNOSPACE {"Merge Sort, O(1) space", mergeSortNoSpace}

int min(int a, int b){
    return (a < b) ? a : b;
}

void merge(uint8_t **buffer, int *numbers, int from, int mid, int right){
    int i = from;
    int j = mid;
    while(i < mid && j < right){
        if(numbers[i] < numbers[j]){
            if(buffer != NULL){
                SHOW;
                drawNumbers(buffer, numbers, numbersSize, numbersWidth, i, j);
                wait();
            }
            i++;
        }
        else{
            int t = numbers[j];
            for(int x = j; x > i; x--){
                numbers[x] = numbers[x - 1];
                if(buffer != NULL){
                    SHOW;
                    drawNumbers(buffer, numbers, numbersSize, numbersWidth, x, x - 1);
                    wait();
                }
            }
            numbers[i++] = t;
            j++;
            mid++;
        }
    }
}

void mergeWithSpace(uint8_t **buffer, int *numbers, int *temp, int left, int mid, int right){
    int i = left;
    int j = mid;
    int tSize = 0;
    while(i < mid && j < right){
        if(numbers[i] < numbers[j]){
            temp[tSize++] = numbers[i++];
        }
        else{
            temp[tSize++] = numbers[j++];
        }
    }

    // copy rest to the temp array
    while(i < mid){
        temp[tSize++] = numbers[i++];
    }
    while(j < right){
        temp[tSize++] = numbers[j++];
    }

    // copy back
    for(int x = 0; x < tSize; x++){
        numbers[x + left] = temp[x];
        if(buffer != NULL){
            SHOW;
            drawNumbers(buffer, numbers, numbersSize, numbersWidth, x + left, -1);
            wait();
        }
    }
}

// bottom up
void mergeSort(uint8_t **buffer, int *numbers, bool space){
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
            int mid = i + w;
            int right = min(i + w * 2, numbersSize);
            
            if(space){
                mergeWithSpace(buffer, numbers, temp, left, mid, right);
            }
            else{
                merge(buffer, numbers, left, mid, right);
            }
        }
    }
    free(temp);
}

void mergeSortSpace(uint8_t **buffer, int *numbers){
    mergeSort(buffer, numbers, true);
}

void mergeSortNoSpace(uint8_t **buffer, int *numbers){
    mergeSort(buffer, numbers, false);
}
