# cosmo-sokol Architecture

## Core Principle

**Single binary, all platforms, runtime dispatch.**

jart/cosmopolitan provides:
- `cosmo_dlopen()` - works on Linux, Windows, macOS
- `IsLinux()`, `IsWindows()`, `IsXnu()` - runtime platform detection
- No ifdefs needed - all code compiles together

## Platform Loading Pattern

All platforms use the **same pattern**:

```c
void* lib = cosmo_dlopen(path, RTLD_NOW);
void* func = cosmo_dlsym(lib, "function_name");
```

| Platform | Library | Example |
|----------|---------|---------|
| Linux | libX11.so | `cosmo_dlopen("libX11.so.6", ...)` |
| Linux | libGL.so | `cosmo_dlopen("libGL.so.1", ...)` |
| Windows | kernel32.dll | Already available via cosmopolitan |
| Windows | opengl32.dll | `cosmo_dlopen("opengl32.dll", ...)` |
| macOS | libobjc.dylib | `cosmo_dlopen("/usr/lib/libobjc.dylib", ...)` |
| macOS | OpenGL.framework | `cosmo_dlopen(".../OpenGL.framework/OpenGL", ...)` |

**macOS is NOT harder.** It's the same dlopen pattern. The only difference:
- Linux calls `XCreateWindow()`, `glXCreateContext()`
- Windows calls `CreateWindowEx()`, `wglCreateContext()`
- macOS calls `objc_msgSend(NSWindow, "alloc")`, etc.

## Runtime Dispatch

```c
void create_window(int w, int h) {
    if (IsLinux()) {
        // X11 via dlopen'd libX11
        XCreateWindow(display, ...);
    }
    if (IsWindows()) {
        // Win32 API (direct, no dlopen needed)
        CreateWindowEx(...);
    }
    if (IsXnu()) {
        // Cocoa via dlopen'd libobjc
        objc_msgSend(objc_getClass("NSWindow"), sel("alloc"));
    }
}
```

## sokol Integration

sokol_app.h provides the windowing abstraction. cosmo-sokol:

1. Compiles sokol with platform prefixes:
   - `linux_sapp_run()`, `windows_sapp_run()`, `macos_sapp_run()`

2. Creates dispatch shim (sokol_cosmo.c):
   ```c
   void sapp_run(const sapp_desc* desc) {
       if (IsLinux()) linux_sapp_run(desc);
       if (IsWindows()) windows_sapp_run(desc);
       if (IsXnu()) macos_sapp_run(desc);
   }
   ```

3. Each platform backend uses cosmo_dlopen for platform libs

## Nuklear Integration

Replace Dear ImGui (C++) with Nuklear (C):

```
sokol_app   → windowing (platform dispatch)
sokol_gfx   → graphics (GL everywhere)
sokol_nuklear → UI rendering glue
nuklear.h   → immediate mode UI
```

Benefits:
- Single header (nuklear.h)
- Pure C (no cosmoc++ needed)
- MIT/Unlicense (strict-foss compliant)

## Build Flow

```
                    ┌─────────────────────┐
                    │  nuklear.h          │
                    │  (single header)    │
                    └──────────┬──────────┘
                               │
┌──────────────┐    ┌──────────┴──────────┐    ┌──────────────┐
│ sokol_linux.c │    │  sokol_nuklear.h    │    │sokol_windows.c│
│ (X11 dlopen) │    │  (rendering glue)   │    │ (Win32 API)  │
└──────┬───────┘    └──────────┬──────────┘    └──────┬───────┘
       │                       │                       │
       │            ┌──────────┴──────────┐           │
       │            │   sokol_cosmo.c     │           │
       │            │ (runtime dispatch)  │           │
       │            └──────────┬──────────┘           │
       │                       │                       │
       └───────────────────────┼───────────────────────┘
                               │
                    ┌──────────┴──────────┐
                    │     main.c          │
                    │  (app + Nuklear UI) │
                    └──────────┬──────────┘
                               │
                    ┌──────────┴──────────┐
                    │    cosmocc          │
                    │ (single APE binary) │
                    └─────────────────────┘
```

## Files

| File | Purpose |
|------|---------|
| `specs/platform/backends.def` | Platform constants |
| `specs/platform/dispatch.sig` | Function signatures |
| `specs/platform/dlopen.schema` | Library loading state |
| `specs/app/state.schema` | Runtime app state |
| `specs/app/config.schema` | App configuration |
| `specs/ui/nuklear.def` | Nuklear constants |

## Key Insight

The "macOS backend problem" doesn't exist as a separate concern.

It's the same problem already solved for Linux:
1. dlopen the platform library
2. dlsym the functions
3. Call them

Linux: `dlopen("libX11.so")` → `dlsym("XCreateWindow")` → call
macOS: `dlopen("libobjc.dylib")` → `dlsym("objc_msgSend")` → call

Same code path. Same complexity. Same solution.
