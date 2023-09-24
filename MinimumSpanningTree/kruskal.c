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

#ifndef KRUSKAL
#define KRUSKAL

#include <stdbool.h>
#include <inttypes.h>

#include "./graph.h"

#include "./unionFind.h"
#include "./vector.h"

#include "../pixel.h"
#include "./graphDraw.h"

extern struct Pixel *p;

struct Connection{
    int a, b;
    int pointsSize;
};

int comp(const void *a, const void *b){
    const int x = (*(struct Connection**)a)->pointsSize;
    const int y = (*(struct Connection**)b)->pointsSize;
    return (y < x) - (x < y);
}

void kruskalsAlgorithm(struct Vertex **points, size_t pointsSize){
    struct Vector *v;
    vectorInit(&v);

    struct DisjointUnion *u;
    disjointUnionInit(&u, pointsSize);

    // n 
    // full adjacency matrix has n^2
    // for undirected graph without any vertices connected to itself
    // we can use pointsSize = n ^ 2 / 2 - n / 2
    size_t conSize = (pointsSize * pointsSize) / 2 - (pointsSize / 2);
    struct Connection **c = malloc(sizeof(struct Connection) * conSize);
    size_t index = 0;
    for(size_t i = 1; i < pointsSize; i++){
        for(size_t j = 0; j < i; j++){
            c[index] = malloc(sizeof(struct Connection));
            c[index]->a = i;
            c[index]->b = j;
            c[index++]->pointsSize = pow(points[i]->x - points[j]->x, 2) + pow(points[i]->y - points[j]->y, 2);
        }
    }

    qsort(c, conSize, sizeof(struct Connection*), comp);
    
    size_t toConnect = pointsSize - 1;
    for(size_t i = 0; i < conSize && toConnect; i++){
        drawGraph(points, pointsSize, (struct Edge**)v->val, v->size);
        pixelWait(p);

        const int parentA = disjointUnionFind(u, c[i]->a);
        const int parentB = disjointUnionFind(u, c[i]->b);

        pixelSetColor(p, 0, 0, 255, 255);
        drawEdge(&(struct Edge){.a=points[c[i]->a], .b=points[c[i]->b]});
        pixelUpdate(p);
        pixelWait(p);

        // add edge
        if(parentA != parentB){
            vectorPush(v, edgeInit(points[c[i]->a], points[c[i]->b]));
            disjointUnionUnion(u, c[i]->a, c[i]->b);
            toConnect--;
        }

        if(pixelEvents(p) == PIXEL_EXIT){
            break;
        }

        pixelWait(p);
    }

    drawGraph(points, pointsSize, (struct Edge**)v->val, v->size);
  
    pixelWaitExit(p);

    disjointUnionFree(u);
    vectorFree(v);

    for(size_t i = 0; i < conSize; i++){
        free(c[i]);
    }
    free(c);
}

#endif
