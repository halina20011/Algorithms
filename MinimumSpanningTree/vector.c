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

#ifndef VECTOR
#define VECTOR

#include <stdlib.h>
#include <inttypes.h>

struct Vector{
    void **val;
    size_t maxSize;
    size_t size;
};

void vectorInit(struct Vector **v){
    *v = malloc(sizeof(struct Vector));
    (*v)->maxSize = 20;
    (*v)->size = 0;
    (*v)->val = malloc(sizeof(void*) * (*v)->maxSize);
}

void vectorPush(struct Vector *v, void *val){
    if(v->maxSize <= v->size){
        v->maxSize *= 2;
        v->val = realloc(v->val, sizeof(void*) * v->maxSize);
    }

    v->val[v->size++] = val;
}

void vectorFree(struct Vector *v){
    for(size_t i = 0; i < v->size; i++){
        free(v->val[i]);
    }

    free(v);
}

#endif
