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


int hsv2rgb(float h, float s, float v, int *r, int *g, int *b){
    if(1.0 < v){
        v = 1.0;
    } 
        
    float hp = h / 60.0;
    float c = v * s;
    float x = c * (1.0 - abs(((int)hp % 2) - 1.0));
    float rgb[3] = {0};

    if(0 <= hp && hp < 1){
        rgb[0] = c;
        rgb[1] = x;
    }
    if(1 <= hp && hp < 2){
        rgb[0] = x;
        rgb[1] = c;
    }
    if(2 <= hp && hp < 3){
        rgb[1] = c;
        rgb[2] = x;
    }
    if(3 <= hp && hp < 4){
        rgb[1] = x;
        rgb[2] = c;
    }
    if(4 <= hp && hp < 5){
        rgb[0] = x;
        rgb[2] = c;
    }
    if(5 <= hp && hp < 6){
        rgb[0] = c;
        rgb[2] = x;
    }

    float m = v - c;
    rgb[0] += m;
    rgb[1] += m;
    rgb[2] += m;

    *r = rgb[0] * 255;
    *g = rgb[1] * 255;
    *b = rgb[2] * 255;
}

#endif
