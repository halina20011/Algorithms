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

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "./graph.h"

struct DisjointUnion{
    uint32_t *parents;
    size_t *ranks;
    size_t size;
};

void disjointUnionInit(struct DisjointUnion **u, size_t size);

void disjointUnionPrint(struct DisjointUnion *u);


void disjointUnionFree(struct DisjointUnion *u);

int disjointUnionFind(struct DisjointUnion *u, uint32_t n);

void disjointUnionUnion(struct DisjointUnion *u, int a, int b);

#endif
