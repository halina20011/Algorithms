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

#include "algorithms.h"

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

    size_t conSize = 0;
    struct Connection **c = generateConnections(points, pointsSize, &conSize);

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
