// gcc main.c $(sdl2-config --cflags --libs) -o Build/main && ./Build/main

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "pixel.c"

#define windowWidth 800
#define windowHeight 600

const int FRAME_RATE = 60;
float deltaTime;

struct Ball{
    int x;
    int y;
    int r;
    
    double vectorX;
    double vectorY;
};

struct Platform{
    int width;
    int height;
    int speed;
    int x;
    int y;
};

//     a
//    ----
// f |    | b
//    ----
// e | g  | c
//    ---- 
//     d

int numbers[10][7] = {
//   a, b, c, d, e, f, g
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}, // 9
};

int score[2] = {0, 0};

void drawSegmentNumber(SDL_Renderer *renderer, int n, int x, int y, int origin){
    if(n < 0 || 9 < n){
        return;
    }

    int width = 8;
    int size = 60;

    if(origin == 1){
        x -= size;
    }
    else if(origin == 2){
        x -= size;
        y += size * 2;
    }
    else if(origin == 3){
        y += size * 2;
    }
    else if(origin == 4){
        x -= size / 2;
        y -= size / 2;
    }

    for(int i = 0; i < 7; i++){
        // printf("%i,", i);
        if(numbers[n][i] != 1){
            continue;
        }
        if(i % 3 == 0){
            // calculate offset; d will be i 3 and g will be 6 thats need to be fliped zero needs to be handeled seperately
            // i = 0 => 0
            // i = 3 => 6
            // i = 6 => 3
            int heightOffset = (i == 0) ? 0 : ((9 - i) / 3) * (size - width);
            drawRectangle(renderer, width + x, y + heightOffset, size - 2 * width, width);
        }
        else{
            int widthOffset  = (i < 3) ? size - width: 0;
            int heightOffset = (1 < i && i < 5) ? size : width;

            // printf("N: %i x: %i y:%", i);
            drawRectangle(renderer, x + widthOffset, y + heightOffset, width, size - 2 * width);
        }
    }
    // printf("\n");
}

void drawUi(SDL_Renderer *renderer){
    // Draw doted line in middle
    int width = 10;
    int count = windowHeight / width / 2;
    for(int i = 0; i < count * 2; i++){
        int h = i * 2 * width;
        drawRectangle(renderer, windowWidth / 2 - width / 2, h, width, width);
    }

    // Draw score
    int enemyS = score[0];
    int playerS = score[1];
    drawSegmentNumber(renderer, enemyS, windowWidth / 4 * 1, windowHeight / 8 * 1, 4);
    drawSegmentNumber(renderer, playerS, windowWidth / 4 * 3, windowHeight / 8 * 1, 4);
}

double map(double x, double inMin, double inMax, double outMin, double outMax){
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

void bounce(struct Ball *ball, struct Platform *platform){
    if(abs(platform->x - ball->x) < (ball->r + platform->width) / 2 && abs(platform->y - ball->y) < (ball->r + platform->height) / 2){
        // line:        x = 1
        // parabola:    x^2 + y = 1
        // 1^2 + y = 1  /-1- y
        // y = 0
        int offset = ball->y - platform->y;
        double x = map(abs(offset), 0, platform->height / 2, 0, 1);
        double y = 1.00 - x * x;
        printf("%i => %f y: %f\n", offset, x, y);
        ball->vectorX *= -1;
        // ball->vectorY = (offset < 0) ? y : -y;
        ball->vectorY = (offset < 0) ? -y : y;
    }
}

void gameLoop(void *param){
    SDL_Renderer *renderer = *((SDL_Renderer**) param);

    // Get the playerPlatform variable from the void pointer
    struct Platform *playerPlatform = *((struct Platform**) (param + sizeof(SDL_Renderer*)));
    struct Platform *enemyPlatform = *((struct Platform**) (param + sizeof(SDL_Renderer*) + sizeof(struct Platform*)));
    struct Ball *ball = *((struct Ball**) (param + sizeof(SDL_Renderer*) + sizeof(struct Platform*) * 2));

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    drawUi(renderer);

    // Draw both platforms
    drawRectangle(renderer, playerPlatform->x - playerPlatform->width / 2, playerPlatform->y - playerPlatform->height / 2, playerPlatform->width, playerPlatform->height);
    drawRectangle(renderer, enemyPlatform->x - enemyPlatform->width / 2, enemyPlatform->y - enemyPlatform->height / 2, enemyPlatform->width, enemyPlatform->height);
    
    // Move ball
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    drawRectangle(renderer, ball->x - ball->r / 2, ball->y - ball->r / 2, ball->r, ball->r);

    // Move items
    ball->x += ball->vectorX;
    ball->y += ball->vectorY;
    
    // Update enemyPlatform
    int offset = ball->y - enemyPlatform->y;
    if(enemyPlatform->height / 2 < abs(offset)){
        // move
        int direction = (offset < 0) ? -1 : 1;
        enemyPlatform->y += enemyPlatform->speed * direction * deltaTime * 10;
    }

    // Check for collision
    // TODO: implement SAT
    bounce(ball, enemyPlatform);
    bounce(ball, playerPlatform);
    
    // Check for possible point increase
    int halfWidth = ball->r / 2;
    int leftSide = ball->x - halfWidth < 0;
    int rigthSide = windowWidth < ball->x + halfWidth;
    if(leftSide || rigthSide){
        int i = (leftSide) ? 1 : 0;
        score[i] += 1;
        ball->x = windowWidth / 2;
        ball->y = windowHeight / 2;
        ball->vectorX *= -1;
        ball->vectorY *= -1;
    }
    
    SDL_RenderPresent(renderer);
}

int checkWin(SDL_Renderer *renderer){
    // Check if somebody won
    if(9 < score[0] || 9 < score[0]){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        return 1;
    }

    return 0;
}

int main(int argc, char **argv){
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
    }

    // Create a window and renderer
    window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Setup the game
    struct Ball ball = {windowWidth / 2, windowHeight / 2, 20, 14, 0};
    
    struct Platform enemyPlatform = {14, 90, 50, windowWidth / 8 * 1, windowHeight / 2};
    struct Platform playerPlatform = {14, 90, 50, windowWidth / 8 * 7, windowHeight / 2};
    
    // Pass arguments variables as a void pointer
    void *param = malloc(sizeof(SDL_Renderer*) + sizeof(struct Platform*) * 2 + sizeof(struct Ball*));
    *((SDL_Renderer**) param) = renderer;
    *((struct Platform**) (param + sizeof(SDL_Renderer*))) = &playerPlatform;
    *((struct Platform**) (param + sizeof(SDL_Renderer*) + sizeof(struct Platform*))) = &enemyPlatform;
    *((struct Ball**) (param + sizeof(SDL_Renderer*) + sizeof(struct Platform*) * 2)) = &ball;
    
    // Initialize frame timing variables
    Uint32 lastFrameTime = SDL_GetTicks();
    Uint32 frameTime;

    int run = 1;
    int move = 0;
    while(run){
        // Calculate frame timing
        frameTime = SDL_GetTicks() - lastFrameTime;
        deltaTime = (float)frameTime / 1000.0f;
        if (deltaTime < 1.0f / FRAME_RATE) {
            SDL_Delay((1.0f / FRAME_RATE - deltaTime) * 1000.0f);
            frameTime = SDL_GetTicks() - lastFrameTime;
            deltaTime = (float)frameTime / 1000.0f;
        }
        lastFrameTime = SDL_GetTicks();
        
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                SDL_Keycode key = event.key.keysym.sym;
                // printf("Type: %u\n", key);
                if(key == SDLK_q){
                    run = 0;
                }
                else if(key == SDLK_s ||key == SDLK_DOWN){
                    if(playerPlatform.y + playerPlatform.height / 2 + playerPlatform.speed < windowHeight){
                        move = playerPlatform.speed;
                    }
                }
                else if(key == SDLK_w || key == SDLK_UP){
                    if(0 < playerPlatform.y - playerPlatform.height / 2 - playerPlatform.speed){
                        move = -playerPlatform.speed;
                    }
                }
            }
            else if(event.type == SDL_KEYUP){
                SDL_Keycode key = event.key.keysym.sym;
                if(key == SDLK_s ||key == SDLK_DOWN || key == SDLK_w || key == SDLK_UP){
                    move = 0;
                }
            }
            else if(event.type == SDL_QUIT){
                run = 0;
            }
        }
        // Update playerPlatform
        playerPlatform.y += move * deltaTime * 10;
        
        // TODO: write it smarter
        // TODO: add win screen
        int r = checkWin(renderer);
        if(r){
            ball.vectorX = 0;
            ball.vectorY = 0;
        }
        else{
            gameLoop(param);
        }
    }
    
    printf("Param\n");
    free(param);
    printf("Renderer\n");
    SDL_DestroyRenderer(renderer);
    printf("Window\n");
    SDL_DestroyWindow(window);
    printf("Quit\n");
    SDL_Quit();

    printf("0\n");
    return 0;
}
