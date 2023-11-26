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

#ifndef GRAPHDRAW
#define GRAPHDRAW

#include <stdlib.h>

#include "../pixel.h"
#include "./graph.c"

extern struct Pixel *p;

void drawVertex(struct Vertex *v){
    drawCircle(p, v->x, v->y, 5);
}

void drawEdge(struct Edge *e){
    drawLine(p, e->a->x, e->a->y, e->b->x, e->b->y);
}

void drawGraph(struct Vertex **verticies, const size_t vertexSize, struct Edge **edges, const size_t edgesSize){
    pixelSetColor(p, 0, 0, 0, 255);
    pixelFill(p);
    pixelSetColor(p, 255, 255, 255, 255);

    for(size_t i = 0; i < vertexSize; i++){
        drawVertex(verticies[i]);
    }

    pixelSetColor(p, 255, 0, 0, 255);
    for(size_t i = 0; i < edgesSize; i++){
        drawEdge(edges[i]);
    }

    pixelUpdate(p);
}

#endif
