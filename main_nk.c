/*
 * main_nk.c - cosmo-sokol with Nuklear (strict-foss)
 *
 * Reusable base for cosmopolitan GUI applications:
 *   - sokol_app.h  → cross-platform window/events
 *   - sokol_gfx.h  → cross-platform graphics (GL)
 *   - nuklear.h    → immediate-mode GUI (pure C, single header)
 *
 * Build: cosmocc (no C++ required)
 * Run: Single APE binary on Linux/Windows/macOS
 */

#define SOKOL_GLCORE
#define SOKOL_NO_ENTRY
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"

/* Nuklear configuration */
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include "nuklear.h"

/* sokol-nuklear glue */
#define SOKOL_NUKLEAR_IMPL
#include "util/sokol_nuklear.h"

#include <cosmo.h>
#include "win32_tweaks.h"

/* ══════════════════════════════════════════════════════════════════════════════
 * Application State
 * ══════════════════════════════════════════════════════════════════════════════ */

static struct {
    sg_pass_action pass_action;

    /* Demo state */
    float slider_value;
    int checkbox_value;
    int option;
    char text_buffer[256];
    int text_len;

    /* Theme colors */
    struct nk_colorf bg_color;
} app;

/* ══════════════════════════════════════════════════════════════════════════════
 * Callbacks
 * ══════════════════════════════════════════════════════════════════════════════ */

static void init(void) {
    /* Initialize sokol-gfx */
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });

    /* Initialize sokol-nuklear */
    snk_setup(&(snk_desc_t){
        .dpi_scale = sapp_dpi_scale(),
        .logger.func = slog_func,
    });

    /* Initialize app state */
    app.slider_value = 0.5f;
    app.checkbox_value = 0;
    app.option = 0;
    app.text_buffer[0] = '\0';
    app.text_len = 0;
    app.bg_color = (struct nk_colorf){0.1f, 0.18f, 0.24f, 1.0f};

    app.pass_action = (sg_pass_action){
        .colors[0] = {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = {app.bg_color.r, app.bg_color.g, app.bg_color.b, 1.0f}
        }
    };
}

static void frame(void) {
    struct nk_context *ctx = snk_new_frame();

    /* ── Main Demo Window ──────────────────────────────────────────────────── */
    if (nk_begin(ctx, "cosmo-sokol + Nuklear",
                 nk_rect(50, 50, 400, 400),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                 NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {

        /* Header */
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_label(ctx, "Cosmopolitan GUI Demo", NK_TEXT_CENTERED);
        nk_label(ctx, "Single APE binary - runs everywhere!", NK_TEXT_CENTERED);

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);

        /* Platform info */
        nk_layout_row_dynamic(ctx, 25, 1);
        if (IsLinux()) {
            nk_label(ctx, "Platform: Linux (X11)", NK_TEXT_LEFT);
        } else if (IsWindows()) {
            nk_label(ctx, "Platform: Windows", NK_TEXT_LEFT);
        } else if (IsXnu()) {
            nk_label(ctx, "Platform: macOS", NK_TEXT_LEFT);
        } else {
            nk_label(ctx, "Platform: Unknown", NK_TEXT_LEFT);
        }

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);

        /* Slider */
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_label(ctx, "Slider:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_slider_float(ctx, 0.0f, &app.slider_value, 1.0f, 0.01f);

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);

        /* Checkbox */
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_checkbox_label(ctx, "Enable Feature", &app.checkbox_value);

        /* Radio buttons */
        nk_layout_row_dynamic(ctx, 25, 3);
        if (nk_option_label(ctx, "Option A", app.option == 0)) app.option = 0;
        if (nk_option_label(ctx, "Option B", app.option == 1)) app.option = 1;
        if (nk_option_label(ctx, "Option C", app.option == 2)) app.option = 2;

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);

        /* Text input */
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_label(ctx, "Text Input:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_edit_string(ctx, NK_EDIT_FIELD, app.text_buffer, &app.text_len, 256, nk_filter_default);

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);

        /* Buttons */
        nk_layout_row_dynamic(ctx, 30, 2);
        if (nk_button_label(ctx, "Button 1")) {
            /* Handle click */
        }
        if (nk_button_label(ctx, "Button 2")) {
            /* Handle click */
        }

        nk_layout_row_dynamic(ctx, 10, 1);
        nk_spacing(ctx, 1);

        /* Color picker for background */
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_label(ctx, "Background Color:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 120, 1);
        app.bg_color = nk_color_picker(ctx, app.bg_color, NK_RGBA);

        /* Update clear color */
        app.pass_action.colors[0].clear_value.r = app.bg_color.r;
        app.pass_action.colors[0].clear_value.g = app.bg_color.g;
        app.pass_action.colors[0].clear_value.b = app.bg_color.b;
    }
    nk_end(ctx);

    /* ── Render ────────────────────────────────────────────────────────────── */
    sg_begin_pass(&(sg_pass){
        .action = app.pass_action,
        .swapchain = sglue_swapchain()
    });
    snk_render(sapp_width(), sapp_height());
    sg_end_pass();
    sg_commit();
}

static void cleanup(void) {
    snk_shutdown();
    sg_shutdown();
}

static void event(const sapp_event* ev) {
    snk_handle_event(ev);
}

/* ══════════════════════════════════════════════════════════════════════════════
 * Entry Point
 * ══════════════════════════════════════════════════════════════════════════════ */

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    ShowCrashReports();

    if (IsWindows()) {
        win32_tweaks_hide_console();
    }

    sapp_run(&(sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 800,
        .height = 600,
        .window_title = "cosmo-sokol (Nuklear)",
        .icon.sokol_default = true,
        .logger.func = slog_func,
    });

    return 0;
}
