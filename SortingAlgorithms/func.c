#include "func.h"

extern struct Number *numbers;
extern size_t numbersSize;

// void fillArrays(struct Numbers *numbers, int *array, double increase){
//     for(int i = 0; i < numbers; i++){
//         array[i] *= increase;
//     }
// }
//
// void shuffleArray(struct Numbers *numbers){
//     unsigned seed = time(NULL);
//     srand(seed);
//
//     for(size_t i = 0; i < numbers->size; i++){
//         int newIndex = rand() % numbers->size;
//         if(i != newIndex){
//             struct Number *a = &(numbers->number[newIndex]);
//             struct Number *b = &(numbers->number[i]);
//             struct Number temp = *a;
//             *a = *b;
//             *b = temp;
//         }
//     }
// }
//
// void divisotors(int *array, int number, int *size){
//     int s = 0;
//     const int sqrtN = sqrt(number);
//     for(int i = 2; i <= sqrtN; i++){
//         if(WINDOW_WIDTH % i == 0){
//             if(array){
//                 array[s] = i;
//             }
//             s++;
//             if(i != sqrtN){
//                 if(array){
//                     array[s] = number / i;
//                 }
//                 s++;
//             }
//         }
//     }
//
//     *size = s;
// }
//
// int searchInArray(int *array, int arraySize, int target){
//     int left = 0;
//     int right = arraySize;
//     while(left <= right){
//         const int m = left + (right - left) / 2;
//
//         if(array[m] == target){
//             return m;
//         }
//         else if(array[m] < target){
//             left = m + 1;
//         }
//         else{
//             right = m - 1;
//         }
//     }
//
//     return -1;
// }

// void update(){
//     while(changes.size){
//         size_t i = changes.val[--changes.size];
//         updateIndex(v, i);
//     }
// }
//
// void printArray(int *array, int arraySize){
//     for(int i = 0; i < arraySize; i++){
//         printf("%d ", array[i]);
//     }
//
//     printf("\n");
// }
//
// void drawNumbers(){
//     vertexSetColor(v, 0, 0, 0, 255);
//     vertexFill(v);
//     vertexSetColor(v, 255, 255, 255, 255);
//
//     for(int i = 0; i < arrSize; i++){
//         int x = i * numberWidth;
//         drawRectangle(v, x, WINDOW_HEIGHT - arr[i], numberWidth, arr[i]);
//     }
// }
//
// void highlight(int index){
//     if(index < 0 || arrSize <= index){
//         return;
//     }
//
//     uint8_t *curr = p->rgba;
//     uint8_t temp[4] = {0, 0, 0, 255};
//
//     int x = index * numberWidth;
//     p->rgba = temp;
//     drawRectangle(v, x, 0, numberWidth, WINDOW_HEIGHT);
//
//     p->rgba = curr;
//     drawRectangle(v, x, WINDOW_HEIGHT - arr[index], numberWidth, arr[index]);
// }
//

// void swapNumbers(int i, int j){
//     // highlight(i);
//     // highlight(j);
//     // VertexWait();
//     sleepFunc(1);
//
//
//     swap(numbers + i, numbers + j);
//     // highlight(i);
//     // highlight(j);
//     // changes.val[changes.size++] = i;
//     // changes.val[changes.size++] = j;
//     sleepFunc(1);
//     // VertexWait();
// }

//
// void highlightRegion(int left, int right){
//     if(!HIGHLIGHT)
//         return;
//
//     vertexSetColor(v, 0, 0, 255, 255);
//     const int end = (right < arrSize) ? right : arrSize;
//     for(int i = left; i < end; i++){
//         int x = i * numberWidth;
//         drawRectangle(v, x, 0, numberWidth, WINDOW_HEIGHT - arr[i]);
//     }
//     vertexSetColor(v, 255, 255, 255, 255);
// }
//
// void drawFinalAnimation(){
//     drawNumbers();
//     vertexSetColor(v, 0, 255, 0, 255);
//     for(int i = 0; i < arrSize; i++){
//         int x = i * numberWidth;
//         drawRectangle(v, x, WINDOW_HEIGHT - arr[i], numberWidth, arr[i]);
//         VertexWait();
//     }
// }
