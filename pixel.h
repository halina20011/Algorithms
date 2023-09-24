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

#ifndef PIXEL
#define PIXEL

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define PIXEL_CONTINUE 0
#define PIXEL_EXIT 1
#define PIXEL_RUN 2

// in milliseconds
const uint32_t sleepTable[10] = {1000, 500, 250, 100, 50, 10, 5, 2, 1, 0};
const size_t sleepTableSize = sizeof(sleepTable) / sizeof(sleepTable[0]);

#ifdef CAPTURE_ON
    #include "pngWrapper.c"
    #include <sys/types.h>
    #include <sys/stat.h>
    extern int capture;
    extern uint16_t captureIndex;
#endif

struct Pixel{
    uint8_t *buffer;
    size_t width, height, size;
    uint8_t *rgba;
    SDL_Texture *texture;

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    
    struct timespec sleepDuration;
    size_t sleepIndex;

    bool run;
};

void pixelSetIndexSpeed(struct Pixel*, size_t);
void pixelSetColor(struct Pixel*, uint8_t, uint8_t, uint8_t, uint8_t);

struct Pixel *pixelInit(const char *windowName, size_t width, size_t height){
    struct Pixel *p = malloc(sizeof(struct Pixel));

    p->size = width * height * 4;
    p->width = width;
    p->height = height;

    p->run = true;

    p->sleepDuration.tv_nsec = 0;
    pixelSetIndexSpeed(p, sleepTableSize / 2);

    // Initialize SDL2
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Create a window and renderer
    p->window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p->width, p->height, SDL_WINDOW_SHOWN);
    p->renderer = SDL_CreateRenderer(p->window, -1, SDL_RENDERER_ACCELERATED);
    p->texture = SDL_CreateTexture(p->renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, p->width, p->height);

    p->buffer = malloc(sizeof(uint8_t) * p->size);
    p->rgba = malloc(sizeof(uint8_t) * 4);
    
    pixelSetColor(p, 0, 0, 0, 255);

    return p;
}

void pixelSetIndexSpeed(struct Pixel *p, size_t index){
    p->sleepIndex = index;

    p->sleepDuration.tv_nsec = sleepTable[p->sleepIndex] * 1000000;
    p->sleepDuration.tv_sec = p->sleepDuration.tv_nsec / 1000000000;
    p->sleepDuration.tv_nsec %= 1000000000;
}

void pixelSetSpeed(struct Pixel *p, bool speed){
    if(speed){
        pixelSetIndexSpeed(p, sleepTableSize - 1);
    }
    else{
        pixelSetIndexSpeed(p, 0);
    }
}

int pixelEvents(struct Pixel *p){
    while(SDL_PollEvent(&p->event)){
        if(p->event.type == SDL_KEYDOWN){
            SDL_Keycode key = p->event.key.keysym.sym;
            if(key == SDLK_q || key == SDLK_ESCAPE){
                p->run = false;
                return PIXEL_EXIT;
            }
            else if(key == SDLK_RETURN){
                return PIXEL_RUN;
            }
            else if(key == SDLK_s){
                FILE *file;
                
                char fileName[50];
                snprintf(fileName, 50, "screenShot%ld.raw", (long)time(NULL));

                file = fopen(fileName, "w");
                if(file == NULL){
                    fprintf(stderr, "file \"%s\" couldn't be opened\n");
                }
                else{
                    fwrite(p->buffer, sizeof(uint8_t), p->size, file);
                    fclose(file);
                    printf("\"%s\" saved\n", fileName);
                }
            }
        }
        else if(p->event.type == SDL_KEYUP){
            const SDL_Keycode key = p->event.key.keysym.sym;
            
            if(key == SDLK_0){
                pixelSetSpeed(p, 0);
            }
            else if(key == SDLK_9){
                pixelSetSpeed(p, 1);
            }
            else if(key == SDLK_DOWN){
                if(0 < p->sleepIndex){
                    pixelSetIndexSpeed(p, p->sleepIndex - 1);
                }
            }
            else if(key == SDLK_UP){
                if(p->sleepIndex < sleepTableSize - 1){
                    pixelSetIndexSpeed(p, p->sleepIndex + 1);
                }
            }
        }
        else if(p->event.type == SDL_QUIT){
            p->run = false;
            return PIXEL_EXIT;
        }
    }

    return PIXEL_CONTINUE;
}

void pixelWait(struct Pixel *p){
    nanosleep(&p->sleepDuration, NULL);
}

void pixelWaitExit(struct Pixel *p){
    while(pixelEvents(p) != PIXEL_EXIT && p->run){}
}

void pixelFree(struct Pixel *p){
    free(p->buffer);
    free(p->rgba);

    SDL_DestroyRenderer(p->renderer);
    SDL_DestroyWindow(p->window);
    SDL_Quit();
}

void pixelSetColor(struct Pixel *p, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    p->rgba[0] = r;
    p->rgba[1] = g;
    p->rgba[2] = b;
    p->rgba[3] = a;
}

void pixelUpdate(struct Pixel *p){
    SDL_UpdateTexture(p->texture, NULL, p->buffer, 4 * p->width);
    // SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
    SDL_RenderCopy(p->renderer, p->texture, NULL, NULL);

    SDL_RenderPresent(p->renderer);

    #ifdef CAPTURE_ON
    char fileName[6];
    const char folderName[11] = "./Capture/";
    char filePath[21];
    if(capture == 2){
        struct stat st = {0};
        if(stat(folderName, &st) == -1){
            mkdir(folderName, 0777);
            printf("Folder was made\n");
        }
        capture = 1;
    }
    if(capture == 1){
        // TODO: Posible integer overflow
        // uint16_t = 2 ^ 16 - 1 => max number of didgets is 5 + \0
        snprintf(fileName, 6, "%05hd", captureIndex++);
        strncpy(filePath, folderName, 11);
        strncat(filePath, fileName, 6);
        strncat(filePath, ".png", 5);
        // printf("Path: %s\n", filePath);
        savePng(filePath, buffer, WINDOWWIDTH, WINDOWHEIGHT);
    }
    #endif
}

#define PixelWait() {\
    pixelUpdate(p); \
    if(pixelEvents(p) == PIXEL_EXIT){\
        exit(EXIT_SUCCESS); \
    }\
    pixelWait(p); \
} \

void pixelClear(struct Pixel *p){
    for(int i = 0; i < p->size; i++){
        p->buffer[i] = 0;
    }
}

void pixelFill(struct Pixel *p){
    for(size_t y = 0; y < p->height; y++){
        for(size_t x = 0; x < p->width; x++){
            const size_t i = 4 * (y * p->width + x);

            p->buffer[i + 0] = p->rgba[0];
            p->buffer[i + 1] = p->rgba[1];
            p->buffer[i + 2] = p->rgba[2];
            p->buffer[i + 3] = p->rgba[3];
        }
    }
}

void pixelDraw(struct Pixel *p, int x, int y){
    if(x < 0 || p->width <= x || y < 0 || p->height <= y){
        return;
    }

    const size_t i = 4 * (y * p->width + x);

    p->buffer[i + 0] = p->rgba[0];
    p->buffer[i + 1] = p->rgba[1];
    p->buffer[i + 2] = p->rgba[2];
    p->buffer[i + 3] = p->rgba[3];
}

void drawLine(struct Pixel *p, int x0, int y0, int x1, int y1){
    int dx =  abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */
 
    for(;;){
        pixelDraw(p, x0, y0);
        if(x0 == x1 && y0 == y1){ break; }
        e2 = 2 * err;
        if(e2 >= dy){ err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if(e2 <= dx){ err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}

double calculateResolution(int radius){
    return 360.0 / (2.0 * abs(radius) * M_PI);
}

void drawCircle(struct Pixel *p, int x, int y, int radius){
    double resolution = calculateResolution(radius);
    for(double i = 0.0; i < 360.0; i += resolution){
        int angle = i;
        int x1 = radius * cos(angle * M_PI / 180.0);
        int y1 = radius * sin(angle * M_PI / 180.0);
        pixelDraw(p, x + x1, y + y1);
    }
}

void drawRectangle(struct Pixel *p, int x, int y, int width, int height){
    for(int i = 0; i < height; i++){
        drawLine(p, x, y + i, x + width, y + i);
    }
}

#endif
