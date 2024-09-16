#include "algorithms.h"

#define OFFSET 16

void reverse(int from, int to){
    for(int i = from; i < (to - from) / 2; i++){
        swapNumbers(from + i, to - i - 1);
    }
}

void rotate(int size, int from, int to){
    // 0123456789 size = 4
    // reverse          => 9876543210
    // split by k       => 9876|543210
    // reverse parts    => 6789|012345
    reverse(from, to);
    reverse(from, from + size);
    reverse(from + size, to);
}

int floorPowerOfTwo(int x){
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    // x = x | (x >> 32);
    return x - (x >> 1);
}

void blockSort(int *numbers, int numbersSize){
    // insertionSort(numbers, numbersSize);
    // rotate((double)numbersSize * 3.0/4.0, 0, numbersSize);
    int *temp = malloc(sizeof(int) * numbersSize);

    int powerOfTwo = floorPowerOfTwo(numbersSize);
    double scale = (double)numbersSize / (double)powerOfTwo;

    printf("powerOfTwo %i %g %g\n", powerOfTwo, scale, sqrt(numbersSize));

    for(int merge = 0; merge < powerOfTwo; merge += OFFSET){
        int start = (double)merge * scale;
        int size = ((double)OFFSET) * scale;
        insertionSort(numbers + start, size);
    }

    for(int length = OFFSET; length < powerOfTwo; length += length){
        for(int m = 0; m < powerOfTwo; m += length * 2){
            int start = m * scale;
            int mid = (m + length) * scale;
            int end = (m + length * 2) * scale;

            if(numbers[end - 1] < numbers[start]){
                rotate(mid - start, start, end);
            }
            else if(numbers[mid] < numbers[mid - 1]){
                mergeWithSpace(numbers, temp, start, mid, end);
            }
        }
    }

    free(temp);
}
