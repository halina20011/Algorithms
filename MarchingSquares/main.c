// gcc main.c $(sdl2-config --cflags --libs) -lm -lpng -o Build/main && ./Build/main

// Copyright (C) 2023  halina20011
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <time.h>

#include "../pixel.h"
#include "../pngWrapper.c"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

void drawPoints(struct Pixel *p, int **field, int columns, int rows, int resolution){
    pixelSetColor(p, 255, 0, 0, 255);
    for(int i = 0; i < columns; i++){
        for(int j = 0; j < rows; j++){
            if(1 <= field[i][j]){
                pixelDraw(p, j * resolution, i * resolution);
            }
        }
    }
}

int getState(int a, int b, int c, int d){
    return d * 8 + c * 4 + b * 2 + a * 1;
}

void drawIsolines(struct Pixel *p, int **field, int m, int n, int resolution){
    for(int i = 0; i < m - 1; i++){
        for(int j = 0; j < n - 1; j++){
            int x = j * resolution;
            int y = i * resolution;

            int a[2] = {x + resolution / 2, y                 };
            int b[2] = {x + resolution    , y + resolution / 2};
            int c[2] = {x + resolution / 2, y + resolution    };
            int d[2] = {x                 , y + resolution / 2};

            // int state = getState(arr);
            int A = field[i][j];
            int B = field[i][j + 1];
            int C = field[i + 1][j + 1]; 
            int D = field[i + 1][j];
            
            int state = getState(D, C, B, A);

            switch(state){
                case 0:
                    break;
                case 1:
                    drawLine(p, c[0], c[1], d[0], d[1]);
                    break;
                case 2:
                    drawLine(p, b[0], b[1], c[0], c[1]);
                    break;
                case 3:
                    drawLine(p, b[0], b[1], d[0], d[1]);
                    break;
                case 4:
                    drawLine(p, a[0], a[1], b[0], b[1]);
                    break;
                case 5:
                    drawLine(p, a[0], a[1], d[0], d[1]);
                    drawLine(p, b[0], b[1], c[0], c[1]);
                    break;
                case 6:
                    drawLine(p, a[0], a[1], c[0], c[1]);
                    break;
                case 7:
                    drawLine(p, a[0], a[1], d[0], d[1]);
                    break;
                case 8:
                    drawLine(p, a[0], a[1], d[0], d[1]);
                    break;
                case 9: 
                    drawLine(p, a[0], a[1], c[0], c[1]);
                    break;
                case 10: 
                    drawLine(p, a[0], a[1], b[0], b[1]);
                    drawLine(p, d[0], d[1], c[0], c[1]);
                    break;
                case 11: 
                    drawLine(p, a[0], a[1], b[0], b[1]);
                    break;
                case 12: 
                    drawLine(p, b[0], b[1], d[0], d[1]);
                    break;
                case 13: 
                    drawLine(p, b[0], b[1], c[0], c[1]);
                    break;
                case 14: 
                    drawLine(p, d[0], d[1], c[0], c[1]);
                    break;
                case 15:
                    break;
            }
        }
    }
}

int **makeArray(int columns, int rows){
    int **field = malloc(sizeof(int *) * columns);
    
    for(int i = 0; i < columns; i++){
        field[i] = malloc(sizeof(int) * rows);
    }

    return field;
}

void freeArray(int **array, int width){
    for(int i = 0; i < width; i++){
        free(array[i]);
    }

    free(array);
}

void marchingSquares(struct Pixel *p, int **field, int columns, int rows, int resolution){
    unsigned seed = time(NULL); // time(0);
    srand(seed);
    
    // Generate new patern
    for(int y = 0; y < columns; y++){
        for(int x = 0; x < rows; x++){
            // Generate random value from 0 to 1
            field[y][x] = rand() % 2;
        }
    }

    // Debugging
    // for(int i = 0; i < columns; i++){
    //     for(int j = 0; j < rows; j++){
    //         printf("%i,", field[i][j]);
    //     }
    //     printf("\n");
    // }

    pixelSetColor(p, 0, 0, 0, 255);
    pixelFill(p);

    // Set new color
    pixelSetColor(p, 255, 0, 0, 255);
    // drawPoints(p, field, columns, rows, resolution);
    drawIsolines(p, field, columns, rows, resolution);
    pixelUpdate(p);
}

int main(){
    int resolution = 10;
    int columns = 1 + WINDOW_WIDTH / resolution;
    int rows = 1 + WINDOW_HEIGHT / resolution;
    
    int **field = makeArray(columns, rows);
    
    struct Pixel *p = pixelInit("Marching Squares", WINDOW_WIDTH, WINDOW_HEIGHT);

    marchingSquares(p, field, columns, rows, resolution);

    while(1){
        int r = pixelEvents(p);
        if(r == PIXEL_EXIT){
            break;
        }
        else if(r == PIXEL_RUN){
            marchingSquares(p, field, columns, rows, resolution);
        }
    }
    
    freeArray(field, columns);
    
    pixelFree(p);

    return 0;
}
