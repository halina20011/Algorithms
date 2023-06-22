#ifndef FUNC
#define FUNC

#include <stdio.h>
// for time
#include <time.h>

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

#endif
