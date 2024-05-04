#include "maze.h"

VECTOR_TYPE_FUNCTIONS(struct Cell*, CellPointVector, "");

void mazeGetSize(uint32_t windowWidth, uint32_t windowHeight){
    
}

void mazeInit(struct Maze *maze, uint16_t width, uint16_t height){
    struct Cell **rows = malloc(sizeof(struct Cell*) * height);
    for(uint16_t y = 0; y < height; y++){
        rows[y] = calloc(width, sizeof(struct Cell));
        for(uint16_t x = 0; x < width; x++){
            rows[y][x].connected = 0;
            rows[y][x].mask = 0xff;
            rows[y][x].x = x;
            rows[y][x].y = y;
            rows[y][x].visited = false;
        }
    }

    for(uint16_t x = 0; x < width; x++){
        rows[0][x].mask ^= UP;
        rows[height - 1][x].mask ^= DOWN;
    }

    for(uint16_t y = 0; y < height; y++){
        rows[y][0].mask ^= LEFT;
        rows[y][width - 1].mask ^= RIGHT;
    }

    maze->board = rows;
    maze->width = width;
    maze->height = height;

    // const char str[] = {0xE2, 0x96, 0x88, 0x0};
    maze->wall = '#';
    maze->notWall = ' ';
}

void mazeFromArray(struct Maze *maze, uint16_t width, uint16_t height, uint8_t *board){
    mazeInit(maze, width, height);

    for(uint16_t y = 0; y < height; y++){
        for(uint16_t x = 0; x < width; x++){
            size_t i = y * width + x;
            maze->board[y][x].connected = board[i];
        }
    }
}

char *decodeChar(uint8_t con){
    switch(con){
        case UP:
            return "╵";
        case RIGHT:
            return "╶";
        case DOWN:
            return "╷";
        case LEFT:
            return "╴";
        case UP | RIGHT:
            return "└";
        case RIGHT | DOWN:
            return "┌";
        case DOWN | LEFT:
            return "┐";
        case UP | LEFT:
            return "┘";
        case UP | DOWN:
            return "│";
        case RIGHT | LEFT:
            return "─";
        case UP | RIGHT | DOWN:
            return "├";
        case RIGHT | DOWN | LEFT:
            return "┬";
        case DOWN | LEFT | UP:
            return "┤";
        case LEFT | UP | RIGHT:
            return "┴";
        case UP | RIGHT | DOWN | LEFT:
            return "┼";
        default:
            return "█";
    }

}

void mazeValPrint(struct Maze maze){
    for(uint16_t y = 0; y < maze.height; y++){
        for(uint16_t x = 0; x < maze.width; x++){
            char *c = decodeChar(maze.board[y][x].connected);
            printf("%s", c);
        }
        putchar('\n');
    }
}

void mazePrint(struct Maze maze){
    for(uint16_t x = 0; x < maze.height * 2 + 2; x++){
        putchar(maze.wall);
    }
    putchar('\n');

    for(uint16_t y = 0; y < maze.height; y++){
        putchar(maze.wall);
        for(uint16_t x = 0; x < maze.width - 1; x++){
            if(maze.board[y][x].connected){
                putchar(maze.notWall);
            }
            else{
                putchar(maze.wall);
            }

            putchar(maze.board[y][x].connected);
            if(maze.board[y][x].connected & RIGHT){
                putchar(maze.notWall);
            }
            else{
                putchar(maze.wall);
            }
        }

        putchar(maze.wall);
        putchar('\n');

        if(y != maze.height - 1){
            for(uint16_t x = 0; x < maze.width; x++){
                if(maze.board[y][x].connected & DOWN){
                    putchar(maze.notWall);
                }
                else{
                    putchar(maze.wall);
                }
            }
        }
    }
}
