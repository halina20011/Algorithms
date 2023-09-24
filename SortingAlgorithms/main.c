// gcc main.c $(sdl2-config --cflags --libs) -lm -lpng -o Build/main && ./Build/main [indexAnimation]
// ./build.sh && ./Build/main [indexAnimation]

// Copyright (C) 2023  halina20011
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdbool.h>
// for usleep()
#include <unistd.h>
// for PRIu32 macro
#include <inttypes.h>
#include <math.h>

// #define CAPTURE_ON true
#include "../pixel.h"
#include "../pngWrapper.c"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Pixel *p = NULL;

int *availableSizes = NULL;
int availableSizesSize;

int *numbers = NULL;
int numbersSize = -1;

int numberWidth = 0;

bool HIGHLIGHT = false;

int capture = 0;
// int capture = 2;
uint16_t captureIndex = 0;

bool ended = 0;
int indexAnimation = 0;

struct Algorithm{
    const char *name;
    const void *algorithm;
};

#include "func.h"
#include "singleLinkedList.h"

// include all sorting algorithms 
#include "Algorithms/bubbleSort.h"
#include "Algorithms/insertionSort.h"
#include "Algorithms/selectionSort.h"
#include "Algorithms/oddevenSort.h"
#include "Algorithms/cocktailSort.h"
#include "Algorithms/gnomeSort.h"

#include "Algorithms/radixSort.h"
#include "Algorithms/heapsort.h"
#include "Algorithms/mergeSort.h"
#include "Algorithms/countingSort.h"

#include "Algorithms/stoogeSort.h"
#include "Algorithms/bogosort.h"
#include "Algorithms/slowsort.h"

struct Algorithm algorithms[] = {
    BUBBLESORT, INSERTIONSORT, SELECTIONSORT,ODDEVENSORT, COCKTAILSORT, GNOMESORT, RADIXSORT, HEAPSORT, MERGESORT, MERGESORTNOSPACE, COUNTINGSORT, STOOGESORT, BOGOSORT, SLOWSORT
};

int sortingAlgorithmsLength = sizeof(algorithms) / sizeof(algorithms[0]);

#define optionsSize 10

const char options[optionsSize][2][50] = {
    {"-h", "show this help message"},
    {"-i", "select algorithm to run with this index"},
    {"-s", "size of array"},
    {"-ls", "list available sizes"},
    {"-l", "list all available algorithms"},
    {"-n", "custom input to sort e.g '[5,3,1,2]'"},
    {"-N", "custom input to shuffle and then sort"},
    {"-r", "show region if available"},
    {"--slow", "start in the slowest speed"},
    {"--fast", "start in the fastest speed"}
};

int comp(const void *a, const void *b){
    const int x = *(int*)a;
    const int y = *(int*)b;

    return (y < x) - (x < y);
}

void getAvailableSizes(){
    divisotors(NULL, WINDOW_WIDTH, &availableSizesSize);
    availableSizes = malloc(sizeof(int) * availableSizesSize);
    divisotors(availableSizes, WINDOW_WIDTH, &availableSizesSize);

    qsort(availableSizes, availableSizesSize, sizeof(int), comp);
}

void showOptions(){
    printf("please select correct algorithm to run\n");
    printf("usage: [flag] [option]\n");
    for(int i = 0; i < optionsSize; i++){
        printf("  %s \t\t %s\n", options[i][0], options[i][1]);
    }
}

void showAlgorithms(){
    printf("index\t\talgorithm name\n");
    printf("   -1\t\t[run the last algorithm]\n");

    for(int i = 0; i < sortingAlgorithmsLength; i++){
        printf("%5d\t\t%s\n", i, algorithms[i].name);
    }
}

int parseInput(char *input, int **array, int *arraySize){
    char *iPtr = input;

    int size = 0;
    struct Node *head = NULL;
    struct Node *tail = NULL;

    int n = 0;
    int numberToParse = false;
    while(*iPtr != '\0'){
        if(isdigit(*iPtr)){
            n *= 10;
            int d = *iPtr - '0';
            n += d;
            numberToParse = true;
        }

        if(!isdigit(*iPtr) || *(iPtr + 1) == '\0'){
            if(numberToParse){
                add(&head, &tail, n);
                size++;
                n = 0;
                numberToParse = false;
            }
        }
        iPtr++;
    }
    *array = malloc(sizeof(int) * size);
    *arraySize = size;

    for(int i = 0; head; i++){
        (*array)[i] = deleteFirst(&head);
    }
    
    if(searchInArray(availableSizes, availableSizesSize, size) == -1){
        fprintf(stderr, "sorry but size '%i' of custom input is unvailable, run '-ls' to show all available sizes\n", size);
        return 1;
    }

    return 0;
}

int main(int argc, char **argv){
    bool shuffle = true;
    bool customInput = false;
    numbersSize = -1;
    int runAlgorithmIndex = -1;
    int initialSpeed = -1;
    if(argc == 1){
        showOptions();
        return 1;
    }
    
    getAvailableSizes();

    for(int i = 1; i < argc; i++){
        char *flag = argv[i];
        if(strcmp(flag, "-h") == 0){
            showOptions();
            return 0;
        }
        else if(strcmp(flag, "-l") == 0){
            showAlgorithms();
            return 0;
        }
        else if(strcmp(flag, "-ls") == 0){
            printArray(availableSizes, availableSizesSize);
            return 0;
        }
        else if(strcmp(flag, "-i") == 0){
            if(argc <= i + 1){
                fprintf(stderr, "missing option after flag '%s'\n", flag);
                return 1;
            }
            if(!sscanf(argv[i + 1], "%i", &runAlgorithmIndex)){
                fprintf(stderr, "invalid algorithm index\n");
                return 1;
            }
            if(runAlgorithmIndex < -2 || sortingAlgorithmsLength <= runAlgorithmIndex){
                fprintf(stderr, "valid range is <-1; %i)\n", sortingAlgorithmsLength);
                return 1;
            }
            i++;
        }
        else if(strcmp(flag, "-s") == 0){
            if(argc <= i + 1){
                fprintf(stderr, "missing option after flag '%s'\n", flag);
                return 1;
            }
            if(!sscanf(argv[i + 1], "%i", &numbersSize)){
                fprintf(stderr, "invalid numbers size\n");
                return 1;
            }
            if(searchInArray(availableSizes, availableSizesSize, numbersSize) == -1){
                fprintf(stderr, "this size is unvailable run '-ls' to show all available sizes\n");
                return 1;
            }
            i++;
        }
        else if(strcmp(flag, "-n") == 0){
            if(argc <= i + 1){
                fprintf(stderr, "missing option after flag '%s'\n", flag);
                return 1;
            }
            customInput = true;
            shuffle = false;
            if(parseInput(argv[i + 1], &numbers, &numbersSize)){
                fprintf(stderr, "failed parsing of the input\n");
                return 0;
            }
            i++;
        }
        else if(strcmp(flag, "-N") == 0){
            if(argc <= i + 1){
                fprintf(stderr, "missing option after flag '%s'\n", flag);
                return 1;
            }
            customInput = true;
            if(parseInput(argv[i + 1], &numbers, &numbersSize)){
                fprintf(stderr, "failed parsing of the input\n");
                return 0;
            }
            i++;
        }
        else if(strcmp(flag, "-r") == 0){
            HIGHLIGHT = true;
        }
        else if(strcmp(flag, "--slow") == 0){
            initialSpeed = 0;
        }
        else if(strcmp(flag, "--fast") == 0){
            initialSpeed = 1;
        }
        else{
            fprintf(stderr, "Invalid option:\"%s\"\n", flag);
            fprintf(stderr, "Try: \"%s -h\" for more information\n", argv[0]);
            return 1;
        }
    }
    
    if(runAlgorithmIndex == -1){
        runAlgorithmIndex = sortingAlgorithmsLength - 1;
    }

    if(!customInput){
        if(numbersSize <= 0){
            numbersSize = 100;
        }
        numbers = malloc(sizeof(int) * numbersSize);
        if(numbers == NULL){
            fprintf(stderr, "error allocating memory for numbers buffer\n");
            return 1;
        }
        for(int i = 0; i < numbersSize; i++){
            numbers[i] = i + 1;
        }
    }

    if(shuffle){
        shuffleArray(numbers, numbersSize);
    }

    int max = 0;
    for(int i = 0; i < numbersSize; i++){
        max = (max < numbers[i]) ? numbers[i] : max;
    }
    
    numberWidth = WINDOW_WIDTH / numbersSize;

    const double increase = (double)WINDOW_HEIGHT / (double)max;
    fillArray(numbers, numbersSize, increase);

    printf("algorithm: \"%s\"\n", algorithms[runAlgorithmIndex].name);

    p = pixelInit("Sorting Algorithms", WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if(initialSpeed != -1){
        pixelSetSpeed(p, initialSpeed);
    }

    const void (*algorithm)(int *numbers, int numbersSize) = algorithms[runAlgorithmIndex].algorithm;

    // int *numbersCopy = malloc(sizeof(int) * numbersSize);
    // memcpy(numbersCopy, numbers, sizeof(int) * numbersSize);
    // algorithm(NULL, numbersCopy);
    // printArray(numbersCopy, );
    // free(numbersCopy);
    
    algorithm(numbers, numbersSize);

    pixelWaitExit(p);
    
    free(numbers);
    pixelFree(p);

    return 0;
}
