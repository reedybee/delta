# Delta
Delta is a single c-header file for (eventually) creating and managing windows across various
operating systems and hardware using a single API.

## Features:
- Create and manage windows using the `Win32 API`,
- Interface with OpenGL directly to handle graphics,

## Usage:
Delta's header file can be found at `delta/delta.h`.

Including `delta.h` in your project, delta can then be used by: 
```c
#define DELTA_IMPLEMENTATION
#include <delta.h>
```
A delta window can then be created, and, if needed, an OpenGL context can be added to it.

## Planned Features:
 Planned features may change, these are just features I would like to implement in the future.
- Input Support
- Linux and MacOS Support (Creating/Managing windows)
- Support for different graphics API's (Vulkan, Metal, Direct12, etc...)

## License
This software is distributed under the MIT License, details can be found in LICENSE.md.
