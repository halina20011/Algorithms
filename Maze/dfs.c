#include "dfs.h"

void createMazeDFS(struct Maze *maze, bool animate){
    struct CellPointVector *cellStack= CellPointVectorInit();

    srand(time(NULL));

    struct Cell *start = &(maze->board[0][0]);
    start->visited = true;
    CellPointVectorPush(cellStack, start);

    const int8_t dirs[] = {UP, RIGHT, DOWN, LEFT};
    const int8_t ops[] = {DOWN, LEFT, UP, RIGHT};
    const int8_t posY[] = {-1, 0, 1, 0};
    const int8_t posX[] = {0, 1, 0, -1};

    int8_t cellsDirIndex[4];
    struct Cell **cels = malloc(sizeof(struct Cell) * 4);
    struct Cell *c = NULL;
    while(cellStack->size){
        CellPointVectorPop(cellStack, &c);
        // printf("%i %i\n", c->x, c->y);

        uint8_t s = 0;
        for(uint8_t i = 0; i < 4; i++){
            if(c->mask & dirs[i] && !(c->connected & dirs[i])){
                // printf("adding %i\n", i);
                struct Cell *ne = &maze->board[c->y + posY[i]][c->x + posX[i]];
                if(!ne->visited){
                    cels[s] = ne;
                    cellsDirIndex[s++] = i;
                }
            }
        }
        // printf("size %i\n", s);
        if(s != 0){
            CellPointVectorPush(cellStack, c);

            int index = rand() % s;
            uint8_t dirrIndex = cellsDirIndex[index];
            struct Cell *conn = cels[index];
            c->connected |= dirs[dirrIndex];
            conn->connected |= ops[dirrIndex];
            conn->visited = true;
            CellPointVectorPush(cellStack, conn);
        }
    }

    CellPointVectorFree(cellStack);
}
