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
#include <windows.h>

// used to distinquish what is apart of the Delta api
#define DELAPI 
// used to distinquish what is win32 based
#define DELAPI_WIN32
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
#define int8_d   int8_t
// signed short
#define int16_d  int16_t
// signed int
#define int32_d  int32_t
// signed long long int
#define int64_d  int64_t

#define DELTA_FAIL 0
#define DELTA_SUCCESS 1

#define DELTA_WINDOW_SHOWN  1
#define DELTA_WINDOW_HIDDEN 0

#define DELTA_VERSION_MAJOR 0
#define DELTA_VERSION_MINOR 1

typedef struct DELTA_WINDOW {
    uint32_d created;
    uint32_d destroyed;
    HWND hwnd;
} deltaWindow;

typedef struct DELTA_DATA {
    // will be changing
    deltaWindow* deltaWindows;
} DeltaData;

extern DeltaData deltaData;

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
delta's memset
sets the first num bytes in the given block
to the given value
returns the new block, or
nullptr if block is a nullptr
*/
void* override memset_d(void* block, int value, size_d num);
/*
creates a delta window
returns a pointer to the newly created window, or,
nullptr on failure
*/
DELAPI deltaWindow* deltaCreateWindow(const char* title, uint32_d w, uint32_d h, uint32_d flags);
/*
updates the delta window,
retrieves any updates sent to delta
from the operating system
returns nothing.
*/
DELAPI void deltaUpdateWindow(deltaWindow* window);
/*
retrieves the close flag from
the delta window given
returns the close flag
*/
DELAPI uint32_d deltaWindowShouldClose(deltaWindow* window);
/*
free the given delta window,
releasing the memory used
*/
DELAPI void deltaDestroyWindow(deltaWindow* window);

// win32 api

DELAPI_WIN32 LRESULT CALLBACK GetWindowProcWin32(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

DELAPI_WIN32 HWND CreateWindowWin32(const char* title, uint32_d width, uint16_d height);

#endif

#ifdef DELTA_IMPLEMENTATION

DeltaData deltaData;

// overrides

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
// dont fucking use this
void* override memset_d(void* block, int value, size_d num) {
    memset(block, value, num);
    return block;
}

// delta implementations

DELAPI deltaWindow* deltaCreateWindow(const char* title, uint32_d w, uint32_d h, uint32_d flags) {
    deltaWindow* window = (deltaWindow*)malloc_d(sizeof(deltaWindow));
    memset(window, 0, sizeof(deltaWindow));

    window->created = 1;
    window->destroyed = 0;

    window->hwnd = CreateWindowWin32(title, w, h);

    ShowWindow(window->hwnd, flags);

    return (deltaWindow*)window;
}

DELAPI void deltaUpdateWindow(deltaWindow* window) {
    if (window == nullptr) {
        printf("NO WINDOW\n");
        return;
    }

    MSG msg = { 0 };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT)
            window->destroyed = 1;
    }
    return;
}

DELAPI uint32_d deltaWindowShouldClose(deltaWindow* window) {
    return window->destroyed;
}

DELAPI void deltaDestroyWindow(deltaWindow* window) {
    DestroyWindow(window->hwnd);
    free(window);
}

// win32 implementations

DELAPI_WIN32 LRESULT CALLBACK GetWindowProcWin32(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 1;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

DELAPI_WIN32 HWND CreateWindowWin32(const char* title, uint32_d width, uint16_d height) {

    HINSTANCE instance = GetModuleHandle(0);
    if (instance == NULL) 
        printf("Failed to get instance\n");

    const char classname[] = "Delta Window";

    WNDCLASS class = { 0 };

    class.lpfnWndProc = GetWindowProcWin32;
    class.hInstance = instance;
    class.lpszClassName = classname;

    if (!RegisterClass(&class))
        printf("Failed to register class\n");

    HWND hwnd = CreateWindowEx(
        0,
        classname,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL,
        NULL,
        instance,
        NULL
    );
    if (hwnd == NULL) {
        printf("Failed to create win32 window\n");
    }
    return hwnd;
}

#endif