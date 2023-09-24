#include <stdbool.h>

#include "../../pixel.h"
#include "../func.h"

#define HEAPSORT {"Heapsort", &heapsort}

extern struct Pixel *p;

void maxHeapify(int *heap, int heapSize, int node){
    int leftChild = 2 * node + 1;
    int rightChild = 2 * node + 2;
    int max = node;

    drawNumbers();
    if(leftChild < heapSize && heap[max] < heap[leftChild]){
        max = leftChild;
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(node);
        highlight(leftChild);
        PixelWait();
    }

    if(rightChild < heapSize && heap[max] < heap[rightChild]){
        max = rightChild;
        pixelSetColor(p, 255, 0, 0, 255);
        highlight(node);
        highlight(rightChild);
        PixelWait();
    }

    if(max != node){
        swapNumbers(node, max);
        maxHeapify(heap, heapSize, max);
    }
}

void heapsort(int *numbers, int numbersSize){
    int heapSize = numbersSize;
    
    // create max heap
    const int lastInternalNode = (heapSize - 1) / 2;
    for(int node = lastInternalNode; node >= 0; node--){
        maxHeapify(numbers, heapSize, node);
    }

    while(heapSize){
        // move the max number to the end and decrease the size of the heap
        swapNumbers(0, --heapSize);
        maxHeapify(numbers, heapSize, 0);
    }

    drawFinalAnimation(numbers, numbersSize);
}
