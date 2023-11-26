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

#ifndef PRIMS
#define PRIMS

#include <stdbool.h>
#include <math.h>
#include <sys/types.h>

#include "graph.c"
#include "./vector.c"

#include "../pixel.h"
#include "./graphDraw.c"

extern struct Pixel *p;

void heapify(struct Connection **arr, size_t size, int node){
    const int left = 2 * node + 1;
    const int right = 2 * node + 2;
    int min = node;
    min = ((size_t)left < size && arr[left]->pointsSize < arr[min]->pointsSize) ? left : min;
    min = ((size_t)right < size && arr[right]->pointsSize < arr[min]->pointsSize) ? right : min;
    if(min != node){
        struct Connection *temp = arr[node];
        arr[node] = arr[min];
        arr[min] = temp;
        heapify(arr, size, min);
    }
}

void swap(struct Connection **a, struct Connection **b){
    struct Connection *temp = *a;
    *a = *b;
    *b = temp;
}

void heapInsert(struct Vector *v, struct Connection *connection){
    vectorPush(v, connection);
    int parent = (v->size - 1) / 2;
    int node = v->size - 1;
    while(node && ((struct Connection*)v->val[node])->pointsSize < ((struct Connection*)v->val[parent])->pointsSize){
        swap((struct Connection**)(v->val + parent), (struct Connection**)(v->val + node));
        node = parent;
        parent = (node - 1) / 2;
    }
}

void print(struct Vector *v){
    for(size_t i = 0; i < v->size; i++){
        printf("%i ", ((struct Connection*)v->val[i])->pointsSize);
    }
    printf("\n\n\n");
}

void addConnections(size_t node, struct Vertex **vertices, size_t verticesSize, struct Vector *v, bool *table){
    pixelSetColor(p, 0, 0, 255, 255);
    for(size_t i = 0; i < verticesSize; i++){
        if(!table[i]){
            struct Connection *connection = connectionInit(vertices, node, i);
            heapInsert(v, connection);
            drawEdge(&(struct Edge){.a=vertices[node], .b=vertices[i]});
            pixelUpdate(p);
            pixelWait(p);

            if(pixelEvents(p) == PIXEL_EXIT){
                exit(0);
            }
        }
    }
}

// add arbitrary vertex to the tree
// get the smalles edge that is connected to the tree
// if both vertices are not in tree 
//      add the edges
//      delete the edge from heap and continue searching
void primsAlgorithm(struct Vertex **vertices, size_t verticesSize){
    struct Vector *mst;
    vectorInit(&mst);
    bool *table = calloc(verticesSize, sizeof(bool));

    struct Vector *v = NULL;
    vectorInit(&v);
    
    drawGraph(vertices, verticesSize, (struct Edge**)mst->val, mst->size);

    addConnections(0, vertices, verticesSize, v, table);

    while(true){
        // print(v);
        drawGraph(vertices, verticesSize, (struct Edge**)mst->val, mst->size);
        struct Connection *edge = NULL;
        while(!edge && 1 < v->size){
            edge = (struct Connection*)v->val[--v->size];
            swap((struct Connection**)v->val + 0, &edge);
            if(table[edge->a] && table[edge->b]){
                edge = NULL;
            }

            heapify((struct Connection**)v->val, v->size, 0);
        }

        if(edge == NULL){
            break;
        }

        vectorPush(mst, edgeInit(vertices[edge->a], vertices[edge->b]));

        if(!table[edge->a]){
            addConnections(edge->a, vertices, verticesSize, v, table);
        }
        if(!table[edge->b]){
            addConnections(edge->b, vertices, verticesSize, v, table);
        }

        table[edge->a] = true;
        table[edge->b] = true;

        pixelSetColor(p, 0, 0, 255, 255);
        drawEdge(&(struct Edge){.a=vertices[edge->a], .b=vertices[edge->b]});
        pixelUpdate(p);
        pixelWait(p);

        if(pixelEvents(p) == PIXEL_EXIT){
            break;
        }
    }

    drawGraph(vertices, verticesSize, (struct Edge**)mst->val, mst->size);
    pixelWaitExit(p);

    vectorFree(v);
}

#endif
