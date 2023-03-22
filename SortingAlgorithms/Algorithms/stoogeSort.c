#include <math.h>
#include "../draw.c"

void *stoogeSortInit(SDL_Renderer *renderer, int *numbers, int width){
    int *i = malloc(sizeof(int));
    int *j = malloc(sizeof(int));
    int *pos = malloc(sizeof(int));
       
    int length = WINDOW_WIDTH / width;

    *(i) = 0;
    *(j) = length - 1;
    *(pos) = 1;

    int *stack = malloc(sizeof(int) * length);

    stack[0] = 0;
    stack[1] = length - 1;

    size_t paramSize = sizeof(SDL_Renderer*) + sizeof(int*) * 5 + sizeof(int);
    printf("Param size: %zu\n", paramSize);

    void *param = malloc(paramSize);
    *((SDL_Renderer**) (param)) = renderer;
    *((int**) (param + sizeof(SDL_Renderer*))) = numbers;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1)) = i;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2)) = j;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 3)) = pos;
    *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 4)) = stack;
    *((int*)  (param + sizeof(SDL_Renderer*) + sizeof(int*) * 5)) = width;

    return param;
}

int *stoogeSortFree(void *param){
    int *i      = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1));
    int *j      = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2));
    int *pos    = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 3));
    int *stack  = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 4));
    
    free(i);
    free(j);
    free(pos);
    free(stack);

    return 0;
}

// Example
// int *numbersCopy = malloc(sizeof(int) * length);
// memcpy(numbersCopy, numbers, sizeof(int) * length);
// stoogeSortAlg(numbersCopy, length);
// printArray(numbersCopy, length);
int stoogeSortAlg(int *numbers, int length){
    int i = 0;
    int j = length - 1;
    int pos = 1;

    int *stack = malloc(sizeof(int) * length);

    stack[0] = i;
    stack[1] = j;

    while(0 <= pos){
        j = stack[pos--];
        i = stack[pos--];

        if(2 < j - i + 1){
            int oneThird = ((j - i + 1) / 3);
            stack[++pos] = i;
            stack[++pos] = j - oneThird;

            stack[++pos] = i + oneThird;
            stack[++pos] = j;

            stack[++pos] = i;
            stack[++pos] = j - oneThird;
        }

        if(numbers[j] < numbers[i]){
            swap(&numbers[j], &numbers[i]);
        }
    }

    free(stack);
}

int stoogeSortAlgRecursive(int *numbers, int length, int i){
    if(numbers[length - 1] < numbers[i]){
        swap(&numbers[i], &numbers[length - 1]);
    }

    if(2 < length - i){
        int oneThird = (int)((length - i) / 3);
        stoogeSortAlgRecursive(numbers, length - oneThird, i);
        stoogeSortAlgRecursive(numbers, length, i + oneThird);
        stoogeSortAlgRecursive(numbers, length - oneThird, i);
    }
    
    return 0;
}

Uint32 stoogeSort(Uint32 interval, void *param){
    SDL_Renderer *renderer = *((SDL_Renderer**) param);
    int *numbers    = *((int**) (param + sizeof(SDL_Renderer*)));
    int *i          = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 1));
    int *j          = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 2));
    int *pos        = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 3));
    int *stack      = *((int**) (param + sizeof(SDL_Renderer*) + sizeof(int*) * 4));
    int width       = *((int*)  (param + sizeof(SDL_Renderer*) + sizeof(int*) * 5));

    int length = WINDOW_WIDTH / width;

    // Show only swaps
    if(0 <= *pos){
        while(0 <= *pos){
            *(j) = stack[*pos];
            *(pos) -= 1;
            *(i) = stack[*pos];
            *(pos) -= 1;
            
            // drawNumbers(renderer, numbers, length, width, *(i), *(j));
            
            int l = (*(j) - *(i)) + 1;
            
            if(2 < l){
                int oneThird = (int)(l / 3);

                stack[++*(pos)] = *(i);
                stack[++*(pos)] = *(j) - oneThird;

                stack[++*(pos)] = *(i) + oneThird;
                stack[++*(pos)] = *(j);

                stack[++*(pos)] = *(i);
                stack[++*(pos)] = *(j) - oneThird;
            }

            if(numbers[*(j)] < numbers[*(i)]){
                swap(&numbers[*(i)], &numbers[*(j)]);
                drawNumbers(renderer, numbers, length, width, *(i), *(j));
                break;
            }
        }
    }
    else{
        if(indexAnimation++ < length){
            drawFinalAnimation(renderer, numbers, length, width);
        }
    }

    return interval;
}
