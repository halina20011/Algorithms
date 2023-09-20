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

#ifndef GRAPH
#define GRAPH

#include <stdlib.h>

struct Vertex{
    int x, y;
};

struct Vertex *vertexInit(int x, int y){
    struct Vertex *v = malloc(sizeof(struct Vertex));
    v->x = x;
    v->y = y;

    return v;
}

struct Vertex *vertexCopy(struct Vertex *v){
    return vertexInit(v->x, v->y);
}

struct Edge{
    struct Vertex *a, *b;
};

struct Edge *edgeInit(struct Vertex *a, struct Vertex *b){
    struct Edge *e = malloc(sizeof(struct Edge));
    e->a = vertexCopy(a);
    e->b = vertexCopy(b);

    return e;
}

#endif
