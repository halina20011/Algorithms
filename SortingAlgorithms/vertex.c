#include "vertex.h"
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
// #define WINDOW_WIDTH 1366
// #define WINDOW_HEIGHT 768

// in milliseconds
const uint32_t sleepTable[10] = {1000, 500, 250, 100, 50, 10, 5, 2, 1, 0};
const size_t sleepTableSize = sizeof(sleepTable) / sizeof(sleepTable[0]);

GLint vertexShader;
GLint fragmentShader;
GLuint shaderProgram;

GLint colorUniform;
GLint heightUniform;
GLint indexUniform;

GLint uniColor;

GLFWwindow *w = NULL;
extern struct Number **numbers;
extern size_t numbersSize;

extern size_t bufferSize;
extern float *buffer;

extern float WIDTH;

char *readFile(const char *fileName){
    FILE *file = fopen(fileName, "rb");
    if(!file){
        fprintf(stderr, "unable to open file: %s\n", fileName);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(sizeof(char) * (fileSize + 1));
    fread(buffer, 1, fileSize, file);
    fclose(file);

    buffer[fileSize] = 0;

    return buffer;
}

GLint compileShader(char *fileName, int type){
    char *shaderSource = readFile(fileName);
    // printf("shader %s\n", shaderSource);
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char * const*)&shaderSource, NULL);
    glCompileShader(shader);
    free(shaderSource);

    char buffer[512];
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    glGetShaderInfoLog(shader, 512, NULL, buffer);
    if(status != GL_TRUE){
        fprintf(stderr, "failed to compile a shader:\n");
        fprintf(stderr, "%s\n", buffer);
        exit(1);
    }

    return shader;
}

// void vertexSetIndexSpeed(struct Vertex *v, size_t index){
//     v->sleepIndex = index;
//
//     v->sleepDuration.tv_nsec = sleepTable[v->sleepIndex] * 1000000;
//     v->sleepDuration.tv_sec = v->sleepDuration.tv_nsec / 1000000000;
//     v->sleepDuration.tv_nsec %= 1000000000;
// }
//
// void vertexSetSpeed(struct Vertex *v, bool speed){
//     if(speed){
//         vertexSetIndexSpeed(v, sleepTableSize - 1);
//     }
//     else{
//         vertexSetIndexSpeed(v, 0);
//     }
// }

void vertexKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    struct Vertex *v = glfwGetWindowUserPointer(window);

    if(key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE){
        v->run = false;
    }

    if(action == GLFW_RELEASE){
        // if(key == GLFW_KEY_0){
        //     vertexSetSpeed(v, 0);
        // }
        // else if(key == GLFW_KEY_9){
        //     vertexSetSpeed(v, 1);
        // }
        // else if(key == GLFW_KEY_DOWN){
        //     if(0 < v->sleepIndex){
        //         vertexSetIndexSpeed(v, v->sleepIndex - 1);
        //     }
        // }
        // else if(key == GLFW_KEY_UP){
        //     if(v->sleepIndex < sleepTableSize - 1){
        //         vertexSetIndexSpeed(v, v->sleepIndex + 1);
        //     }
        // }
        // else if(key == GLFW_KEY_SPACE){
        //     waitFor(v, GLFW_KEY_SPACE);
        // }
    }
}

struct Vertex *vertexInit(const char windowName[], float width){
    struct Vertex *v = malloc(sizeof(struct Vertex));
    v->run = true;

    if(glfwInit() != GL_TRUE){
        fprintf(stderr, "failed to initialize GLFW\n");
        exit(1);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);
    // GLFWwindow *window = glfwCreateWindow(800, 600, "Piano Tutor", glfwGetPrimaryMonitor(), NULL);
    w = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowName, NULL, NULL);
    v->window = w;
    glfwMakeContextCurrent(v->window);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        fprintf(stderr, "failed to initialize GLEW\n");
        exit(1);
    }

    glfwSetWindowUserPointer(v->window, v);
    glfwSetKeyCallback(v->window, vertexKeyCallback);
    
    // vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glClearColor(0, 0, 0, 0);

    // float def[12] = {
    //     -1, 1,
    //     -1,-1,
    //     -1 + width, 1,
    //     -1 + width,-1
    // };

    // float vertices[] = {
    //     // Position      // Height    // Color
    //     -0.5f, -0.5f,    1.0f,       1.0f, 0.0f, 0.0f,   // Vertex 1
    //      0.5f, -0.5f,    1.0f,       0.0f, 1.0f, 0.0f,   // Vertex 2
    //      0.5f,  0.5f,    1.0f,       0.0f, 0.0f, 1.0f,   // Vertex 3
    //     -0.5f,  0.5f,    1.0f,       1.0f, 1.0f, 0.0f    // Vertex 4
    // };
    // float vertices[] = {
    //     // Position    // Color
    //      -0.5f,  0.5f, 1.0f,// Vertex 3
    //       0.5f,  0.5f, 1.0f,// Vertex 4
    //      -0.5f, -0.5f, 1.0f,// Vertex 1
    //      0.5f, -0.5f,  1.0f,// Vertex 2
    // };
    // float vertices[] = {
    //     -1., 1., 1.,
    //     1., 1., 1.,
    //     -1., -1., .2,
    //     1., -1., 0.4,
    // };
    // struct Number vertices = {
    //     {-1, 1, 1},
    //     {1, 1, 1},
    //     {-1, -1, 1},
    //     {1, -1, 1},
    // };

    // create vertex buffer Object (VBO)
    glGenBuffers(1, &(v->vbo));
    glBindBuffer(GL_ARRAY_BUFFER, v->vbo); // make it active object
    // glBufferData(GL_ARRAY_BUFFER, numbers->size * sizeof(struct Number), numbers->number, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(sizeof(float) * 2));
    // glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(def), def, GL_STATIC_DRAW);
// #define POS(n) n, 2, GL_FLOAT, GL_FALSE, sizeof(struct Number)
// #define COLOR(n) n, 1, GL_FLOAT, GL_FALSE, sizeof(struct Number)
//     glVertexAttribPointer(POS(0), (void*)(offsetof(struct Number, p1.x)));
//     glVertexAttribPointer(COLOR(1), (void*)(offsetof(struct Number, p1.c)));
    // glVertexAttribPointer(POS(2), (void*)(offsetof(struct Number, p2.x)));
    // glVertexAttribPointer(COLOR(3), (void*)(offsetof(struct Number, p2.c)));
    // glVertexAttribPointer(POS(4), (void*)(offsetof(struct Number, p3.x)));
    // glVertexAttribPointer(COLOR(5), (void*)(offsetof(struct Number, p3.c)));
    // glVertexAttribPointer(POS(6), (void*)(offsetof(struct Number, p4.x)));
    // glVertexAttribPointer(COLOR(7), (void*)(offsetof(struct Number, p4.c)));
    // glEnableVertexAttribArray(2);
    // glEnableVertexAttribArray(3);
    // glEnableVertexAttribArray(4);
    // glEnableVertexAttribArray(5);
    // glEnableVertexAttribArray(6);
    // glEnableVertexAttribArray(7);

    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct Number), (void*)(offsetof(struct Number, position)));
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(struct Number), (void*)0);
    // glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 6*sizeof(struct Number), (void*)(2 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6*sizeof(struct Number), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    // compile shaders
    vertexShader = compileShader("vertexShader.vert", GL_VERTEX_SHADER);
    fragmentShader = compileShader("fragmentShader.frag", GL_FRAGMENT_SHADER);
    shaderProgram = glCreateProgram();
    
    // glBindFragDataLocation(shaderProgram, 0, "outColor");
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // colorUniform = glGetUniformLocation(shaderProgram, "color");
    // glUniform3f(colorUniform, 1, 1, 1);
    //
    // GLint widthUniform = glGetUniformLocation(shaderProgram, "width");
    // heightUniform = glGetUniformLocation(shaderProgram, "height");
    // indexUniform = glGetUniformLocation(shaderProgram, "index");
    //
    // glUniform1f(widthUniform, w);
    // glUniform1i(indexUniform, 10);
    // glUniform1f(heightUniform, 0);

    // GLFWwindow *window = v->window;
    // while(!glfwWindowShouldClose(window)){
    //     glfwPollEvents();
    //     if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    //         glfwSetWindowShouldClose(window, GL_TRUE);
    //     }
    //
    //     glClearColor(0, 0, 0, 0);
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //     glfwSwapBuffers(window);
    // }
    // while(1){
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     glBufferData(GL_ARRAY_BUFFER, sizeof(struct Number), &vertices, GL_STATIC_DRAW);
    //     glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //     glfwSwapBuffers(w);
    //     sleepFunc(1);
    // }
    return v;
}

void printPoint(struct Point p){
    printf("%f %f ", p.x, p.y);
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    // glBufferData(
    //         GL_ARRAY_BUFFER, 
    //         sizeof(struct Number) * numbersSize, numbers, GL_DYNAMIC_DRAW);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, numbersSize);
    glDrawArrays(GL_TRIANGLES, 0, numbersSize * VERTICES_PER_ITEM);
    // for(size_t i = 0; i < numbersSize; i++){
    //     struct Number n = numbers[i];
    //     printPoint(n.p1);
    //     printPoint(n.p2);
    //     printPoint(n.p3);
    //     printPoint(n.p4);
    //     printf("\n");
    // }
    glfwSwapBuffers(w);
    sleepFunc(0.1);
}

// p1 0 *0
// p2 1 *0
// p3 2 *1
//   
// p3 3 *1 
// p4 4 *1
// p2 5 *0

float sideOffset[] = {0, 0, 1, 1, 1, 0};
void updateNumber(struct Number *number, size_t i){
    for(size_t v = 0; v < VERTICES_PER_ITEM; v++){
        float x = i * WIDTH;
        float widthOffset = x + sideOffset[v] * WIDTH - 1;
        number->buffer[v * VERTEX_SIZE] = widthOffset;
    }
}

void updateBuffer(){
    memset(buffer, 0, BUFFER_BYTES_SIZE * numbersSize);
    for(size_t i = 0; i < numbersSize; i++){
        // printf("%p ", numbers[i]);
        // printf("%f ", numbers[i]->buffer[0]);
        printf("%i ", numbers[i]->val);
        updateNumber(numbers[i], i);
        memcpy(&buffer[i * BUFFER_SIZE], &numbers[i]->buffer, BUFFER_BYTES_SIZE);
    }
    printf("\n");
    printf("size: %zu\n", bufferSize);

    glBufferData(GL_ARRAY_BUFFER, bufferSize, buffer, GL_STATIC_DRAW);
}

void sleepFunc(float duration){
    // printf("eep\n");
    double start = glfwGetTime(), timer = start;
    while(timer < start + duration){
        // printf("%f\n", timer);
        timer = glfwGetTime();
        glfwPollEvents();
    }
    // printf("end\n");
}

void vertexFree(){
    glDeleteProgram(shaderProgram);

    glfwTerminate();
}

// void updateIndex(struct Vertex *v, size_t i){
//     struct Color c = numbers[i].type;
//     glUniform1i(indexUniform, i);
//     glUniform3f(colorUniform, 0, 0, 0);
//     glUniform1f(heightUniform, 0.0);
//     glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//     glUniform1f(heightUniform, -numbers[i].height);
//     glUniform3fv(colorUniform, 1, &c);
//     glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//     glfwSwapBuffers(v->window);
// }

// void vertexUpdate(struct Vertex *v){
//     glfwPollEvents();
//     glClear(GL_COLOR_BUFFER_BIT);
//     
//     for(size_t i = 0; i < numbersSize; i++){
//         struct Color c = numbers[i].type;
//         glUniform1i(indexUniform, i);
//         glUniform1f(heightUniform, -numbers[i].height);
//         glUniform3fv(colorUniform, 1, &c);
//         glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//     }
//
//     printf("updated\n");
//     glfwSwapBuffers(v->window);
// }

void vertexWait(struct Vertex *v){
    nanosleep(&(v->sleepDuration), NULL);
}

void vertexWaitExit(struct Vertex *v){
    while(v->run){
        glfwPollEvents();
    }
}

void vertexSetColor(double r, double g, double b){
    glUniform3f(uniColor, r, g, b);
}

void swap(size_t i1, size_t i2){
    // printf("swap %zu %zu\n", i1, i2);
    // printf("sizes %zu %zu\n", BUFFER_SIZE, BUFFER_BYTES_SIZE);
    // return;
    // struct Pointer *temp = numbers[];
    struct Number *m1 = numbers[i1];
    struct Number *m2 = numbers[i2];
    
    float *b1 = m1->buffer;
    float *b2 = m2->buffer;
    size_t b1Index = i1 * BUFFER_BYTES_SIZE;
    size_t b2Index = i2 * BUFFER_BYTES_SIZE;
    // printf("%zu %zu\n", b1Index, b2Index);
    // for(size_t j = 0; j < VERTICES_PER_ITEM; j++){
    //     for(size_t x = 0; x < VERTEX_SIZE; x++){
    //         printf("%f ", b1[j * VERTEX_SIZE + x]);
    //     }
    //     printf("\n");
    // }

    // struct Number *temp = numbers[i1];
    
    updateNumber(m1, i2);
    updateNumber(m2, i1);

    // printf("1 %p %p\n", numbers[i1], numbers[i2]);
    struct Number *temp = numbers[i1];
    numbers[i1] = numbers[i2];
    numbers[i2] = temp;
    // printf("2 %p %p\n", numbers[i1], numbers[i2]);
    
    // updateBuffer();
    glBufferSubData(GL_ARRAY_BUFFER, b1Index, BUFFER_BYTES_SIZE, b2);
    glBufferSubData(GL_ARRAY_BUFFER, b2Index, BUFFER_BYTES_SIZE, b1);
}

// TODO: fix
size_t colors[100] = {};
size_t colorsSize = 0;

void setColor(size_t index, struct Color color){
    for(size_t i = 0; i < VERTICES_PER_ITEM; i++){
        numbers[index]->buffer[i * VERTEX_SIZE + RED_INDEX] = color.r;
        numbers[index]->buffer[i * VERTEX_SIZE + GREEN_INDEX] = color.g;
        numbers[index]->buffer[i * VERTEX_SIZE + BLUE_INDEX] = color.b;
    }
    colors[colorsSize++] = index;
}

void clearColors(){
    for(size_t c = 0; c < colorsSize; c++){
        size_t index = colors[c]; 
        for(size_t v = 0; v < VERTICES_PER_ITEM; v++){
            numbers[index]->buffer[v * VERTEX_SIZE + RED_INDEX]     = 0.0f;
            numbers[index]->buffer[v * VERTEX_SIZE + GREEN_INDEX]   = 0.0f;
            numbers[index]->buffer[v * VERTEX_SIZE + BLUE_INDEX]    = 0.0f;
        }
    }
    colorsSize = 0;
}

void highlight(size_t index){
    // struct Number *number = numbers[index];
    setColor(index, RED);
}

// double toRad(double radius){
//     return (M_PI * radius) / 180.0;
// }
//
// double dAbs(double d){
//     if(d < 0){
//         return d * -1.0;
//     }
//
//     return d;
// }
//
// void rotate(double rad, double r, double *x, double *y){
//     double a = rad;
//     *x = r * cos(a);
//     *y = r * sin(a);
// }
//
// // returns rad
// double calcAngle(double _x1, double _y1, double _x2, double _y2){
//     double x1 = _x2 - _x1;
//     double y1 = _y2 - _y1;
//     return atan2(x1, y1);
// }
//
// void drawLine(struct Vertex *v, int x1, int y1, int x2, int y2, int width){
//     if(x1 < x2){
//         swap(&x1, &x2);
//         swap(&y1, &y2);
//     }
//
//     double w2 = (double)width / 2.0;
//     double angle = dAbs(calcAngle(x1, y1, x2, y2));
//     double A, B;
//     rotate(angle, w2, A, B);
//     const double aX = x1 + A, aY = y1 + B;
//     const double bX = x2 + A, bY = y2 + B;
//     const double cX = x2 - A, cY = y2 - B;
//     const double dX = x1 - A, dY = y1 - B;
//     // a           b
//     // |-----------|
//     // |           |
//     // |-----------|
//     // d           c
//     double buffer[] = {
//         aX, aY,
//         dX, dY,
//         cX, cY,
//
//         aX, aY,
//         bX, bY,
//         cX, cY
//     };
//
//     glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
//     glDrawArrays(GL_TRIANGLES, 0, 6); // primitiveType offset count
// }

void drawRectangle(int x, int y, int width, int height){
    // x,y         x + w, y
    // |-----------|
    // |           |
    // |-----------|
    // x, y + h    x + w, y + h
    const double aX = x,            aY = y;
    const double bX = x + width,    bY = y;
    const double cX = x,            cY = y + height;
    const double dX = x + width,    dY = y + height;
    double buffer[] = {
        aX, aY,
        bX, bY,
        cX, cY,

        bX, bY, 
        dX, dY,
        cX, cY,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6); // primitiveType offset count
}

// function drawCircle(gl, x, y, radius, resolution){
//     const degreeOffset = 360.0 / resolution;
//     let buffer = new Float32Array(2 * 3 * resolution);
//     let prevPoint = cPoint(0, x, y, radius);
//     for(let i = 1; i <= resolution; i++){
//         const thisPoint = cPoint(i * degreeOffset, x, y, radius);
//         const index = (i - 1) * 3 * 2;
//         buffer[index + 0] = prevPoint[0];
//         buffer[index + 1] = prevPoint[1];
//         buffer[index + 2] = thisPoint[0];
//         buffer[index + 3] = thisPoint[1];
//         buffer[index + 4] = x;
//         buffer[index + 5] = y;
//         prevPoint = thisPoint;
//     }
//     buffer = buffer.map(v => {
//         return v.toFixed(10);
//     });
//     // console.log(buffer);
//     gl.bufferData(gl.ARRAY_BUFFER, buffer, gl.STATIC_DRAW);
//     const primitiveType = gl.TRIANGLES;
//     const offset = 0;
//     const count = 3 * resolution;
//     gl.drawArrays(primitiveType, offset, count);
// }
//
// function toDeg(rad){
//     return (rad * 180) / Math.PI;
// }
//
// // dot product
