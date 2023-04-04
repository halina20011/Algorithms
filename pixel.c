#ifndef PIXEL
#define PIXEL

extern const int WINDOWWIDTH;
extern const int WINDOWHEIGHT;

extern SDL_Renderer *renderer;

int rgba[4] = {0};

int setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
}

int newPixelBuffer(uint8_t **output){
    *output = malloc(sizeof(uint8_t) * (WINDOWWIDTH * WINDOWHEIGHT * 4));
    if(output == NULL){
        return 1;
    }

    return 0;
}

void update(uint8_t *buffer){
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, WINDOWWIDTH, WINDOWHEIGHT);
    SDL_UpdateTexture(texture, NULL, buffer, 4 * WINDOWWIDTH);
    // SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
}

void clear(uint8_t **buffer){
    for(int i = 0; i < WINDOWWIDTH * WINDOWHEIGHT * 4; i++){
        *(*buffer + i) = 0;
    }
}

void drawPixel(uint8_t **buffer, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    if(WINDOWWIDTH <= x || WINDOWHEIGHT <= y){
        return;
    }

    int i = 4 * (y * WINDOWWIDTH + x);

    *(*buffer + i + 0) = r;
    *(*buffer + i + 1) = g;
    *(*buffer + i + 2) = b;
    *(*buffer + i + 3) = a;
}

void drawLine(uint8_t **buffer, int x0, int y0, int x1, int y1){
    int dx =  abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */
 
    for(;;){
        drawPixel(buffer, x0, y0, rgba[0], rgba[1], rgba[2], rgba[3]);
        if(x0 == x1 && y0 == y1){ break; }
        e2 = 2 * err;
        if(e2 >= dy){ err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if(e2 <= dx){ err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}

void drawRectangle(uint8_t **buffer, int x, int y, int width, int height){
    for(int i = 0; i < height; i++){
        drawLine(buffer, x, y + i, x + width, y + i);
    }
}

#endif
