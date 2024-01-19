// ./build.sh && ./Build/main

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

// TODO: boruvka's algorithm 

#include <stdio.h>
#include <stdlib.h>

#include "../pixel.h"
// #include "../pngWrapper.c"

// for time() and clock()
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "./graph.h"

#include "./algorithms.h"

struct Pixel *p = NULL;

struct Vertex **randomVertices(int maxX, int maxY, size_t size){
    unsigned seed = time(NULL); // time(0);
    srand(seed);

    struct Vertex **vertices = malloc(sizeof(struct Vertex*) * size);
    for(size_t i = 0; i < size; i++){
        vertices[i] = vertexInit(rand() % maxX, rand() % maxY);
    }

    return vertices;
}

void freeVertices(struct Vertex **vertices, size_t size){
    for(size_t i = 0; i < size; i++){
        free(vertices[i]);
    }

    free(vertices);
}

int main(){
    p = pixelInit("minimum spanning tree", 600, 600);
    pixelSetIndexSpeed(p, 1);
    
    const size_t verticesSize = 50;
    struct Vertex **vertices = randomVertices(p->width, p->height, verticesSize);
    
    kruskalsAlgorithm(vertices, verticesSize);
    p->run = true;

    primsAlgorithm(vertices, verticesSize);
    
    freeVertices(vertices, verticesSize);

    pixelFree(p);

    return 0;
}
