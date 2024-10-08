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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    while (!deltaWindowShouldClose(window)) {
    
        if (deltaGetKey(DELTA_KEY_1))
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        if (deltaGetKey(DELTA_KEY_2))
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        if (deltaGetKey(DELTA_KEY_3))
            glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        deltaUpdateWindow(window);  
    }
    deltaDestroyWindow(window);
    
    return 0;
}