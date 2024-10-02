#define DELTA_IMPLEMENTATION
#include <delta.h>

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Hello World\n");

    deltaWindow* window = deltaCreateWindow("Delta Test Window", 100, 100, DELTA_WINDOW_SHOWN);

    while (1) {
        deltaUpdateWindow(window);
    }

    return 0;
}