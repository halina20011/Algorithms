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

#include "graph.h"

struct Vertex *vertexInit(int x, int y){
    struct Vertex *v = malloc(sizeof(struct Vertex));
    v->x = x;
    v->y = y;

    return v;
}

struct Vertex *vertexCopy(struct Vertex *v){
    return vertexInit(v->x, v->y);
}

struct Edge *edgeInit(struct Vertex *a, struct Vertex *b){
    struct Edge *e = malloc(sizeof(struct Edge));
    e->a = vertexCopy(a);
    e->b = vertexCopy(b);

    return e;
}

struct Connection *connectionInit(struct Vertex **vertices, size_t a, size_t b){
    struct Connection *connection = malloc(sizeof(struct Connection));
    connection->a = a;
    connection->b = b;
    connection->pointsSize = pow(vertices[b]->x - vertices[a]->x, 2) + pow(vertices[b]->y - vertices[a]->y, 2);

    return connection;
}

struct Connection **generateConnections(struct Vertex **vertices, size_t verticesSize, size_t *connSize){
    // n 
    // full adjacency matrix has n^2
    // for undirected graph without any vertices connected to itself
    // we can use pointsSize = n ^ 2 / 2 - n / 2 = > (n * (n - 1)) / 2
    *connSize = (verticesSize * (verticesSize - 1)) / 2;
    struct Connection **connections = malloc(sizeof(struct Connection*) * (*connSize));
    size_t index = 0;
    for(size_t y = 1; y < verticesSize; y++){
        for(size_t x = 0; x < y; x++){
            connections[index++] = connectionInit(vertices, y, x);
        }
    }

    return connections;
}
