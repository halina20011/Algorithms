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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// #define CAPTURE_ON true
#include "../pixel.h"
#include "../pngWrapper.c"

struct Pixel *p = NULL;

int *availableSizes = NULL;
int availableSizesSize;

int *arr = NULL;
int arrSize = -1;

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
#include "singlyLinkedList.h"

// include all sorting algorithms 
#include "Algorithms/algorithms.h"

struct Algorithm algorithms[] = {
    BUBBLESORT, INSERTIONSORT, SELECTIONSORT, ODDEVENSORT, COCKTAILSORT, COMBSORT, CIRCLESORT, CYCLESORT, GNOMESORT, RADIXSORT, HEAPSORT, MERGESORT, MERGESORTNOSPACE, QUICKSORTLOMUTO, QUICKSORTHOARE, COUNTINGSORT, STOOGESORT, BOGOSORT, SLOWSORT
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

int *parseInput(char *input, int *arraySize){
    char *iPtr = input;

    int size = 0;
    struct List *list = newSinglyLinkedList();

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
                listPush(list, &n);
                size++;
                n = 0;
                numberToParse = false;
            }
        }
        iPtr++;
    }

    int *array = malloc(sizeof(int) * size);
    *arraySize = size;

    struct Node *curr = list->head;
    for(int i = 0; curr; i++){
        array[i] = *(int*)curr->val;
        curr = curr->next;
    }

    if(searchInArray(availableSizes, availableSizesSize, size) == -1){
        fprintf(stderr, "sorry but size '%i' of custom input is unvailable, run '-ls' to show all available sizes\n", size);
        exit(1);
    }

    return array;
}

int main(int argc, char **argv){
    bool shuffle = true;
    bool customInput = false;
    arrSize = -1;
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
            if(!sscanf(argv[i + 1], "%i", &arrSize)){
                fprintf(stderr, "invalid arr size\n");
                return 1;
            }
            if(searchInArray(availableSizes, availableSizesSize, arrSize) == -1){
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
            arr = parseInput(argv[i + 1], &arrSize);
            i++;
        }
        else if(strcmp(flag, "-N") == 0){
            if(argc <= i + 1){
                fprintf(stderr, "missing option after flag '%s'\n", flag);
                return 1;
            }
            customInput = true;
            arr = parseInput(argv[i + 1], &arrSize);
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
        if(arrSize <= 0){
            arrSize = 100;
        }
        arr = malloc(sizeof(int) * arrSize);
        if(arr == NULL){
            fprintf(stderr, "error allocating memory for arr buffer\n");
            return 1;
        }
        for(int i = 0; i < arrSize; i++){
            arr[i] = i + 1;
        }
    }

    if(shuffle){
        shuffleArray(arr, arrSize);
    }

    int max = 0;
    for(int i = 0; i < arrSize; i++){
        max = (max < arr[i]) ? arr[i] : max;
    }
    
    numberWidth = WINDOW_WIDTH / arrSize;

    const double increase = (double)WINDOW_HEIGHT / (double)max;
    fillArray(arr, arrSize, increase);

    printf("algorithm: \"%s\"\n", algorithms[runAlgorithmIndex].name);

    p = pixelInit("Sorting Algorithms", WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if(initialSpeed != -1){
        pixelSetSpeed(p, initialSpeed);
    }

    void (*algorithm)(int *arr, int arrSize) = algorithms[runAlgorithmIndex].algorithm;
    
    algorithm(arr, arrSize);

    drawFinalAnimation(arr, arrSize);

    pixelWaitExit(p);
    
    free(arr);
    pixelFree(p);

    return 0;
}
