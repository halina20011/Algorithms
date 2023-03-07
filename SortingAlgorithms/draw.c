#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int drawFinalAnimation(SDL_Renderer *renderer, int *numbers, int length, int width){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int i = 0; i < length; i++){
        int x = i * width;
        if(i <= indexAnimation){
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            drawRectangle(renderer, x, WINDOW_HEIGHT - numbers[i], width, numbers[i]);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        else{
            drawRectangle(renderer, x, WINDOW_HEIGHT - numbers[i], width, numbers[i]);
        }
    }

    SDL_RenderPresent(renderer);
    
    return 0;
}

int drawNumbers(SDL_Renderer *renderer, int *numbers, int length, int width, int _j){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int i = 0; i < length; i++){
        int x = i * width;
        if(_j == i || _j + 1 == i){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            drawRectangle(renderer, x, WINDOW_HEIGHT - numbers[i], width, numbers[i]);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        else{
            drawRectangle(renderer, x, WINDOW_HEIGHT - numbers[i], width, numbers[i]);
        }
    }

    SDL_RenderPresent(renderer);
    
    return 0;
}
