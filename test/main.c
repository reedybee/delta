#define DELTA_IMPLEMENTATION
#include <delta.h>

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Hello World\n");

    if (!InitDelta())
        return 1;
    
    return 0;
}