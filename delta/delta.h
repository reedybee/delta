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
#include <glad/glad.h>

// used to distinquish what is apart of the Delta api
#define DELAPI 
// used to distinquish what is win32 based
#define DELAPI_WIN32
// used to distinguish what standard functions are being overriden
#define override

#define nullptr (void*)0
// unsigned long long int
typedef size_t      size;
// unsigned char
typedef uint8_t     uint8;
// unsigned short
typedef uint16_t    uint16;
// unsigned int
typedef uint32_t    uint32;
// unsigned long long int
typedef uint64_t    uint64;
// signed char
typedef int8_t      int8;
// signed short
typedef int16_t     int16;
// signed int
typedef int32_t     int32;
// signed long long int
typedef int64_t     int64;

typedef void* (*deltaProcAddress)(const char*); 

#define DELTA_FAIL 0
#define DELTA_SUCCESS 1

#define DELTA_WINDOW_HIDDEN 0b00000000
#define DELTA_WINDOW_SHOWN  0b00000001
#define DELTA_WINDOW_OPENGL 0b00000010

#define DELTA_VERSION_MAJOR 0
#define DELTA_VERSION_MINOR 1

typedef struct DELTA_WINDOW {
    uint32 created;
    uint32 destroyed;
    HWND hwnd;
} deltaWindow;

typedef struct DELTA_DATA {
    deltaWindow* deltaWindow;
    HGLRC context;
} DeltaData;

extern DeltaData deltaData;


// overrides


/* 
delta's malloc,
allocates a size of heap memory
returns a pointer to the newly allocated
memory, or nullptr if size is 0 
*/
void* override malloc_d(size size);
/*
delta's realloc
reallocates the given block of memory
returns a pointer to the new block,
returns the old block if newSize is 0, or,
returns nullptr if block is nullptr
*/
void* override realloc_d(void* block, size newSize);
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
void* override memset_d(void* block, int value, size num);


// delta declarations


/*
creates and sets the OpenGL's context
to the give major and minor versions.
*/
DELAPI uint32 deltaSetOpenGLContext(uint32 versionMajor, uint32 versionMinor);
/*
creates a delta window
returns a pointer to the newly created window, or,
nullptr on failure
*/
DELAPI deltaWindow* deltaCreateWindow(const char* title, uint32 w, uint32 h, uint32 flags);
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
DELAPI uint32 deltaWindowShouldClose(deltaWindow* window);
/*
free the given delta window,
releasing the memory used
*/
DELAPI void deltaDestroyWindow(deltaWindow* window);
/*
returns the address to the current openGL context
*/
DELAPI deltaProcAddress deltaGetProcAddress(const char* name);

DELAPI void deltaGetWindowSize(deltaWindow* window, int* w, int* h);


// win32 declarations


// Define necessary WGL_ARB constants manually
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB  0x9126

// Profile bits
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);

DELAPI_WIN32 LRESULT CALLBACK GetWindowProcWin32(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

DELAPI_WIN32 HWND CreateWindowWin32(const char* title, uint32 width, uint16 height);

#endif

/*
------------------------------
    IMPLEMENTATIONS FOLLOW     
------------------------------
*/

#ifdef DELTA_IMPLEMENTATION

DeltaData deltaData;

// overrides

void* override malloc_d(size size) {
    if (size <= 0)
        return nullptr;
    return malloc(size);
}

void* override realloc_d(void* block, size newSize) {
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
void* override memset_d(void* block, int value, size num) {
    memset(block, value, num);
    return block;
}


// delta implementations


DELAPI uint32 deltaSetOpenGLContext(uint32 versionMajor, uint32 versionMinor) {
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    HDC hdc = GetDC(deltaData.deltaWindow->hwnd);
    if (hdc == NULL) {
        printf("Failed to retrieve HDC\n");
        return 0;
    }

    uint32 pixelFormatNumber = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormatNumber, &pfd);

    HGLRC temp = wglCreateContext(hdc);
    wglMakeCurrent(hdc, temp);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (!wglCreateContextAttribsARB) {
        printf("Failed to create context attribs arb\n");
        return 0;
    }

    uint32 attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, versionMajor,
        WGL_CONTEXT_MINOR_VERSION_ARB, versionMinor,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 
        0
    };

    HGLRC hrc = wglCreateContextAttribsARB(hdc, 0, attribs);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(temp);
    wglMakeCurrent(hdc, hrc);
    deltaData.context = hrc;

    return 1;
}

DELAPI deltaWindow* deltaCreateWindow(const char* title, uint32 w, uint32 h, uint32 flags) {
    deltaWindow* window = (deltaWindow*)malloc_d(sizeof(deltaWindow));
    memset(window, 0, sizeof(deltaWindow));

    window->created = 1;
    window->destroyed = 0;

    window->hwnd = CreateWindowWin32(title, w, h);

    uint8 shown = flags & DELTA_WINDOW_SHOWN;
    if (shown)
        ShowWindow(window->hwnd, 1);
    else 
        ShowWindow(window->hwnd, SW_MINIMIZE);

    deltaData.deltaWindow = window;
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

    SwapBuffers(GetDC(window->hwnd));

    return;
}

DELAPI uint32 deltaWindowShouldClose(deltaWindow* window) {
    return window->destroyed;
}

DELAPI void deltaDestroyWindow(deltaWindow* window) {
    DestroyWindow(window->hwnd);
    free(window);
}

DELAPI deltaProcAddress deltaGetProcAddress(const char* name) {
    deltaProcAddress addr = (deltaProcAddress)wglGetProcAddress(name);

    if (!addr) {
        HMODULE module = GetModuleHandleA("opengl32.dll");
        addr = (deltaProcAddress)GetProcAddress(module, name);
    }
    return addr;
}

DELAPI void deltaGetWindowSize(deltaWindow* window, int* w, int* h) {
    RECT rect;

    GetWindowRect(window->hwnd, &rect);

    *w = rect.right  - rect.left;
    *h = rect.bottom - rect.top;

    return;
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

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));

            EndPaint(hwnd, &ps);
        }
        return 0;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

DELAPI_WIN32 HWND CreateWindowWin32(const char* title, uint32 width, uint16 height) {

    HINSTANCE instance = GetModuleHandle(0);
    if (instance == NULL) 
        printf("Failed to get instance\n");

    const char classname[] = "Delta Window";

    WNDCLASS class = { 0 };

    class.lpfnWndProc = GetWindowProcWin32;
    class.hInstance = instance;
    class.hCursor = LoadCursor(NULL, IDC_ARROW);
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