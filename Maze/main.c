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

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "maze.h"
#include "dfs.h"

#include "../pixel.h"
#include "../pngWrapper.c"

// for time() and clock()
#include <time.h>
#include <math.h>

// Generation
// TODO: Iterative randomized Kruskal's algorithm
// TODO: Iterative randomized Prim's algorithm
// TODO: Wilson's algorithm
// TODO: Recursive division method
//
// Solving
// TODO: Hand on Wall Rule
// TODO: Dead-end filling
// TODO: Random Wall Rule

int main(){
    // struct Pixel *p = pixelInit("Template", 600, 600);
    struct Maze maze = {};
    // mazeInit(&maze, 5, 4);
    //
    uint8_t board[20] = {
        UP | RIGHT, LEFT | RIGHT, LEFT | DOWN, RIGHT, LEFT | DOWN,
        RIGHT, LEFT | DOWN, UP | RIGHT | DOWN, LEFT | RIGHT, UP | LEFT,
        DOWN, UP | RIGHT | DOWN, ALL, LEFT, DOWN,
        UP | RIGHT, UP | LEFT, UP | RIGHT, RIGHT | LEFT, UP | DOWN | LEFT
    };

    mazeFromArray(&maze, 5, 4, board);
    mazeValPrint(maze);

    struct Maze maze2 = {};
    mazeInit(&maze2, 100, 20);
    createMazeDFS(&maze2, false);
    
    mazeValPrint(maze2);
    // mazePrint(maze);

    // pixelFree(p);

    return 0;
}
