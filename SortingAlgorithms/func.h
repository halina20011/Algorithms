#ifndef FUNC
#define FUNC

#include <stdio.h>
#include <stdbool.h>
// for time
#include <time.h>

#include "vertex.h"

extern struct Vertex *v;

// extern struct Number *numbers;
// extern size_t numbersSize;

#ifndef WINDOW_WIDTH 
#define WINDOW_WIDTH 800
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 600
#endif

struct Changes{
    size_t *val;
    size_t size;
};

extern struct Changes changes;

// void fillArrays(struct Numbers *numbers, int *array, int arraySize, double increase);
// void shuffleArray(struct Numbers *numbers);
// void divisotors(int *array, int number, int *size);
// int searchInArray(int *array, int arraySize, int target);
// void printArray(int *array, int arraySize);
// void drawNumbers();
// void highlightRegion(int left, int right);
// void drawFinalAnimation();

// void update();

// void swapNumbers(int i, int j);
// void highlight(int index);

#endif
