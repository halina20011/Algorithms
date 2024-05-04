#ifndef DFS_MAZE
#define DFS_MAZE

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "time.h"

#include "maze.h"

struct DFS{
    uint16_t width, height;
    uint8_t type;
};

void createMazeDFS(struct Maze *maze, bool animate);

#endif
