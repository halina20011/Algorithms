#ifndef MAZE
#define MAZE

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "../vector.h"

#define UP      (1 << 0)
#define RIGHT   (1 << 1)
#define DOWN    (1 << 2)
#define LEFT    (1 << 3)
#define ALL     (UP | RIGHT | DOWN | LEFT)

struct Cell{
    uint8_t connected;
    uint8_t mask;
    bool visited;
    uint16_t x, y;
};

NEW_VECTOR_TYPE(struct Cell*, CellPointVector);

struct Maze{
    struct Cell **board;
    uint16_t width, height;
    uint8_t wall, notWall;
};

void mazeGetSize(uint32_t windowWidth, uint32_t windowHeight);

void mazeInit(struct Maze *maze, uint16_t width, uint16_t height);
void mazeFromArray(struct Maze *maze, uint16_t width, uint16_t height, uint8_t *board);

void mazeValPrint(struct Maze maze);
void mazePrint(struct Maze maze);

#endif
