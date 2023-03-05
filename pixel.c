float cenX = 0.0;
float cenY = 0.0;
float scale = 1.0;

void drawPoint(SDL_Renderer *renderer, int x, int y){
    SDL_RenderDrawPoint(renderer, x, y);
}

void drawFromBuffer(SDL_Renderer *renderer, int *buffer, int width, int height){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            int index = 4 * (width * x + y);
            SDL_SetRenderDrawColor(renderer, buffer[index + 0], buffer[index + 1], buffer[index + 2], buffer[index + 3]);
            SDL_RenderDrawPoint(renderer, x, y);
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

// Functions
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
