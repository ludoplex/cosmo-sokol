# macOS Shims for Cosmopolitan Sokol

## Current Status

The macOS backend is currently a **stub implementation** that compiles but does not 
provide actual functionality. When run on macOS, it will print an error message.

## Challenge

Sokol's macOS backend uses Objective-C extensively:
- `NSApplication`, `NSWindow`, `NSOpenGLView` for window management
- Metal or OpenGL for graphics
- Cocoa event handling

Cosmopolitan's `cosmocc` cannot directly compile Objective-C.

## Implementation Path

To implement full macOS support, we need to:

### Option 1: Objective-C Runtime via C (Recommended)

Call Objective-C methods from C using the runtime:

```c
#include <objc/objc.h>
#include <objc/message.h>

// Load Objective-C runtime
void* libobjc = cosmo_dlopen("/usr/lib/libobjc.dylib", RTLD_NOW);

// Get objc_msgSend
typedef id (*objc_msgSend_t)(id, SEL, ...);
objc_msgSend_t msgSend = cosmo_dlsym(libobjc, "objc_msgSend");

// Example: [NSApplication sharedApplication]
id app = msgSend(objc_getClass("NSApplication"), sel_registerName("sharedApplication"));
```

Files needed in this directory:
- `objc_runtime.c` - Wrapper for objc_msgSend calls
- `cocoa.c` - NSApplication, NSWindow, NSOpenGLView shims
- `metal.c` (optional) - Metal shims if using SOKOL_METAL

### Option 2: Native Helper Library

Compile a small Objective-C helper on macOS that exposes C functions,
then dlopen it at runtime. Less portable but simpler.

### Required Frameworks

On macOS, these system frameworks need to be accessible:
- Cocoa.framework
- OpenGL.framework (for GL backend) or Metal.framework (for Metal backend)
- QuartzCore.framework
- IOKit.framework

## Files to Create

```
shims/macos/
├── README.md          (this file)
├── objc_runtime.c     (Objective-C runtime wrappers)
├── objc_runtime.h     
├── cocoa.c            (Cocoa shims: NSApplication, NSWindow, etc.)
├── opengl.c           (OpenGL context creation)
└── gen-cocoa          (Generator script like gen-x11)
```

## See Also

- `shims/sokol/sokol_macos.c` - Current stub implementation with detailed comments
- `shims/linux/gen-x11` - Example of how Linux X11 shims are generated
- Cosmopolitan's objc support in `libc/nt/thunk/`
