#define DELTA_IMPLEMENTATION
#include <delta.h>

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Hello World\n");

    deltaWindow* window = deltaCreateWindow("Delta Test Window", 1000, 700, DELTA_WINDOW_SHOWN);

    while (deltaWindowShouldClose(window) == 0) {
        deltaUpdateWindow(window);
    }
    deltaDestroyWindow(window);
    return 0;
}