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
#include <glad/glad.h>

// used to distinquish what is apart of the Delta api
#define DELAPI 
// used to distinquish what is win32 based
#define DELAPI_WIN32
// used to distinquish what is win32 based
#define DELAPI_UNIX

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

#define DELTA_KEY_0 0x30
#define DELTA_KEY_1 0x31
#define DELTA_KEY_2 0x32
#define DELTA_KEY_3 0x33
#define DELTA_KEY_4 0x34
#define DELTA_KEY_5 0x35
#define DELTA_KEY_6 0x36
#define DELTA_KEY_7 0x37
#define DELTA_KEY_8 0x38
#define DELTA_KEY_9 0x39

#define DELTA_KEY_A 0x41
#define DELTA_KEY_B 0x42
#define DELTA_KEY_C 0x43
#define DELTA_KEY_D 0x44
#define DELTA_KEY_E 0x45
#define DELTA_KEY_F 0x46
#define DELTA_KEY_G 0x47
#define DELTA_KEY_H 0x48
#define DELTA_KEY_I 0x49
#define DELTA_KEY_J 0x4A
#define DELTA_KEY_K 0x4B
#define DELTA_KEY_L 0x4C
#define DELTA_KEY_M 0x4D
#define DELTA_KEY_N 0x4E
#define DELTA_KEY_O 0x4F
#define DELTA_KEY_P 0x50
#define DELTA_KEY_Q 0x51
#define DELTA_KEY_R 0x52
#define DELTA_KEY_S 0x53
#define DELTA_KEY_T 0x54
#define DELTA_KEY_U 0x55
#define DELTA_KEY_V 0x56
#define DELTA_KEY_W 0x57
#define DELTA_KEY_X 0x58
#define DELTA_KEY_Y 0x59
#define DELTA_KEY_Z 0x5A

#define DELTA_STATE_KEY_RELEASE 0
#define DELTA_STATE_KEY_PRESS   1

#define WINDOWS 0x00
#define UNIX 0x01

#if defined(_WIN32) || defined(WIN32)

#include <windows.h>
#include <dwmapi.h>

// Define necessary WGL_ARB constants manually
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB  0x9126

// Profile bits
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

typedef struct DELTA_WINDOW {
    uint32 created;
    uint32 destroyed;
    HWND hwnd;
} deltaWindow;

typedef struct DELTA_DATA {
    deltaWindow* deltaWindow;
    HGLRC context;
    uint16 openGLVersion;
} DeltaData;

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);

DELAPI_WIN32 LRESULT CALLBACK GetWindowProcWin32(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

DELAPI_WIN32 HWND CreateWindowWin32(const char* title, uint32 width, uint32 height);

DELAPI_WIN32 void CreateOpenGLContextWin32();

DELAPI_WIN32 deltaWindow* CreateDeltaWindowWin32(const char* title, uint32 w, uint32 heigth, uint32 flags);

DELAPI_WIN32 void UpdateWindowWin32(deltaWindow* windows);

DELAPI_WIN32 deltaProcAddress GetProcAddressWin32(const char* name);

DELAPI_WIN32 void GetWindowSizeWin32(deltaWindow* window, uint32* w, uint32* h);

DELAPI_WIN32 POINT GetMousePosWin32(deltaWindow* window, uint32* x, uint32* y);

#define DELTA_WIN32 1
#define DELTA_UNIX  0

#endif

#ifdef __unix__


typedef struct DELTA_WINDOW {

} deltaWindow;

typedef struct DELTA_DATA {

} DeltaData;

#define DELTA_UNIX 1

#endif


extern DeltaData deltaData;


// delta declarations


/*
creates and sets the OpenGL's context
to the give major and minor versions.
*/
DELAPI uint32 deltaSetOpenGLContext(uint8 versionMajor, uint8 versionMinor);
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

DELAPI void deltaGetWindowSize(deltaWindow* window, uint32* w, uint32* h);

DELAPI void deltaGetMousePosition(deltaWindow* window, uint32* x, uint32* y); 

DELAPI uint32 deltaGetKey(uint32 key);

#endif

/*
------------------------------
    IMPLEMENTATIONS FOLLOW     
------------------------------
*/

#ifdef DELTA_IMPLEMENTATION

DeltaData deltaData;


// delta implementations


DELAPI uint32 deltaSetOpenGLContext(uint8 versionMajor, uint8 versionMinor) {
    
    deltaData.openGLVersion = versionMajor;
    deltaData.openGLVersion <<= 8;
    deltaData.openGLVersion += versionMinor;

    if (DELTA_WIN32) {
        CreateOpenGLContextWin32();
    }

    return 1;
}

DELAPI deltaWindow* deltaCreateWindow(const char* title, uint32 w, uint32 h, uint32 flags) {
    
    deltaWindow* window = (deltaWindow*)malloc(sizeof(deltaWindow));
    memset(window, 0, sizeof(deltaWindow));

    if (DELTA_WIN32) {
        window = CreateDeltaWindowWin32(title, w, h, flags);
    }
    if (DELTA_UNIX) {
        
    }

    return (deltaWindow*)window;
}

DELAPI void deltaUpdateWindow(deltaWindow* window) {
    
    if (window == nullptr) {
        printf("NO WINDOW\n");
        return;
    }

    if(DELTA_WIN32) {
        UpdateWindowWin32(window);
    }

    Sleep(5);

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
    deltaProcAddress addr;
    
    if (DELTA_WIN32) {
        addr = GetProcAddressWin32(name);
    }

    return addr;
}

DELAPI void deltaGetWindowSize(deltaWindow* window, uint32* w, uint32* h) {
    if (DELTA_WIN32) {
        GetWindowSizeWin32(window, w, h);
    }

    return;
}   

DELAPI void deltaGetMousePosition(deltaWindow* window, uint32* x, uint32* y) {
    
    int32 xPos = 0, yPos = 0;
    uint32 w = 0, h = 0;

    if (DELTA_WIN32) {
        POINT pos = GetMousePosWin32(window, x, y);
        xPos = pos.x; 
        yPos = pos.y;
        deltaGetWindowSize(window, &w, &h);
    }

    if (xPos < 0) {
        xPos = 0;
        return;
    }
        
    if (xPos > w) {
        xPos = w;
        return;
    }

    if (yPos < 0) {
        yPos = 0;
        return;
    }
    if (yPos > h) {
        yPos = h;
        return;
    }

    *x = (uint32)xPos;
    *y = (uint32)yPos;

    return;
}

DELAPI uint32 deltaGetKey(uint32 key) {
    return GetKeyState(key) & 0x8000;
}


// win32 implementations

    #ifdef DELTA_WIN32

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

DELAPI_WIN32 HWND CreateWindowWin32(const char* title, uint32 width, uint32 height) {

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

    RECT windowSize = { 0 };
    windowSize.left = 0;
    windowSize.top  = 0;
    windowSize.right = width;
    windowSize.bottom  = height;

    AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, 0);

    uint32 newWidth  = windowSize.right  - windowSize.left;
    uint32 newHeight = windowSize.bottom - windowSize.top;

    HWND hwnd = CreateWindowEx(
        0,
        classname,
        title,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        CW_USEDEFAULT, CW_USEDEFAULT, newWidth, newHeight,
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

DELAPI_WIN32 void CreateOpenGLContextWin32() {

    uint8 versionMajor = deltaData.openGLVersion >> 8;
    uint8 versionMinor = deltaData.openGLVersion << 8;

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
        return;
    }

    uint32 pixelFormatNumber = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormatNumber, &pfd);

    HGLRC temp = wglCreateContext(hdc);
    wglMakeCurrent(hdc, temp);

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (!wglCreateContextAttribsARB) {
        printf("Failed to create context attribs arb\n");
        return;
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
}

DELAPI_WIN32 deltaWindow* CreateDeltaWindowWin32(const char* title, uint32 w, uint32 h, uint32 flags) {
    deltaWindow* window = (deltaWindow*)malloc(sizeof(deltaWindow));
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
    return window;
}

DELAPI_WIN32 void UpdateWindowWin32(deltaWindow* window) {
    MSG msg = { 0 };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT)
            window->destroyed = 1;
    }

    SwapBuffers(GetDC(window->hwnd));
}

DELAPI_WIN32 deltaProcAddress GetProcAddressWin32(const char* name) {
    deltaProcAddress addr = (deltaProcAddress)wglGetProcAddress(name);

    if (!addr) {
        HMODULE module = GetModuleHandleA("opengl32.dll");
        addr = (deltaProcAddress)GetProcAddress(module, name);
    }
    return addr;
}

DELAPI_WIN32 void GetWindowSizeWin32(deltaWindow* window, uint32* w, uint32* h) {
    RECT rect = { 0 };

    GetClientRect(window->hwnd, &rect);

    *w = rect.right  - rect.left;
    *h = rect.bottom - rect.top;
}

DELAPI_WIN32 POINT GetMousePosWin32(deltaWindow* window, uint32* x, uint32* y) {
    POINT pos = { 0 };

    GetCursorPos(&pos);

    MapWindowPoints(NULL, window->hwnd, &pos, 1);

    return pos;
}

    #endif

#endif