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

#ifndef UNIONFIND
#define UNIONFIND

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

#include "./graph.c"

struct DisjointUnion{
    uint32_t *parents;
    size_t *ranks;
    size_t size;
};

void disjointUnionInit(struct DisjointUnion **u, size_t size){
    *u = malloc(sizeof(struct DisjointUnion));
    (*u)->parents = malloc(sizeof(uint32_t) * size);
    (*u)->ranks = malloc(sizeof(size_t) * size);
    (*u)->size = size;
    for(size_t i = 0; i < size; i++){
        (*u)->parents[i] = i;
        (*u)->ranks[i] = 1;
    }
}

void disjointUnionPrint(struct DisjointUnion *u){
    for(size_t i = 0; i < u->size; i++){
        printf("%i ", u->parents[i]);
    }
    printf("\n");
}

void disjointUnionFree(struct DisjointUnion *u){
    free(u->parents);
    free(u->ranks);
    free(u);
}

int disjointUnionFind(struct DisjointUnion *u, uint32_t n){
    while(n != u->parents[n]){
        u->parents[n] = u->parents[u->parents[n]];
        n = u->parents[n];
    }

    return n;
}

void disjointUnionUnion(struct DisjointUnion *u, int a, int b){
    const int parentA = disjointUnionFind(u, a);
    const int parentB = disjointUnionFind(u, b);
    // disjointUnionPrint(u);
    if(parentA == parentB){
        return;
    }
    
    const size_t newRank = u->ranks[parentA] + u->ranks[parentB];
    if(u->ranks[parentA] < u->ranks[parentB]){
        u->parents[parentA] = parentB;
        u->ranks[parentB] = newRank;
    }
    else{
        u->parents[parentB] = parentA;
        u->ranks[parentA] = newRank;
    }
}

#endif
