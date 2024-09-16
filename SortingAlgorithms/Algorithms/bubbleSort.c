#include "algorithms.h"

void bubbleSort(){
    bool isSorted = true;

    printf("start\n");
    // vertexUpdate(v);
    // draw();
    //
    for(size_t i = 0; i < numbersSize - 1; i++){
        printf("%zu\n", i);
        isSorted = true;

        for(size_t j = 0; j < numbersSize - i - 1; j++){
            highlight(j);
            highlight(j + 1);

            // VertexWait();
            
            // printf("%p %p\n", numbers[j + 1], numbers[j]);
            // printf("%i < %i\n", numbers[j + 1]->val, numbers[j]->val);
            if(numbers[j + 1]->val < numbers[j]->val){
                // printf("swap\n");
                swap(j, j + 1);
                isSorted = false;
            }
            
            // printf("draw\n");
            draw();
            // update();
        }

        if(isSorted){
            break;
        }
    }

    printf("end\n");
}
