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

#ifndef GRADIENT
#define GRADIENT

#include <stdlib.h>
#include "rgba.h"

double lerp(double p1, double p2, double t){
    return p1 + (p2 - p1) * t;
}

double lerpt(double p1, double p2, double v){
    return (v - p1) / (p2 - p1);
}

int makeGradient(int resolution, struct RGBA ***output, struct RGBA *colors, int *positions, int size){
    *output = malloc(sizeof(struct RGBA*) * resolution);
    if(*output == NULL){
        return 1;
    }

    for(int i = 0; i < resolution; i++){
        struct RGBA *c = malloc(sizeof(struct RGBA));
        if(c == NULL){
            return 1;
        }

        rgbaInit(c, 0, 0, 0, 0);
        *(*output + i) = c;
    }
    
    int index = 0;
    for(int i = 0; i < size - 1; i++){
        struct RGBA color1 = colors[i + 0];
        struct RGBA color2 = colors[i + 1];

        // Get resolution between colors
        int r = positions[i + 1] - positions[i];
        for(int x = 0; x < r; x++){
            double t = lerpt(0, r, x);
            
            (*(*output + index + x))->r = lerp(color1.r, color2.r, t);
            (*(*output + index + x))->g = lerp(color1.g, color2.g, t);
            (*(*output + index + x))->b = lerp(color1.b, color2.b, t);
            (*(*output + index + x))->a = lerp(color1.a, color2.a, t);
        }

        index += r;
    }

    return 1;
}

void clearGradient(struct RGBA ***g, int size){
    for(int i = 0; i < size; i++){
        free(*(*g + i));
    }

    free(*g);
}

#endif
