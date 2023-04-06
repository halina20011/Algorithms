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

#ifndef RGBACOLOR
#define RGBACOLOR

struct RGBA{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

void rgbaInit(struct RGBA *color, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}

struct RGBA black   = {  0,   0,   0, 255};
struct RGBA white   = {255, 255, 255, 255};
struct RGBA red     = {255,   0,   0, 255};
struct RGBA green   = {  0, 255,   0, 255};

int hsv2rgb(float H, float S, float V, int *r, int *g, int *b){
    if(H < 0 || 360 < H || S < 0 || 100 < S || V < 0 || 100 < V){
        *r = 0;
        *g = 0;
        *b = 0;
        return 1;
    }

    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float _r, _g, _b;

    if(H >= 0 && H < 60){
        _r = C;
        _g = X;
        _b = 0;
    }
    else if(H >= 60 && H < 120){
        _r = X;
        _g = C;
        _b = 0;
    }
    else if(H >= 120 && H < 180){
        _r = 0;
        _g = C;
        _b = X;
    }
    else if(H >= 180 && H < 240){
        _r = 0;
        _g = X;
        _b = C;
    }
    else if(H >= 240 && H < 300){
        _r = X;
        _g = 0;
        _b = C;
    }
    else{
        _r = C;
        _g = 0;
        _b = X;
    }

    *r = (int)(_r + m) * 255;
    *g = (int)(_g + m) * 255;
    *b = (int)(_b + m) * 255;

    return 0;
}
#endif
