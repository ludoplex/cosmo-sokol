# dispatch.sig - Platform dispatch function signatures
# Runtime dispatch layer: sokol_cosmo.c routes to platform implementations
#
# Pattern: sapp_foo() -> {linux,windows,macos}_sapp_foo()
# Selection: IsLinux() | IsWindows() | IsXnu()

module sapp {
    # ── Window Management ──────────────────────────────────────────────────────
    fn run(desc: *sapp_desc) -> void
    fn isvalid() -> i32
    fn width() -> i32
    fn height() -> i32
    fn widthf() -> f32
    fn heightf() -> f32

    # ── Display Properties ─────────────────────────────────────────────────────
    fn high_dpi() -> i32
    fn dpi_scale() -> f32
    fn color_format() -> i32
    fn depth_format() -> i32
    fn sample_count() -> i32

    # ── Window Control ─────────────────────────────────────────────────────────
    fn set_window_title(title: *char) -> void
    fn set_icon(desc: *sapp_icon_desc) -> void
    fn is_fullscreen() -> i32
    fn toggle_fullscreen() -> void
    fn request_quit() -> void
    fn cancel_quit() -> void
    fn quit() -> void

    # ── Input State ────────────────────────────────────────────────────────────
    fn show_mouse(show: i32) -> void
    fn mouse_shown() -> i32
    fn lock_mouse(lock: i32) -> void
    fn mouse_locked() -> i32
    fn set_mouse_cursor(cursor: i32) -> void
    fn get_mouse_cursor() -> i32
    fn show_keyboard(show: i32) -> void
    fn keyboard_shown() -> i32

    # ── Frame Info ─────────────────────────────────────────────────────────────
    fn frame_count() -> u64
    fn frame_duration() -> f64
    fn userdata() -> *void
    fn query_desc() -> sapp_desc
    fn consume_event() -> void

    # ── Clipboard ──────────────────────────────────────────────────────────────
    fn set_clipboard_string(str: *char) -> void
    fn get_clipboard_string() -> *char

    # ── Drag & Drop ────────────────────────────────────────────────────────────
    fn get_num_dropped_files() -> i32
    fn get_dropped_file_path(index: i32) -> *char

    # ── Platform-Specific Accessors ────────────────────────────────────────────
    fn gl_get_framebuffer() -> u32
    fn gl_get_major_version() -> i32
    fn gl_get_minor_version() -> i32
    fn metal_get_device() -> *void
    fn metal_get_current_drawable() -> *void
    fn d3d11_get_device() -> *void
    fn d3d11_get_device_context() -> *void
    fn macos_get_window() -> *void
    fn win32_get_hwnd() -> *void
}

module sg {
    # ── Lifecycle ──────────────────────────────────────────────────────────────
    fn setup(desc: *sg_desc) -> void
    fn shutdown() -> void
    fn isvalid() -> i32
    fn reset_state_cache() -> void
    fn query_backend() -> i32
    fn query_features() -> sg_features
    fn query_limits() -> sg_limits

    # ── Resource Creation ──────────────────────────────────────────────────────
    fn make_buffer(desc: *sg_buffer_desc) -> sg_buffer
    fn make_image(desc: *sg_image_desc) -> sg_image
    fn make_sampler(desc: *sg_sampler_desc) -> sg_sampler
    fn make_shader(desc: *sg_shader_desc) -> sg_shader
    fn make_pipeline(desc: *sg_pipeline_desc) -> sg_pipeline
    fn make_attachments(desc: *sg_attachments_desc) -> sg_attachments

    # ── Resource Destruction ───────────────────────────────────────────────────
    fn destroy_buffer(buf: sg_buffer) -> void
    fn destroy_image(img: sg_image) -> void
    fn destroy_sampler(smp: sg_sampler) -> void
    fn destroy_shader(shd: sg_shader) -> void
    fn destroy_pipeline(pip: sg_pipeline) -> void
    fn destroy_attachments(atts: sg_attachments) -> void

    # ── Rendering ──────────────────────────────────────────────────────────────
    fn begin_pass(pass: *sg_pass) -> void
    fn apply_viewport(x: i32, y: i32, w: i32, h: i32, origin_top_left: i32) -> void
    fn apply_scissor_rect(x: i32, y: i32, w: i32, h: i32, origin_top_left: i32) -> void
    fn apply_pipeline(pip: sg_pipeline) -> void
    fn apply_bindings(bindings: *sg_bindings) -> void
    fn apply_uniforms(stage: i32, data: *sg_range) -> void
    fn draw(base: i32, num: i32, instances: i32) -> void
    fn end_pass() -> void
    fn commit() -> void
}
