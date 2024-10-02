/* 
delta.h copyright (c) reedybee 2024
free to use, single file c library for window management
no warranty implied; use at your own risk
*/
#ifndef DELTA_H 
#define DELTA_H
// header
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// used to distinquish what is apart of the Delta api
#define DELAPI 
// used to distinguish what standard functions are being overriden
#define override

#define nullptr (void*)0
// unsigned long long int
#define size_d   size_t
// unsigned char
#define uint8_d  uint8_t
// unsigned short
#define uint16_d uint16_t
// unsigned int
#define uint32_d uint32_t
// unsigned long long int
#define uint64_d uint64_t
// signed char
#define int8_d  int8_t
// signed short
#define int16_d int16_t
// signed int
#define int32_d int32_t
// signed long long int
#define int64_d int64_t

#define DELTA_FAIL 0
#define DELTA_SUCCESS 1

#define DELTA_VERSION_MAJOR 0
#define DELTA_VERSION_MINOR 1

/* 
delta's malloc,
allocates a size of heap memory
returns a pointer to the newly allocated
memory, or nullptr if size is 0 
*/
void* override malloc_d(size_d size);
/*
delta's realloc
reallocates the given block of memory
returns a pointer to the new block,
returns the old block if newSize is 0, or,
returns nullptr if block is nullptr
*/
void* override realloc_d(void* block, size_d newSize);
/*
delta's free
frees the given block of memory
returns nothing,
if block is nullptr, this function does nothing
*/
void override free_d(void* block);
/*
initializes delta
returns DELTA_SUCCESS on success, or,
DELTA_FAIL on failure
*/
DELAPI int InitDelta(void); 

#endif

// implementation

#ifdef DELTA_IMPLEMENTATION

void* override malloc_d(size_d size) {
    if (size <= 0)
        return nullptr;
    return malloc(size);
}

void* override realloc_d(void* block, size_d newSize) {
    if (newSize <= 0) 
        return block;
    if (block == nullptr)
        return nullptr;
    return realloc(block, newSize);
}

void override free_d(void* block) {
    if (block == nullptr)
        return;
    free(block);
}

DELAPI int InitDelta(void) {
    // init stuff
    return DELTA_SUCCESS;
}

#endif