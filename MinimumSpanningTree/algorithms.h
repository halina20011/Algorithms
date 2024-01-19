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

#ifndef ALGORITHMS
#define ALGORITHMS

#include <stdbool.h>
#include <inttypes.h>

#include "./graph.h"

#include "./unionFind.h"
#include "./vector.h"

#include "../pixel.h"
#include "./graphDraw.h"

void kruskalsAlgorithm(struct Vertex **points, size_t pointsSize);
void primsAlgorithm(struct Vertex **vertices, size_t verticesSize);

#endif
