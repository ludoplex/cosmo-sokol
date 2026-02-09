/*
 * sokol_macos.c - macOS backend for Cosmopolitan sokol
 * 
 * CURRENT STATUS: STUB IMPLEMENTATION
 * 
 * This file provides macos_ prefixed sokol functions for the Cosmopolitan
 * runtime dispatch pattern. Currently it provides stub implementations that
 * compile but print an error at runtime.
 * 
 * CHALLENGE:
 * Sokol's macOS backend (in sokol_app.h and sokol_gfx.h) uses Objective-C
 * extensively - NSApplication, NSWindow, NSOpenGLView, Metal, etc. Cosmopolitan's
 * cosmocc cannot directly compile Objective-C code.
 * 
 * FUTURE IMPLEMENTATION OPTIONS:
 * 
 * 1. OBJC RUNTIME VIA C:
 *    Use Objective-C runtime functions (objc_msgSend, objc_getClass, sel_registerName)
 *    to call Cocoa/Metal from pure C. This is verbose but fully compatible with cosmocc.
 *    The Objective-C runtime can be dlopen'd from the system:
 *      void* libobjc = cosmo_dlopen("/usr/lib/libobjc.dylib", RTLD_NOW);
 *    
 *    Example:
 *      id pool = objc_msgSend(objc_getClass("NSAutoreleasePool"), sel_registerName("new"));
 *      id app = objc_msgSend(objc_getClass("NSApplication"), sel_registerName("sharedApplication"));
 * 
 * 2. MINIMAL NATIVE HELPER:
 *    Compile a small native Objective-C helper library on macOS that exposes C functions
 *    for the essential operations (create window, create GL context, pump events).
 *    This would be dlopen'd at runtime.
 * 
 * 3. GLFW/SDL ALTERNATIVE:
 *    Use a C-based windowing library (GLFW has C bindings) instead of sokol_app
 *    for macOS. GLFW can be shimmed similarly to how X11 is shimmed on Linux.
 * 
 * For now, this stub allows the project to compile and demonstrates the pattern.
 * When run on macOS, sapp_run() will print an error message explaining the situation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* We need the sokol type definitions but NOT the implementations */
#define SOKOL_APP_INCLUDED  /* Prevent sokol_app.h from defining implementations */
#define SOKOL_GFX_INCLUDED  /* Prevent sokol_gfx.h from defining implementations */

/* Include the prefix header to rename all functions to macos_* */
#include "sokol_macos.h"

/* Now include sokol headers just for type definitions */
/* We need to temporarily undefine to get the types, then redefine */
#undef SOKOL_APP_INCLUDED
#undef SOKOL_GFX_INCLUDED

#include <sokol_app.h>
#include <sokol_gfx.h>

/* State for stub implementation */
static bool _macos_initialized = false;
static sapp_desc _macos_desc = {0};
static int _macos_width = 800;
static int _macos_height = 600;

/* Helper to print the not-implemented message */
static void _macos_not_implemented(const char* func) {
    /* Only print once to avoid spam */
    static bool printed = false;
    if (!printed) {
        fprintf(stderr, 
            "\n"
            "=============================================================\n"
            "COSMO-SOKOL: macOS support is not yet fully implemented.\n"
            "\n"
            "sokol on macOS requires Objective-C which cosmocc cannot\n"
            "directly compile. Full implementation requires using the\n"
            "Objective-C runtime from C (objc_msgSend pattern).\n"
            "\n"
            "See sokol_macos.c header comments for implementation options.\n"
            "=============================================================\n"
            "\n");
        printed = true;
    }
}

/*
 * sokol_app stub implementations
 */

bool macos_sapp_isvalid(void) {
    return _macos_initialized;
}

int macos_sapp_width(void) {
    return _macos_width;
}

float macos_sapp_widthf(void) {
    return (float)_macos_width;
}

int macos_sapp_height(void) {
    return _macos_height;
}

float macos_sapp_heightf(void) {
    return (float)_macos_height;
}

int macos_sapp_color_format(void) {
    return 1; /* SG_PIXELFORMAT_RGBA8 */
}

int macos_sapp_depth_format(void) {
    return 42; /* SG_PIXELFORMAT_DEPTH_STENCIL */
}

int macos_sapp_sample_count(void) {
    return 1;
}

bool macos_sapp_high_dpi(void) {
    return false;
}

float macos_sapp_dpi_scale(void) {
    return 1.0f;
}

void macos_sapp_show_keyboard(bool show) {
    (void)show;
}

bool macos_sapp_keyboard_shown(void) {
    return false;
}

bool macos_sapp_is_fullscreen(void) {
    return false;
}

void macos_sapp_toggle_fullscreen(void) {
}

void macos_sapp_show_mouse(bool show) {
    (void)show;
}

bool macos_sapp_mouse_shown(void) {
    return true;
}

void macos_sapp_lock_mouse(bool lock) {
    (void)lock;
}

bool macos_sapp_mouse_locked(void) {
    return false;
}

void macos_sapp_set_mouse_cursor(sapp_mouse_cursor cursor) {
    (void)cursor;
}

sapp_mouse_cursor macos_sapp_get_mouse_cursor(void) {
    return SAPP_MOUSECURSOR_DEFAULT;
}

void* macos_sapp_userdata(void) {
    return _macos_desc.user_data;
}

sapp_desc macos_sapp_query_desc(void) {
    return _macos_desc;
}

void macos_sapp_request_quit(void) {
}

void macos_sapp_cancel_quit(void) {
}

void macos_sapp_quit(void) {
    if (_macos_desc.cleanup_cb) {
        _macos_desc.cleanup_cb();
    }
    _macos_initialized = false;
}

void macos_sapp_consume_event(void) {
}

uint64_t macos_sapp_frame_count(void) {
    return 0;
}

double macos_sapp_frame_duration(void) {
    return 1.0 / 60.0;
}

void macos_sapp_set_clipboard_string(const char* str) {
    (void)str;
}

const char* macos_sapp_get_clipboard_string(void) {
    return "";
}

void macos_sapp_set_window_title(const char* str) {
    (void)str;
}

void macos_sapp_set_icon(const sapp_icon_desc* icon_desc) {
    (void)icon_desc;
}

int macos_sapp_get_num_dropped_files(void) {
    return 0;
}

const char* macos_sapp_get_dropped_file_path(int index) {
    (void)index;
    return "";
}

void macos_sapp_run(const sapp_desc* desc) {
    _macos_not_implemented("sapp_run");
    
    if (desc) {
        _macos_desc = *desc;
        _macos_width = desc->width > 0 ? desc->width : 800;
        _macos_height = desc->height > 0 ? desc->height : 600;
    }
    
    /* Call init callback if provided */
    if (_macos_desc.init_cb) {
        _macos_initialized = true;
        _macos_desc.init_cb();
    }
    
    /* For stub: just cleanup immediately */
    if (_macos_desc.cleanup_cb) {
        _macos_desc.cleanup_cb();
    }
    
    _macos_initialized = false;
    
    fprintf(stderr, "macos_sapp_run: Exiting (stub implementation)\n");
    exit(1);
}

const void* macos_sapp_egl_get_display(void) {
    return NULL;
}

const void* macos_sapp_egl_get_context(void) {
    return NULL;
}

void macos_sapp_html5_ask_leave_site(bool ask) {
    (void)ask;
}

uint32_t macos_sapp_html5_get_dropped_file_size(int index) {
    (void)index;
    return 0;
}

void macos_sapp_html5_fetch_dropped_file(const sapp_html5_fetch_request* request) {
    (void)request;
}

const void* macos_sapp_metal_get_device(void) {
    return NULL;
}

const void* macos_sapp_metal_get_current_drawable(void) {
    return NULL;
}

const void* macos_sapp_metal_get_depth_stencil_texture(void) {
    return NULL;
}

const void* macos_sapp_metal_get_msaa_color_texture(void) {
    return NULL;
}

const void* macos_sapp_macos_get_window(void) {
    return NULL;
}

const void* macos_sapp_ios_get_window(void) {
    return NULL;
}

const void* macos_sapp_d3d11_get_device(void) {
    return NULL;
}

const void* macos_sapp_d3d11_get_device_context(void) {
    return NULL;
}

const void* macos_sapp_d3d11_get_swap_chain(void) {
    return NULL;
}

const void* macos_sapp_d3d11_get_render_view(void) {
    return NULL;
}

const void* macos_sapp_d3d11_get_resolve_view(void) {
    return NULL;
}

const void* macos_sapp_d3d11_get_depth_stencil_view(void) {
    return NULL;
}

const void* macos_sapp_win32_get_hwnd(void) {
    return NULL;
}

const void* macos_sapp_wgpu_get_device(void) {
    return NULL;
}

const void* macos_sapp_wgpu_get_render_view(void) {
    return NULL;
}

const void* macos_sapp_wgpu_get_resolve_view(void) {
    return NULL;
}

const void* macos_sapp_wgpu_get_depth_stencil_view(void) {
    return NULL;
}

uint32_t macos_sapp_gl_get_framebuffer(void) {
    return 0;
}

int macos_sapp_gl_get_major_version(void) {
    return 4;
}

int macos_sapp_gl_get_minor_version(void) {
    return 1;
}

const void* macos_sapp_android_get_native_activity(void) {
    return NULL;
}

/*
 * sokol_gfx stub implementations
 */

static sg_desc _macos_sg_desc = {0};
static bool _macos_sg_initialized = false;

void macos_sg_setup(const sg_desc* desc) {
    _macos_not_implemented("sg_setup");
    if (desc) {
        _macos_sg_desc = *desc;
    }
    _macos_sg_initialized = true;
}

void macos_sg_shutdown(void) {
    _macos_sg_initialized = false;
}

bool macos_sg_isvalid(void) {
    return _macos_sg_initialized;
}

void macos_sg_reset_state_cache(void) {
}

sg_trace_hooks macos_sg_install_trace_hooks(const sg_trace_hooks* trace_hooks) {
    (void)trace_hooks;
    sg_trace_hooks hooks = {0};
    return hooks;
}

void macos_sg_push_debug_group(const char* name) {
    (void)name;
}

void macos_sg_pop_debug_group(void) {
}

bool macos_sg_add_commit_listener(sg_commit_listener listener) {
    (void)listener;
    return false;
}

bool macos_sg_remove_commit_listener(sg_commit_listener listener) {
    (void)listener;
    return false;
}

sg_buffer macos_sg_make_buffer(const sg_buffer_desc* desc) {
    (void)desc;
    sg_buffer buf = {0};
    return buf;
}

sg_image macos_sg_make_image(const sg_image_desc* desc) {
    (void)desc;
    sg_image img = {0};
    return img;
}

sg_sampler macos_sg_make_sampler(const sg_sampler_desc* desc) {
    (void)desc;
    sg_sampler smp = {0};
    return smp;
}

sg_shader macos_sg_make_shader(const sg_shader_desc* desc) {
    (void)desc;
    sg_shader shd = {0};
    return shd;
}

sg_pipeline macos_sg_make_pipeline(const sg_pipeline_desc* desc) {
    (void)desc;
    sg_pipeline pip = {0};
    return pip;
}

sg_attachments macos_sg_make_attachments(const sg_attachments_desc* desc) {
    (void)desc;
    sg_attachments atts = {0};
    return atts;
}

void macos_sg_destroy_buffer(sg_buffer buf) {
    (void)buf;
}

void macos_sg_destroy_image(sg_image img) {
    (void)img;
}

void macos_sg_destroy_sampler(sg_sampler smp) {
    (void)smp;
}

void macos_sg_destroy_shader(sg_shader shd) {
    (void)shd;
}

void macos_sg_destroy_pipeline(sg_pipeline pip) {
    (void)pip;
}

void macos_sg_destroy_attachments(sg_attachments atts) {
    (void)atts;
}

void macos_sg_update_buffer(sg_buffer buf, const sg_range* data) {
    (void)buf;
    (void)data;
}

void macos_sg_update_image(sg_image img, const sg_image_data* data) {
    (void)img;
    (void)data;
}

int macos_sg_append_buffer(sg_buffer buf, const sg_range* data) {
    (void)buf;
    (void)data;
    return 0;
}

bool macos_sg_query_buffer_overflow(sg_buffer buf) {
    (void)buf;
    return false;
}

bool macos_sg_query_buffer_will_overflow(sg_buffer buf, size_t size) {
    (void)buf;
    (void)size;
    return false;
}

void macos_sg_begin_pass(const sg_pass* pass) {
    (void)pass;
}

void macos_sg_apply_viewport(int x, int y, int width, int height, bool origin_top_left) {
    (void)x; (void)y; (void)width; (void)height; (void)origin_top_left;
}

void macos_sg_apply_viewportf(float x, float y, float width, float height, bool origin_top_left) {
    (void)x; (void)y; (void)width; (void)height; (void)origin_top_left;
}

void macos_sg_apply_scissor_rect(int x, int y, int width, int height, bool origin_top_left) {
    (void)x; (void)y; (void)width; (void)height; (void)origin_top_left;
}

void macos_sg_apply_scissor_rectf(float x, float y, float width, float height, bool origin_top_left) {
    (void)x; (void)y; (void)width; (void)height; (void)origin_top_left;
}

void macos_sg_apply_pipeline(sg_pipeline pip) {
    (void)pip;
}

void macos_sg_apply_bindings(const sg_bindings* bindings) {
    (void)bindings;
}

void macos_sg_apply_uniforms(int ub_slot, const sg_range* data) {
    (void)ub_slot;
    (void)data;
}

void macos_sg_draw(int base_element, int num_elements, int num_instances) {
    (void)base_element;
    (void)num_elements;
    (void)num_instances;
}

void macos_sg_end_pass(void) {
}

void macos_sg_commit(void) {
}

sg_desc macos_sg_query_desc(void) {
    return _macos_sg_desc;
}

sg_backend macos_sg_query_backend(void) {
    return SG_BACKEND_METAL_MACOS;
}

sg_features macos_sg_query_features(void) {
    sg_features features = {0};
    return features;
}

sg_limits macos_sg_query_limits(void) {
    sg_limits limits = {0};
    return limits;
}

sg_pixelformat_info macos_sg_query_pixelformat(sg_pixel_format fmt) {
    (void)fmt;
    sg_pixelformat_info info = {0};
    return info;
}

int macos_sg_query_row_pitch(sg_pixel_format fmt, int width, int row_align_bytes) {
    (void)fmt; (void)width; (void)row_align_bytes;
    return 0;
}

int macos_sg_query_surface_pitch(sg_pixel_format fmt, int width, int height, int row_align_bytes) {
    (void)fmt; (void)width; (void)height; (void)row_align_bytes;
    return 0;
}

sg_resource_state macos_sg_query_buffer_state(sg_buffer buf) {
    (void)buf;
    return SG_RESOURCESTATE_INVALID;
}

sg_resource_state macos_sg_query_image_state(sg_image img) {
    (void)img;
    return SG_RESOURCESTATE_INVALID;
}

sg_resource_state macos_sg_query_sampler_state(sg_sampler smp) {
    (void)smp;
    return SG_RESOURCESTATE_INVALID;
}

sg_resource_state macos_sg_query_shader_state(sg_shader shd) {
    (void)shd;
    return SG_RESOURCESTATE_INVALID;
}

sg_resource_state macos_sg_query_pipeline_state(sg_pipeline pip) {
    (void)pip;
    return SG_RESOURCESTATE_INVALID;
}

sg_resource_state macos_sg_query_attachments_state(sg_attachments atts) {
    (void)atts;
    return SG_RESOURCESTATE_INVALID;
}

sg_buffer_info macos_sg_query_buffer_info(sg_buffer buf) {
    (void)buf;
    sg_buffer_info info = {0};
    return info;
}

sg_image_info macos_sg_query_image_info(sg_image img) {
    (void)img;
    sg_image_info info = {0};
    return info;
}

sg_sampler_info macos_sg_query_sampler_info(sg_sampler smp) {
    (void)smp;
    sg_sampler_info info = {0};
    return info;
}

sg_shader_info macos_sg_query_shader_info(sg_shader shd) {
    (void)shd;
    sg_shader_info info = {0};
    return info;
}

sg_pipeline_info macos_sg_query_pipeline_info(sg_pipeline pip) {
    (void)pip;
    sg_pipeline_info info = {0};
    return info;
}

sg_attachments_info macos_sg_query_attachments_info(sg_attachments atts) {
    (void)atts;
    sg_attachments_info info = {0};
    return info;
}

sg_buffer_desc macos_sg_query_buffer_desc(sg_buffer buf) {
    (void)buf;
    sg_buffer_desc desc = {0};
    return desc;
}

sg_image_desc macos_sg_query_image_desc(sg_image img) {
    (void)img;
    sg_image_desc desc = {0};
    return desc;
}

sg_sampler_desc macos_sg_query_sampler_desc(sg_sampler smp) {
    (void)smp;
    sg_sampler_desc desc = {0};
    return desc;
}

sg_shader_desc macos_sg_query_shader_desc(sg_shader shd) {
    (void)shd;
    sg_shader_desc desc = {0};
    return desc;
}

sg_pipeline_desc macos_sg_query_pipeline_desc(sg_pipeline pip) {
    (void)pip;
    sg_pipeline_desc desc = {0};
    return desc;
}

sg_attachments_desc macos_sg_query_attachments_desc(sg_attachments atts) {
    (void)atts;
    sg_attachments_desc desc = {0};
    return desc;
}

sg_buffer_desc macos_sg_query_buffer_defaults(const sg_buffer_desc* desc) {
    if (desc) return *desc;
    sg_buffer_desc d = {0};
    return d;
}

sg_image_desc macos_sg_query_image_defaults(const sg_image_desc* desc) {
    if (desc) return *desc;
    sg_image_desc d = {0};
    return d;
}

sg_sampler_desc macos_sg_query_sampler_defaults(const sg_sampler_desc* desc) {
    if (desc) return *desc;
    sg_sampler_desc d = {0};
    return d;
}

sg_shader_desc macos_sg_query_shader_defaults(const sg_shader_desc* desc) {
    if (desc) return *desc;
    sg_shader_desc d = {0};
    return d;
}

sg_pipeline_desc macos_sg_query_pipeline_defaults(const sg_pipeline_desc* desc) {
    if (desc) return *desc;
    sg_pipeline_desc d = {0};
    return d;
}

sg_attachments_desc macos_sg_query_attachments_defaults(const sg_attachments_desc* desc) {
    if (desc) return *desc;
    sg_attachments_desc d = {0};
    return d;
}

sg_buffer macos_sg_alloc_buffer(void) {
    sg_buffer buf = {0};
    return buf;
}

sg_image macos_sg_alloc_image(void) {
    sg_image img = {0};
    return img;
}

sg_sampler macos_sg_alloc_sampler(void) {
    sg_sampler smp = {0};
    return smp;
}

sg_shader macos_sg_alloc_shader(void) {
    sg_shader shd = {0};
    return shd;
}

sg_pipeline macos_sg_alloc_pipeline(void) {
    sg_pipeline pip = {0};
    return pip;
}

sg_attachments macos_sg_alloc_attachments(void) {
    sg_attachments atts = {0};
    return atts;
}

void macos_sg_dealloc_buffer(sg_buffer buf) {
    (void)buf;
}

void macos_sg_dealloc_image(sg_image img) {
    (void)img;
}

void macos_sg_dealloc_sampler(sg_sampler smp) {
    (void)smp;
}

void macos_sg_dealloc_shader(sg_shader shd) {
    (void)shd;
}

void macos_sg_dealloc_pipeline(sg_pipeline pip) {
    (void)pip;
}

void macos_sg_dealloc_attachments(sg_attachments attachments) {
    (void)attachments;
}

void macos_sg_init_buffer(sg_buffer buf, const sg_buffer_desc* desc) {
    (void)buf; (void)desc;
}

void macos_sg_init_image(sg_image img, const sg_image_desc* desc) {
    (void)img; (void)desc;
}

void macos_sg_init_sampler(sg_sampler smg, const sg_sampler_desc* desc) {
    (void)smg; (void)desc;
}

void macos_sg_init_shader(sg_shader shd, const sg_shader_desc* desc) {
    (void)shd; (void)desc;
}

void macos_sg_init_pipeline(sg_pipeline pip, const sg_pipeline_desc* desc) {
    (void)pip; (void)desc;
}

void macos_sg_init_attachments(sg_attachments attachments, const sg_attachments_desc* desc) {
    (void)attachments; (void)desc;
}

void macos_sg_uninit_buffer(sg_buffer buf) {
    (void)buf;
}

void macos_sg_uninit_image(sg_image img) {
    (void)img;
}

void macos_sg_uninit_sampler(sg_sampler smp) {
    (void)smp;
}

void macos_sg_uninit_shader(sg_shader shd) {
    (void)shd;
}

void macos_sg_uninit_pipeline(sg_pipeline pip) {
    (void)pip;
}

void macos_sg_uninit_attachments(sg_attachments atts) {
    (void)atts;
}

void macos_sg_fail_buffer(sg_buffer buf) {
    (void)buf;
}

void macos_sg_fail_image(sg_image img) {
    (void)img;
}

void macos_sg_fail_sampler(sg_sampler smp) {
    (void)smp;
}

void macos_sg_fail_shader(sg_shader shd) {
    (void)shd;
}

void macos_sg_fail_pipeline(sg_pipeline pip) {
    (void)pip;
}

void macos_sg_fail_attachments(sg_attachments atts) {
    (void)atts;
}

void macos_sg_enable_frame_stats(void) {
}

void macos_sg_disable_frame_stats(void) {
}

bool macos_sg_frame_stats_enabled(void) {
    return false;
}

sg_frame_stats macos_sg_query_frame_stats(void) {
    sg_frame_stats stats = {0};
    return stats;
}

const void* macos_sg_d3d11_device(void) {
    return NULL;
}

const void* macos_sg_d3d11_device_context(void) {
    return NULL;
}

sg_d3d11_buffer_info macos_sg_d3d11_query_buffer_info(sg_buffer buf) {
    (void)buf;
    sg_d3d11_buffer_info info = {0};
    return info;
}

sg_d3d11_image_info macos_sg_d3d11_query_image_info(sg_image img) {
    (void)img;
    sg_d3d11_image_info info = {0};
    return info;
}

sg_d3d11_sampler_info macos_sg_d3d11_query_sampler_info(sg_sampler smp) {
    (void)smp;
    sg_d3d11_sampler_info info = {0};
    return info;
}

sg_d3d11_shader_info macos_sg_d3d11_query_shader_info(sg_shader shd) {
    (void)shd;
    sg_d3d11_shader_info info = {0};
    return info;
}

sg_d3d11_pipeline_info macos_sg_d3d11_query_pipeline_info(sg_pipeline pip) {
    (void)pip;
    sg_d3d11_pipeline_info info = {0};
    return info;
}

sg_d3d11_attachments_info macos_sg_d3d11_query_attachments_info(sg_attachments atts) {
    (void)atts;
    sg_d3d11_attachments_info info = {0};
    return info;
}

const void* macos_sg_mtl_device(void) {
    return NULL;
}

const void* macos_sg_mtl_render_command_encoder(void) {
    return NULL;
}

sg_mtl_buffer_info macos_sg_mtl_query_buffer_info(sg_buffer buf) {
    (void)buf;
    sg_mtl_buffer_info info = {0};
    return info;
}

sg_mtl_image_info macos_sg_mtl_query_image_info(sg_image img) {
    (void)img;
    sg_mtl_image_info info = {0};
    return info;
}

sg_mtl_sampler_info macos_sg_mtl_query_sampler_info(sg_sampler smp) {
    (void)smp;
    sg_mtl_sampler_info info = {0};
    return info;
}

sg_mtl_shader_info macos_sg_mtl_query_shader_info(sg_shader shd) {
    (void)shd;
    sg_mtl_shader_info info = {0};
    return info;
}

sg_mtl_pipeline_info macos_sg_mtl_query_pipeline_info(sg_pipeline pip) {
    (void)pip;
    sg_mtl_pipeline_info info = {0};
    return info;
}

const void* macos_sg_wgpu_device(void) {
    return NULL;
}

const void* macos_sg_wgpu_queue(void) {
    return NULL;
}

const void* macos_sg_wgpu_command_encoder(void) {
    return NULL;
}

const void* macos_sg_wgpu_render_pass_encoder(void) {
    return NULL;
}

sg_wgpu_buffer_info macos_sg_wgpu_query_buffer_info(sg_buffer buf) {
    (void)buf;
    sg_wgpu_buffer_info info = {0};
    return info;
}

sg_wgpu_image_info macos_sg_wgpu_query_image_info(sg_image img) {
    (void)img;
    sg_wgpu_image_info info = {0};
    return info;
}

sg_wgpu_sampler_info macos_sg_wgpu_query_sampler_info(sg_sampler smp) {
    (void)smp;
    sg_wgpu_sampler_info info = {0};
    return info;
}

sg_wgpu_shader_info macos_sg_wgpu_query_shader_info(sg_shader shd) {
    (void)shd;
    sg_wgpu_shader_info info = {0};
    return info;
}

sg_wgpu_pipeline_info macos_sg_wgpu_query_pipeline_info(sg_pipeline pip) {
    (void)pip;
    sg_wgpu_pipeline_info info = {0};
    return info;
}

sg_wgpu_attachments_info macos_sg_wgpu_query_attachments_info(sg_attachments atts) {
    (void)atts;
    sg_wgpu_attachments_info info = {0};
    return info;
}

sg_gl_buffer_info macos_sg_gl_query_buffer_info(sg_buffer buf) {
    (void)buf;
    sg_gl_buffer_info info = {0};
    return info;
}

sg_gl_image_info macos_sg_gl_query_image_info(sg_image img) {
    (void)img;
    sg_gl_image_info info = {0};
    return info;
}

sg_gl_sampler_info macos_sg_gl_query_sampler_info(sg_sampler smp) {
    (void)smp;
    sg_gl_sampler_info info = {0};
    return info;
}

sg_gl_shader_info macos_sg_gl_query_shader_info(sg_shader shd) {
    (void)shd;
    sg_gl_shader_info info = {0};
    return info;
}

sg_gl_attachments_info macos_sg_gl_query_attachments_info(sg_attachments atts) {
    (void)atts;
    sg_gl_attachments_info info = {0};
    return info;
}
