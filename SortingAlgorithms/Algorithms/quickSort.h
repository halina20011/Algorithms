#include "../func.h"

#define QUICKSORTLOMUTO {"Quicksort (Lomuto partition scheme)", &quicksortLomuto}
#define QUICKSORTHOARE {"Quicksort (Hoare partition scheme)", &quickSortHoare}

// partition the array so the numbers less then the pivot 
// are on the left size
int partitionLomuto(int *numbers, int low, int high){
    int pivot = numbers[high];
    int i = low;

    for(int j = low; j < high; j++){
        drawNumbers();
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(i);
        highlight(j);
        PixelWait();

        if(numbers[j] <= pivot){
            drawNumbers();
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            highlight(j);
            swapNumbers(i++, j);
        }
    }

    drawNumbers();
    swapNumbers(i, high);
    return i;
}

void QuicksortLomuto(int *numbers, int low, int high){
    if(high <= low || low < 0){
        return;
    }

    int p = partitionLomuto(numbers, low, high);
    QuicksortLomuto(numbers, low, p - 1);
    QuicksortLomuto(numbers, p, high);
}

int partitionHoare(int *numbers, int low, int high){
    int pivot = numbers[low + (high - low) / 2];
    
    int i = low - 1;
    int j = high + 1;
    while(1){
        do{
            drawNumbers();
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            highlight(j);
            PixelWait();
            i++;
        } 
        while(numbers[i] < pivot);

        do{
            drawNumbers();
            pixelSetColor(p, 255, 0, 0, 255);
            highlight(i);
            highlight(j);
            PixelWait();
            j--;
        } 
        while(pivot < numbers[j]);

        if(j <= i){
            return j;
        }

        drawNumbers();
        swapNumbers(i, j);
    }

    return pivot;
}

void QuickSortHoare(int *numbers, int low, int high){
    if(high <= low || low < 0){
        return;
    }

    int p = partitionHoare(numbers, low, high);
    QuickSortHoare(numbers, low, p);
    QuickSortHoare(numbers, p + 1, high);
}

void quicksortLomuto(int *numbers, int size){
    QuicksortLomuto(numbers, 0, size - 1);
    drawFinalAnimation();
}

void quickSortHoare(int *numbers, int size){
    QuickSortHoare(numbers, 0, size - 1);
    drawFinalAnimation();
}
