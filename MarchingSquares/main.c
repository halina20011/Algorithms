// gcc main.c $(sdl2-config --cflags --libs) -o Build/main && ./Build/main

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

void drawPoint(SDL_Renderer *renderer, int x, int y){
    SDL_RenderDrawPoint(renderer, x, y);
}

void drawPoints(SDL_Renderer *renderer, int **field, int columns, int rows, int resolution){
    for(int i = 0; i < columns; i++){
        for(int j = 0; j < rows; j++){
            if(field[i][j] >= 1){
                // renderer,                  x,              y
                SDL_RenderDrawPoint(renderer, j * resolution, i * resolution);
            }
        }
    }
}

int getState(int a, int b, int c, int d){
    return d * 8 + c * 4 + b * 2 + a * 1;
}

void drawIsolines(SDL_Renderer *renderer, int **field, int m, int n, int resolution){
    for(int i = 0; i < m - 1; i++){
        for(int j = 0; j < n - 1; j++){
            int x = j * resolution;
            int y = i * resolution;

            int a[2] = {x + resolution / 2, y                 };
            int b[2] = {x + resolution    , y + resolution / 2};
            int c[2] = {x + resolution / 2, y + resolution    };
            int d[2] = {x                 , y + resolution / 2};

            // int state = getState(arr);
            int A = field[i][j];
            int B = field[i][j + 1];
            int C = field[i + 1][j + 1]; 
            int D = field[i + 1][j];
            
            int state = getState(D, C, B, A);

            switch(state){
                case 0:
                    break;
                case 1:
                    SDL_RenderDrawLine(renderer, c[0], c[1], d[0], d[1]);
                    break;
                case 2:
                    SDL_RenderDrawLine(renderer, b[0], b[1], c[0], c[1]);
                    break;
                case 3:
                    SDL_RenderDrawLine(renderer, b[0], b[1], d[0], d[1]);
                    break;
                case 4:
                    SDL_RenderDrawLine(renderer, a[0], a[1], b[0], b[1]);
                    break;
                case 5:
                    SDL_RenderDrawLine(renderer, a[0], a[1], d[0], d[1]);
                    SDL_RenderDrawLine(renderer, b[0], b[1], c[0], c[1]);
                    break;
                case 6:
                    SDL_RenderDrawLine(renderer, a[0], a[1], c[0], c[1]);
                    break;
                case 7:
                    SDL_RenderDrawLine(renderer, a[0], a[1], d[0], d[1]);
                    break;
                case 8:
                    SDL_RenderDrawLine(renderer, a[0], a[1], d[0], d[1]);
                    break;
                case 9: 
                    SDL_RenderDrawLine(renderer, a[0], a[1], c[0], c[1]);
                    break;
                case 10: 
                    SDL_RenderDrawLine(renderer, a[0], a[1], b[0], b[1]);
                    SDL_RenderDrawLine(renderer, d[0], d[1], c[0], c[1]);
                    break;
                case 11: 
                    SDL_RenderDrawLine(renderer, a[0], a[1], b[0], b[1]);
                    break;
                case 12: 
                    SDL_RenderDrawLine(renderer, b[0], b[1], d[0], d[1]);
                    break;
                case 13: 
                    SDL_RenderDrawLine(renderer, b[0], b[1], c[0], c[1]);
                    break;
                case 14: 
                    SDL_RenderDrawLine(renderer, d[0], d[1], c[0], c[1]);
                    break;
                case 15:
                    break;
            }
        }
    }
}

int **makeArray(int columns, int rows){
    int **field = (int**)malloc(sizeof(int *) * columns);
    
    for(int i = 0; i < columns; i++){
        *(field + i) = (int*)malloc(sizeof(int) * rows);
    }

    return field;
}

int freeArray(int **array, int columns){
    for(int i = 0; i < columns; i++){
        free(array[i]);
    }

    free(array);

    return 0;
}

int marchingSquares(SDL_Renderer *renderer, int **field, int columns, int rows, int resolution){
    unsigned seed = time(NULL); // time(0);
    srand(seed);
    
    // Generate new patern
    for(int y = 0; y < columns; y++){
        for(int x = 0; x < rows; x++){
            // Generate random value from 0 to 1
            field[y][x] = rand() % 2;
        }
    }

    // Debugging
    for(int i = 0; i < columns; i++){
        for(int j = 0; j < rows; j++){
            printf("%i,", field[i][j]);
        }
        printf("\n");
    }

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Set new color
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    drawPoints(renderer, field, columns, rows, resolution);
    drawIsolines(renderer, field, columns, rows, resolution);

    SDL_RenderPresent(renderer);
    
    return 0;
}

int main(){
    int resolution = 10;
    int columns = 1 + WINDOW_WIDTH / resolution;
    int rows = 1 + WINDOW_HEIGHT / resolution;
    
    int **field = makeArray(columns, rows);

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    // Create a window and renderer
    window = SDL_CreateWindow("Marching Squares", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    marchingSquares(renderer, field, columns, rows, resolution);

    int run = 1;
    while(run){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                SDL_Keycode key = event.key.keysym.sym;
                if(key == SDLK_q || key == SDLK_ESCAPE){
                    run = 0;
                }
                else if(key == SDLK_RETURN){
                    marchingSquares(renderer, field, columns, rows, resolution);
                }
            }
            else if(event.type == SDL_QUIT){
                run = 0;
            }
        }
    }
    
    freeArray(field, columns);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
