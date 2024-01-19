#ifndef FUNC
#define FUNC

#include <stdio.h>
#include <stdbool.h>
// for time
#include <time.h>

#include "../pixel.h"

extern struct Pixel *p;

extern int numberWidth;
extern int *arr;
extern int arrSize;
extern bool HIGHLIGHT;

#ifndef WINDOW_WIDTH 
#define WINDOW_WIDTH 800
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 600
#endif

void fillArray(int *array, int arraySize, double increase);
void shuffleArray(int *array, int length);
void divisotors(int *array, int number, int *size);
int searchInArray(int *array, int arraySize, int target);
void printArray(int *array, int arraySize);
void drawNumbers();
void highlight(int index);
void swapNumbers(int i, int j);
void highlightRegion(int left, int right);
void drawFinalAnimation();

#endif
