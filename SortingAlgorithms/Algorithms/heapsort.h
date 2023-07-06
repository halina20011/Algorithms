#include <stdbool.h>

#include "../func.h"

#define HEAPSORT {"Heapsort", &heapsort}

void maxHeapify(uint8_t *buffer, int *heap, int heapSize, int node){
    int leftChild = 2 * node + 1;
    int rightChild = 2 * node + 2;
    int max = node;
    if(leftChild < heapSize && heap[node] < heap[leftChild]){
        max = leftChild;
        if(buffer != NULL){
            ProcessEvents()
            drawNumbers(node, leftChild);
            update(buffer);
            wait();
        }
    }
    if(rightChild < heapSize && heap[max] < heap[rightChild]){
        max = rightChild;
        if(buffer != NULL){
            ProcessEvents()
            drawNumbers(node, rightChild);
            update(buffer);
            wait();
        }
    }
    if(max != node){
        swap(heap + node, heap + max);
        maxHeapify(buffer, heap, heapSize, max);
    }
}

void heapsort(uint8_t *buffer, int *numbers, int numbersSize){
    int heapSize = numbersSize;
    
    // create max heap
    const int lastInternalNode = (heapSize - 1) / 2;
    for(int node = lastInternalNode; node >= 0; node--){
        maxHeapify(buffer, numbers, heapSize, node);
    }

    while(heapSize){
        // move the max number to the end and decrease the size of the heap
        swap(numbers + 0, numbers + --heapSize);
        maxHeapify(buffer, numbers, heapSize, 0);
    }
}
