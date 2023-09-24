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

#include <math.h>

#include "../pixel.h"
#include "../rgba.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

float cenX = 0.0;
float cenY = 0.0;
float scale = 1.0;

const float bounds = 2.0;
const int maxIterations = 100;

void pixelToPoint(int x, int y, long double *px, long double *py, int width, int height){
    *px = (x - width / 2.0) * (4.0 / width) * (1.0 / scale) + cenX;
    *py = (y - height / 2.0) * (4.0 / height) * (1.0 / scale) + cenY;
}

void calculatePoint(long double cx, long double cy, int *iterations, int *isIn){
    long double zx = 0;
    long double zy = 0;

    *iterations = 0;
    *isIn = 1;

    while(*iterations < maxIterations && *isIn == 1){
        long double tzx = zx * zx - zy * zy + cx;
        zy = 2 * zx * zy + cy;
        zx = tzx;

        *iterations += 1;

        float d = hypot(zx, zy);
        if(bounds < d){
            *isIn = 0;
        }
    }
}

float smoothColor(float n, float x, float y){
    return 1.0 + n - log(log(hypot(x, y))) / log(2.0);
}

void mandelbrotSet(struct Pixel *p, int width, int height){
    long double px = 0.0; 
    long double py = 0.0;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int i = 4 * (y * width + x);

            int iterations = 0;
            int isIn = 0;
            
            pixelToPoint(x, y, &px, &py, width, height);
            calculatePoint(px, py, &iterations, &isIn);

            int r = 0; 
            int g = 0; 
            int b = 0;

            // float val = smoothColor((float)iterations, (float)px, (float)py) / (float)maxIterations;
            // val = (isnan(val) == 1) ? (float)iterations/(float)maxIterations : val;
            // printf("%f\n", val);
            float val = (float)iterations/(float)maxIterations;
            float h = 360.0 * val;
            float v = 10.0  * val;

            hsv2rgb(h, 1.0, v, &b, &g, &r);

            // if(1 < iterations && isIn == 0){
            //     // float h = 150 + 200 - fmod(pow(iterations/70.0, 0.5) * 200, 255);
            //     // float s = 100;
            //     // float v = 100;
            //     // hsv2rgb(h, s, v, &r, &g, &b);
            //     r = 255; g = 255; b = 255;
            // }
            // else if(isIn == 0){
            //     r = 115; g = 115; b = 115;
            // }

            p->buffer[i + 0] = r;
            p->buffer[i + 1] = g;
            p->buffer[i + 2] = b;
            p->buffer[i + 3] = 255;
        }
    }
}

int main(){
    struct Pixel *p = pixelInit("Mandelbrot set", WINDOW_WIDTH, WINDOW_HEIGHT);

    mandelbrotSet(p, p->width, p->height);
    pixelUpdate(p);
    
    pixelWaitExit(p);

    pixelFree(p);

    return 0;
}
