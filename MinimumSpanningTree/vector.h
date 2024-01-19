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

void vectorInit(struct Vector **v);

void vectorPush(struct Vector *v, void *val);

void vectorFree(struct Vector *v);

#endif
