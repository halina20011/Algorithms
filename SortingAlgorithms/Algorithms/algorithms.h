#ifndef SORTING_ALGO
#define SORTING_ALGO

#include <stdbool.h>
#include <limits.h>

#include "../../pixel.h"
#include "../func.h"
#include "../singlyLinkedList.h"

extern struct Pixel *p;

#define BLOCKSORT {"Block sort", &blockSort}
#define BOGOSORT {"Bogosort", &bogosort}
#define BUBBLESORT {"Bubble Sort", &bubbleSort}
#define CIRCLESORT {"Circle Sort", &circleSort}
#define COCKTAILSORT {"Cocktail Sort", &cocktailSort}
#define COMBSORT {"Comb sort", &combSort}
#define COUNTINGSORT {"Counting Sort", &countingSort}
#define CYCLESORT {"Cycle Sort", &cycleSort}
#define GNOMESORT {"Gnome Sort", &gnomeSort}
#define HEAPSORT {"Heapsort", &heapsort}
#define INSERTIONSORT {"Insertion Sort", &insertionSort}
#define MERGESORT {"Merge Sort", mergeSortSpace}
#define MERGESORTNOSPACE {"Merge Sort, O(1) space", mergeSortNoSpace}
#define ODDEVENSORT {"Odd-even Sort", &oddevenSort}
#define QUICKSORTLOMUTO {"Quicksort (Lomuto partition scheme)", &quicksortLomuto}
#define QUICKSORTHOARE {"Quicksort (Hoare partition scheme)", &quickSortHoare}
#define RADIXSORT {"Radix Sort", &radixSort}
#define SELECTIONSORT {"Selection Sort", &selectionSort}
#define SLOWSORT {"Slowsort", &slowsort}
#define STOOGESORT {"Stooge Sort", &stoogeSort}

void blockSort(int *numbers, int numbersSize);
void bogosort(int *numbers, int numbersSize);
void bubbleSort(int *numbers, int numbersSize);
void circleSort(int *numbers, int size);
void cocktailSort(int *numbers, int numbersSize);
void combSort(int *numbers, int size);
void countingSort(int *numbers, int numbersSize);
void cycleSort(int *numbers, int size);
void gnomeSort(int *numbers, int numbersSize);
void heapsort(int *numbers, int numbersSize);
void insertionSort(int *numbers, int numbersSize);
void mergeSortSpace(int *numbers, int numbersSize);
void mergeSortNoSpace(int *numbers, int numbersSize);
void oddevenSort(int *numbers, int numbersSize);
void quicksortLomuto(int *numbers, int size);
void quickSortHoare(int *numbers, int size);
void radixSort(int *numbers, int numbersSize);
void selectionSort(int *numbers, int numbersSize);
void slowsort(int *numbers, int numbersSize);
void stoogeSort(int *numbers, int numbersSize);

void mergeWithSpace(int *numbers, int *temp, int left, int mid, int right);

#endif
