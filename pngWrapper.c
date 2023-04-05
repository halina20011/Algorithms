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

#ifndef PNGWRAPPER
#define PNGWRAPPER

#include <png.h>

int savePng(const char *fileName, const unsigned char *pixels, int width, int height){
    FILE *fp;
    png_structp pngPtr;
    png_infop infoPtr;
    png_bytep rowPointers[height];

    fp = fopen(fileName, "wb");
    if(!fp){
        return 1;
    }

    pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!pngPtr){
        fclose(fp);
        return 1;
    }

    infoPtr = png_create_info_struct(pngPtr);
    if(!infoPtr){
        png_destroy_write_struct(&pngPtr, NULL);
        fclose(fp);
        return 1;
    }

    if(setjmp(png_jmpbuf(pngPtr))){
        png_destroy_write_struct(&pngPtr, &infoPtr);
        fclose(fp);
        return 1;
    }

    png_set_IHDR(pngPtr, infoPtr, width, height,
        8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    for(int y = 0; y < height; y++){
        rowPointers[y] = (png_bytep)(pixels + y * width * 4);
    }

    png_init_io(pngPtr, fp);
    png_set_rows(pngPtr, infoPtr, rowPointers);
    png_write_png(pngPtr, infoPtr, PNG_TRANSFORM_IDENTITY, NULL);

    png_destroy_write_struct(&pngPtr, &infoPtr);
    fclose(fp);

    return 0;
}

int readPng(char* fileName, unsigned char **pixels, int *width, int *height) {
    FILE *fp = fopen(fileName, "rb");
    if(!fp){
        fprintf(stderr, "Error: failed to open PNG file %s\n", fileName);
        return 1;
    }

    // Create a PNG read struct
    png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!pngPtr){
        fprintf(stderr, "Error: failed to create PNG read struct\n");
        fclose(fp);
        return 1;
    }

    // Create a PNG info struct
    png_infop pngInfo = png_create_info_struct(pngPtr);
    if(!pngInfo){
        fprintf(stderr, "Error: failed to create PNG info struct\n");
        png_destroy_read_struct(&pngPtr, NULL, NULL);
        fclose(fp);
        return 1;
    }

    // Set up error handling
    if(setjmp(png_jmpbuf(pngPtr))){
        fprintf(stderr, "Error: failed to read PNG file %s\n", fileName);
        png_destroy_read_struct(&pngPtr, &pngInfo, NULL);
        fclose(fp);
        return 1;
    }

    // Initialize PNG I/O
    png_init_io(pngPtr, fp);

    // Read PNG header
    png_read_info(pngPtr, pngInfo);

    // Get image attributes
    *width = png_get_image_width(pngPtr, pngInfo);
    *height = png_get_image_height(pngPtr, pngInfo);
    png_byte color_type = png_get_color_type(pngPtr, pngInfo);
    png_byte bit_depth = png_get_bit_depth(pngPtr, pngInfo);

    // Expand palette images to RGB
    if(color_type == PNG_COLOR_TYPE_PALETTE){
        png_set_palette_to_rgb(pngPtr);
    }

    // Expand grayscale images to 8 bits
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8){
        png_set_expand_gray_1_2_4_to_8(pngPtr);
    }

    // Expand bit depths < 8 to 8
    if(bit_depth < 8){
        png_set_packing(pngPtr);
    }

    if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE){
        png_set_filler(pngPtr, 0xFF, PNG_FILLER_AFTER);
    }

    if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
        png_set_gray_to_rgb(pngPtr);
    }
    
    // Update PNG info struct
    png_read_update_info(pngPtr, pngInfo);

    // Allocate memory for pixel data
    size_t rowBytes = png_get_rowbytes(pngPtr, pngInfo);
    *pixels = (unsigned char*)malloc(rowBytes * (*height));

    // Set up row pointers
    png_bytep* rowPointers = (png_bytep*)malloc((*height) * sizeof(png_bytep));
    for(int y = 0; y < (*height); y++){
        rowPointers[y] = *pixels + y * rowBytes;
    }

    // Read image data
    png_read_image(pngPtr, rowPointers);

    // Clean up memory and close the file
    png_destroy_read_struct(&pngPtr, &pngInfo, NULL);
    free(rowPointers);
    fclose(fp);

    return 0;
}

#endif
