// gcc main.c $(sdl2-config --cflags --libs) -lm -o Build/main && ./Build/main

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include <math.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

int *HSVtoRGB(float H, float S, float V){
    int *returnArray = malloc(sizeof(int) * 3);

    if(H > 360 || H < 0 || S > 100 || S < 0 || V > 100 || V < 0){
        return returnArray;
    }

    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;

    if(H >= 0 && H < 60){
        r = C;
        g = X;
        b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X;
        g = C;
        b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0;
        g = C;
        b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0;
        g = X;
        b = C;
    }
    else if(H >= 240 && H < 300){
        r = X;
        g = 0;
        b = C;
    }
    else{
        r = C;
        g = 0;
        b = X;
    }

    returnArray[0] = (int)(r + m) * 255;
    returnArray[1] = (int)(g + m) * 255;
    returnArray[2] = (int)(b + m) * 255;

    return returnArray;
}

float cenX = 0.0;
float cenY = 0.0;
float scale = 1.0;

void draw(SDL_Renderer *renderer, int *buffer, int width, int height){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            int index = 4 * (width * x + y);
            SDL_SetRenderDrawColor(renderer, buffer[index + 0], buffer[index + 1], buffer[index + 2], buffer[index + 3]);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

long double distance(long double x1, long double y1, long double x2, long double y2){
    long double x = x2 - x1;
    long double y = y2 - y1;

    long double s = sqrt(x * x + y * y);
    
    return s;
}

int pixelToPoint(long double *points, int x, int y, int width, int height){
    long double px = (x - width / 2.0) * (4.0 / width) * (1.0 / scale) + cenX;
    long double py = (y - height / 2.0) * (4.0 / height) * (1.0 / scale) + cenY;
    
    points[0] = px;
    points[1] = py;

    return 0;
}

void calculatePoint(int *result, long double cx, long double cy){
    long double zx = 0;
    long double zy = 0;

    int i = 0;

    float bounds = 2.0;
    int isIn = 1;
    int iterations = 100;

    while(i < iterations && isIn == 1){
        long double x = zx * zx - zy * zy + cx;
        long double y = 2 * zx * zy + cy;
        zx = x;
        zy = y;
        i++;
        long double d = distance(0.0, 0.0, zx, zy);
        if(d > bounds){
            isIn = 0;
        }
    }

    result[0] = i;
    result[1] = isIn;
}

int mandelbrotSet(int *buffer, int width, int height){
    long double c[2] = {0.0, 0.0};
    int result[2] = {0, 0};

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            int index = 4 * (width * x + y);
            
            pixelToPoint(c, x, y, width, height);
            calculatePoint(result, c[0], c[1]);

            int r, g, b, a;
            if(result[1] == 1){
                r = 0; g = 0; b = 0;
            }
            else if(result[0] > 1){
                // float h = 150 + 200 - fmod(pow(result[0]/(70), 0.5) * 200, 255);
                // float s = 100;
                // float v = 100;
                // int * rgb = HSVtoRGB(h, s, v);
                // r = rgb[0];
                // g = rgb[1];
                // b = rgb[2];
                r = 255; g = 255; b = 255;
            }
            else{
                r = 115; g = 115; b = 115;
            }

            *(buffer + index + 0) = r;
            *(buffer + index + 1) = g;
            *(buffer + index + 2) = b;

            *(buffer + index + 3) = 255;
        }
    }

    return 0;
}

int main(){
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    // Create a window and renderer
    window = SDL_CreateWindow("Mandelbrot set", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    int *buffer = malloc(sizeof(int) * WINDOW_WIDTH * WINDOW_HEIGHT * 4);

    printf("Generating mandelbrot set...\n");
    mandelbrotSet(buffer, WINDOW_WIDTH, WINDOW_HEIGHT);

    printf("Drawing mandelbrot set..\n");
    draw(renderer, buffer, WINDOW_WIDTH, WINDOW_HEIGHT);

    printf("Mandelbrot set was drawn..\n");
    
    SDL_RenderPresent(renderer);
    free(buffer);

    int run = 1;
    while(run){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                SDL_Keycode key = event.key.keysym.sym;
                if(key == SDLK_q || key == SDLK_ESCAPE){
                    run = 0;
                }
            }
            else if(event.type == SDL_QUIT){
                run = 0;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
