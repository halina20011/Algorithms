#include <stdbool.h>

#include "../func.h"

#define HEAPSORT {"Heapsort", &heapsort}

void maxHeapify(uint8_t **buffer, int *heap, int heapSize, int node){
    int leftChild = 2 * node + 1;
    int rightChild = 2 * node + 2;
    int max = node;
    if(leftChild < heapSize && heap[node] < heap[leftChild]){
        max = leftChild;
        if(buffer != NULL){
            SHOW;
            drawNumbers(node, leftChild);
            wait();
        }
    }
    if(rightChild < heapSize & heap[max] < heap[rightChild]){
        max = rightChild;
        if(buffer != NULL){
            SHOW;
            drawNumbers(node, rightChild);
            wait();
        }
    }
    if(max != node){
        swap(heap + node, heap + max);
        maxHeapify(buffer, heap, heapSize, max);
    }
}

void heapsort(uint8_t **buffer, int *numbers, int numbersSize){
    int heapSize = numbersSize;
    
    // create max heap
    int externalNodes = (heapSize + 1) / 2;
    for(int node = heapSize / 2 - 1; node >= 0; node--){
        maxHeapify(buffer, numbers, heapSize, node);
    }

    while(heapSize){
        // move the max number to the end and decrease the size of the heap
        swap(numbers + 0, numbers + --heapSize);
        maxHeapify(buffer, numbers, heapSize, 0);
    }
}
