void drawPoint(SDL_Renderer *renderer, int x, int y){
    SDL_RenderDrawPoint(renderer, x, y);
}

void drawPoints(SDL_Renderer *renderer, float ** field, int columns, int rows, int resolution){
    for(int i = 0; i < columns; i++){
        for(int j = 0; j < rows; j++){
            if(field[i][j] >= 1){
                // renderer,                  x,              y
                SDL_RenderDrawPoint(renderer, j * resolution, i * resolution);
            }
        }
    }
}

void drawLine(SDL_Renderer *renderer, int x0, int y0, int x1, int y1){
    int dx =  abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */
 
   for(;;){
        drawPoint(renderer, x0, y0);
        if(x0 == x1 && y0 == y1){ break; }
        e2 = 2 * err;
        if(e2 >= dy){ err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if(e2 <= dx){ err += dx; y0 += sy; } /* e_xy+e_y < 0 */
   }
}

void drawRectangle(SDL_Renderer *renderer, int x, int y, int width, int height){
    for(int i = 0; i < height; i++){
        drawLine(renderer, x, y + i, x + width, y + i);
    }
}
