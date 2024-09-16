#ifndef VERTEX
#define VERTEX

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "color.h"

#define VERTEX_DATA         3
#define VERTEX_COLORS       3
#define VERTEX_SIZE         (VERTEX_DATA + VERTEX_COLORS)
#define VERTICES_PER_ITEM   6
#define BUFFER_SIZE         (VERTEX_SIZE * VERTICES_PER_ITEM)
#define BUFFER_BYTES_SIZE   (BUFFER_SIZE * sizeof(float))

#define RED_INDEX       2
#define GREEN_INDEX     3
#define BLUE_INDEX      4

struct Vertex{
    GLFWwindow *window;
    GLuint vbo; // vertex buffer object

    struct timespec sleepDuration;
    size_t sleepIndex;

    bool run;
};

#define REGULAR         ((struct Color){1.0, 1.0, 1.0});
#define SWAPED          ((struct Color){1.0, 0.0, 0.0});
#define HIGHTLIGHTED    ((struct Color){0.0, 1.0, 1.0});

struct Point{
    float x, y, c;
};

struct Number{
    int val;
    float buffer[BUFFER_SIZE];
    float x;
    // struct Point p1, p2, p4, p3;
    // int val;
    // float height;
    // struct Color type;
};

struct Vertex *vertexInit(const char windowName[], float w);
void vertexFree();

void updateIndex(struct Vertex *v, size_t i);
void vertexUpdate(struct Vertex *v);

#define VertexWait() {\
    update(); \
    if(v->run){\
        exit(EXIT_SUCCESS); \
    }\
    vertexWait(v); \
} \


void updateBuffer();


void swap(size_t i1, size_t i2);
void highlight(size_t index);
void setColor(size_t index, struct Color color);
void clearColors(void);

void draw();
void sleepFunc(float duration);

void vertexWait(struct Vertex *v);
void vertexWaitExit(struct Vertex *v);

void vertexSetSpeed(struct Vertex *v, bool speed);

void drawRectangle(int x, int y, int width, int height);

#endif
