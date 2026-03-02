# cosmo-sokol: Reusable GUI Base

## What This Is

A **reusable foundation** for cosmopolitan GUI applications:

- **sokol** → cross-platform window/events/graphics
- **Nuklear** → immediate-mode UI (pure C, single header)
- **cosmocc** → single APE binary runs on Linux/Windows/macOS

## For Projects Using This

### 1. Add as Dependency

```bash
# As submodule
git submodule add https://github.com/ludoplex/cosmo-sokol deps/cosmo-sokol

# Or copy deps/sokol, deps/nuklear, and shims/
```

### 2. Include Headers

```c
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"

#include "util/sokol_nuklear.h"
```

### 3. Implement Callbacks

```c
static void my_init(void) {
    sg_setup(&(sg_desc){ .environment = sglue_environment() });
    snk_setup(&(snk_desc_t){ .dpi_scale = sapp_dpi_scale() });
    // Your init code
}

static void my_frame(void) {
    struct nk_context *ctx = snk_new_frame();

    // Your UI code using Nuklear
    if (nk_begin(ctx, "My Window", nk_rect(10, 10, 200, 200), NK_WINDOW_TITLE)) {
        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_button_label(ctx, "Click Me")) {
            // Handle click
        }
    }
    nk_end(ctx);

    // Render
    sg_begin_pass(...);
    snk_render(sapp_width(), sapp_height());
    sg_end_pass();
    sg_commit();
}

static void my_cleanup(void) {
    snk_shutdown();
    sg_shutdown();
}

static void my_event(const sapp_event* ev) {
    snk_handle_event(ev);
    // Your event handling
}
```

### 4. Entry Point

```c
int main(int argc, char* argv[]) {
    sapp_run(&(sapp_desc){
        .init_cb = my_init,
        .frame_cb = my_frame,
        .cleanup_cb = my_cleanup,
        .event_cb = my_event,
        .width = 800,
        .height = 600,
        .window_title = "My App",
    });
    return 0;
}
```

### 5. Build

```bash
# Pure C - no cosmoc++ needed!
cosmocc -I deps/cosmo-sokol/deps/sokol \
        -I deps/cosmo-sokol/deps/nuklear \
        -I deps/cosmo-sokol/shims/linux \
        -mcosmo -o myapp.com \
        myapp.c \
        deps/cosmo-sokol/shims/sokol/*.c \
        deps/cosmo-sokol/shims/linux/*.c
```

## Example: tedit-cosmo

tedit-cosmo (cosmo-tedit) uses this base:

```c
// tedit uses cosmo-sokol + Nuklear for:
// - Main window (sokol_app)
// - Text editor widget (Nuklear nk_edit_string)
// - Menu bar (Nuklear)
// - Status bar (Nuklear)
// - Dialogs (Nuklear)

static void tedit_frame(void) {
    struct nk_context *ctx = snk_new_frame();

    // Menu bar
    if (nk_begin(ctx, "##menubar", ...)) {
        nk_menubar_begin(ctx);
        if (nk_menu_begin_label(ctx, "File", ...)) {
            if (nk_menu_item_label(ctx, "New", ...)) { /* ... */ }
            if (nk_menu_item_label(ctx, "Open", ...)) { /* ... */ }
            if (nk_menu_item_label(ctx, "Save", ...)) { /* ... */ }
            nk_menu_end(ctx);
        }
        nk_menubar_end(ctx);
    }
    nk_end(ctx);

    // Editor area
    if (nk_begin(ctx, "##editor", ...)) {
        nk_edit_string(ctx, NK_EDIT_MULTILINE | NK_EDIT_CLIPBOARD,
                       editor_buffer, &editor_len, MAX_BUFFER, nk_filter_default);
    }
    nk_end(ctx);

    // Render
    ...
}
```

## Specs (cosmo-bde)

The `specs/` directory contains cosmo-bde spec files:

| Spec | Purpose |
|------|---------|
| `reusable.schema` | App configuration, theme, common UI types |
| `integration.sig` | Public API function signatures |

These define the **reusable interface** that projects depend on.

## Stack

```
┌─────────────────────────────────────────┐
│          Your Application               │
├─────────────────────────────────────────┤
│  cosmo-sokol (this repo)                │
│  ├── Nuklear (UI widgets)               │
│  ├── sokol_app (window/events)          │
│  └── sokol_gfx (graphics)               │
├─────────────────────────────────────────┤
│  Platform Shims                         │
│  ├── Linux: X11 + GL via dlopen         │
│  ├── Windows: Win32 + WGL               │
│  └── macOS: Cocoa via objc dlopen       │
├─────────────────────────────────────────┤
│  jart/cosmopolitan                      │
│  └── cosmo_dlopen, IsLinux/Windows/Xnu  │
└─────────────────────────────────────────┘
```

## Why This Approach

1. **Reusable** - Multiple projects share the same base
2. **Pure C** - No C++ required (Nuklear vs Dear ImGui)
3. **Single binary** - APE runs on Linux/Windows/macOS
4. **Same pattern everywhere** - dlopen for all platforms
5. **Maintainable** - Specs define interfaces, code follows
