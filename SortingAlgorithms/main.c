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
#include <ctype.h> // isdigit
#include <string.h> // strcmp

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// #define CAPTURE_ON true
// #include "../pixel.h"
// #include "../pngWrapper.c"

#include "vertex.h"
struct Vertex *v = NULL;

struct Number **numbers = NULL;
size_t numbersSize;

struct Changes changes;

bool HIGHLIGHT = false;

int capture = 0;
// int capture = 2;
uint16_t captureIndex = 0;

bool ended = 0;
int indexAnimation = 0;

size_t bufferSize = 0;
float *buffer = NULL;

float WIDTH;

struct Algorithm{
    const char *name;
    const void *algorithm;
};

// #include "func.h"
#include "singlyLinkedList.h"

// include all sorting algorithms 
#include "Algorithms/algorithms.h"

struct Algorithm algorithms[] = {BUBBLESORT};
// struct Algorithm algorithms[] = {
//     BUBBLESORT, INSERTIONSORT, SELECTIONSORT, ODDEVENSORT, COCKTAILSORT, COMBSORT, CIRCLESORT, CYCLESORT, GNOMESORT, RADIXSORT, HEAPSORT, MERGESORT, MERGESORTNOSPACE, QUICKSORTLOMUTO, QUICKSORTHOARE, COUNTINGSORT, STOOGESORT, BOGOSORT, SLOWSORT
// };
//
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

// void getAvailableSizes(){
//     // divisotors(NULL, WINDOW_WIDTH, &availableSizesSize);
//     availableSizes = malloc(sizeof(int) * availableSizesSize);
//     divisotors(availableSizes, WINDOW_WIDTH, &availableSizesSize);
//
//     qsort(availableSizes, availableSizesSize, sizeof(int), comp);
// }

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

    // for(int i = 0; i < sortingAlgorithmsLength; i++){
    //     printf("%5d\t\t%s\n", i, algorithms[i].name);
    // }
}

void shuffleArray(int *arr, size_t size){
    unsigned seed = time(NULL);
    srand(seed);

    for(size_t i = 0; i < size; i++){
        size_t newIndex = rand() % size;
        if(i != newIndex){
            int temp = arr[newIndex];
            arr[newIndex] = arr[i];
            arr[i] = temp;
        }
    }
}

int *parseInput(char *input, size_t *arraySize){
    char *iPtr = input;

    size_t size = 0;
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
                listPushConvert(list, int, n);
                size++;
                n = 0;
                numberToParse = false;
            }
        }
        iPtr++;
    }

    int *array = malloc(sizeof(int) * size);
    *arraySize = size;

    struct Node *curr = list->head, *temp = NULL;
    for(int i = 0; curr; i++){
        temp = curr;
        
        array[i] = *((int*)curr->val);
        curr = curr->next;

        free(temp->val);
        free(temp);
    }

    free(list);

    // if(searchInArray(availableSizes, availableSizesSize, size) == -1){
    //     fprintf(stderr, "sorry but size '%i' of custom input is unvailable, run '-ls' to show all available sizes\n", size);
    //     exit(1);
    // }

    return array;
}

void bufferSet(struct Number *number, float *data, size_t *index){
    size_t pointer = *index;
    for(size_t i = 0; i < VERTEX_DATA; i++){
        number->buffer[pointer++] = data[i];
    }

    *index = (pointer + VERTEX_COLORS);
}

int main(int argc, char **argv){
    bool shuffle = true;
    bool customInput = false;
    int runAlgorithmIndex = -1;
    int initialSpeed = -1;
    

    int *arr = NULL;
    size_t arrSize = 0;

    // if(argc == 1){
    //     showOptions();
    //     return 1;
    // }
    
    // getAvailableSizes();

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
        // else if(strcmp(flag, "-ls") == 0){
        //     printArray(availableSizes, availableSizesSize);
        //     return 0;
        // }
        else if(strcmp(flag, "-i") == 0){
            if(argc <= i + 1){
                fprintf(stderr, "missing option after flag '%s'\n", flag);
                return 1;
            }
            if(!sscanf(argv[i + 1], "%i", &runAlgorithmIndex)){
                fprintf(stderr, "invalid algorithm index\n");
                return 1;
            }
            // if(runAlgorithmIndex < -2 || sortingAlgorithmsLength <= runAlgorithmIndex){
            //     fprintf(stderr, "valid range is <-1; %i)\n", sortingAlgorithmsLength);
            //     return 1;
            // }
            i++;
        }
        else if(strcmp(flag, "-s") == 0){
            if(argc <= i + 1){
                fprintf(stderr, "missing option after flag '%s'\n", flag);
                return 1;
            }
            int s = 0;
            if(!sscanf(argv[i + 1], "%i", &s)){
                fprintf(stderr, "invalid arr size\n");
                return 1;
            }
            arrSize = s;
            // if(searchInArray(availableSizes, availableSizesSize, arrSize) == -1){
            //     fprintf(stderr, "this size is unvailable run '-ls' to show all available sizes\n");
            //     return 1;
            // }
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
    
    printf("arrsize: %zu\n", arrSize);
    if(!customInput){
        if(arrSize <= 0){
            arrSize = 50;
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

    int max = INT_MIN; 
    // int min = INT_MAX;
    for(int i = 0; i < arrSize; i++){
        max = (max < arr[i]) ? arr[i] : max;
    }
    
    WIDTH = 2.0 / (float)arrSize;

    const double increase = 2.0 / (double)max;
    printf("increase %f WIDTH %f\n", increase, WIDTH);

    v = vertexInit("Sorting Algorithms", WIDTH);
    numbersSize = arrSize;
    printf("size: %zu\n", numbersSize);
    numbers = malloc(sizeof(struct Number*) * numbersSize);
    bufferSize = numbersSize * BUFFER_BYTES_SIZE;
    buffer = malloc(bufferSize);

    printf("filling...\n");

    if(shuffle){
        shuffleArray(arr, arrSize);
    }

    for(size_t i = 0; i < arrSize; i++){
        printf("%i ", arr[i]);
    }
    printf("\n");

    // p1    p3
    // +-----+
    // |     |
    // |     |
    // +-----+
    // p2    p4
    float wP = -1;
    for(size_t i = 0; i < numbersSize; i++){
        float h = (float)increase * (float)(arr[i] - 1);
        float ph = 1. - (2. - h);
        size_t index = 0;
        struct Number *number = malloc(sizeof(struct Number));
        // number->buffer
        number->val = arr[i];
        // printf("%i\n", i);
        // float ph = 1;
        float p1[] = {wP          , ph, 0};
        float p2[] = {wP          , -1, 0};
        float p3[] = {wP + WIDTH  , ph, 0};
        float p4[] = {wP + WIDTH  , -1, 0};
        bufferSet(number, p1, &index);
        bufferSet(number, p2, &index);
        bufferSet(number, p3, &index);

        bufferSet(number, p3, &index);
        bufferSet(number, p4, &index);
        bufferSet(number, p2, &index);
        memcpy(&buffer[i * BUFFER_SIZE], &number->buffer, BUFFER_BYTES_SIZE);
        // float *p[] = {number->p1, number->p2, number->p3, number->p4};
        // for(size_t pi = 0; pi < 4; pi++){
        //     float *pp = p[pi];
        //     // struct Point *pp = p[pi];
        //     printf("%i %i => %f %f %f\n", i, pi, pp[0], pp[1], pp[2]);
        // }
        // for(size_t j = 0; j < 4; j++){
        //     for(size_t x = 0; x < 3; x++){
        //         printf("%f ", buffer[i * 4 * VERTEX_SIZE + (j * 3 + x)]);
        //     }
        //     printf("\n");
        // }
        // memcpy(&buffer[i * 4 * VERTEX_SIZE + 0], &number->p1, sizeof(float) * VERTEX_SIZE);
        // memcpy(&buffer[i * 4 * VERTEX_SIZE + 1], &number->p2, sizeof(float) * VERTEX_SIZE);
        // memcpy(&buffer[i * 4 * VERTEX_SIZE + 2], &number->p3, sizeof(float) * VERTEX_SIZE);
        // memcpy(&buffer[i * 4 * VERTEX_SIZE + 3], &number->p4, sizeof(float) * VERTEX_SIZE);
        // printf("%f %f %f %f\n", number->p1, numbers->p2, numbers->p3, numbers->p4);
        wP += WIDTH;
        numbers[i] = number;
    }

    // updateBuffer();
    glBufferData(GL_ARRAY_BUFFER, bufferSize, buffer, GL_DYNAMIC_DRAW);
    
    // float bbf[] = {
    //     -0.5, 0.5, 0,
    //     -0.5,-0.5, 0,
    //     0.5, 0.5, 0,
    //     0.5, -0.5, 0,
    // };

    // printf("%zu\n", bufferSize);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(bbf), bbf, GL_DYNAMIC_DRAW);

    // float buffer[] = {
    //     0, 0, 1,
    //     0, 1, 1,
    //     1, 0.5, 1,
    // };
    // printf("filled\n");


    // while(1){
    //     // glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
    //     // glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), numbers, GL_DYNAMIC_DRAW);
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     glDrawArrays(GL_TRIANGLES, 0, numbersSize * 4 * 2);
    //     // glDrawArrays(GL_TRIANGLES, 10, 4);
    //     // glDrawArrays(GL_TRIANGLE_STRIP, 100, 4);
    //     glfwSwapBuffers(v->window);
    //     // printf("swap\n");
    // }

    // glfwSwapInterval(0);
    // for(int i = 0; i < numbersSize; i++){
    //     for(int j = i ; j < numbersSize; j++){
    //         // printf("%i %i\n", i, j);
    //         // glBufferData(GL_ARRAY_BUFFER, sizeof(struct Number) * numbersSize, numbers, GL_DYNAMIC_DRAW);
    //         glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), numbers, GL_DYNAMIC_DRAW);
    //         glClear(GL_COLOR_BUFFER_BIT);
    //         glDrawArrays(GL_TRIANGLE_STRIP, 0, 4 * numbersSize / 2);
    //         // for(int i = 0; i < numbersSize; i++){
    //         //     glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
    //         // }
    //         struct Number temp = numbers[i];
    //         numbers[i] = numbers[j];
    //         numbers[j] = temp;
    //         glfwSwapBuffers(v->window);
    //         // sleepFunc(1);
    //     }
    //     // printf("drawn\n");
    // }
    // while(1){
    // }
    // free(arr);

    // printf("algorithm: \"%s\"\n", algorithms[runAlgorithmIndex].name);
    
    // changes.val = malloc(sizeof(size_t) * arrSize);
    // changes.size = 0;

    // v = vertexInit("Sorting Algorithms", WIDTH);
    
    // if(initialSpeed != -1){
    //     vertexSetSpeed(v, initialSpeed);
    // }
    
    // vertexUpdate(v);
    // update();
    // while(v->run){
    //     glfwPollEvents();
    // }
    
    void (*algorithm)() = algorithms[runAlgorithmIndex].algorithm;
    algorithm();

    // drawFinalAnimation();
    //
    // vertexWaitExit();

    vertexFree();

    return 0;
}
