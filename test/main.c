#include <glad/glad.h>

#define DELTA_IMPLEMENTATION
#include <delta.h>

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Hello World\n");

    deltaWindow* window = deltaCreateWindow("Delta Test Window", 1000, 700, DELTA_WINDOW_SHOWN | DELTA_WINDOW_OPENGL);

    deltaSetOpenGLContext(4, 6);

    if (!gladLoadGLLoader((GLADloadproc)deltaGetProcAddress)) {
        printf("Failed to initialize glad\n");
    }

    while (!deltaWindowShouldClose(window)) {
        deltaUpdateWindow(window);
    }
    deltaDestroyWindow(window);
    return 0;
}