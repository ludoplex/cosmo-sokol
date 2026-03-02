# integration.sig - Public API for cosmo-sokol + Nuklear
# Function signatures that other projects (tedit-cosmo) call
#
# These are the REUSABLE entry points into the GUI framework

module cosmo_sokol {
    # ── App Lifecycle ──────────────────────────────────────────────────────────
    fn run(app: *CosmoApp) -> i32 [doc: "Run the application main loop. Returns exit code."]
    fn quit() -> void [doc: "Request application exit"]
    fn is_running() -> i32 [doc: "Returns 1 if app is running"]

    # ── Window ─────────────────────────────────────────────────────────────────
    fn window_width() -> i32 [doc: "Get window width"]
    fn window_height() -> i32 [doc: "Get window height"]
    fn window_set_title(title: *char) -> void [doc: "Set window title"]
    fn window_is_focused() -> i32 [doc: "Returns 1 if window has focus"]

    # ── Input ──────────────────────────────────────────────────────────────────
    fn key_down(key: i32) -> i32 [doc: "Check if key is pressed"]
    fn key_pressed(key: i32) -> i32 [doc: "Check if key was just pressed this frame"]
    fn mouse_pos(x: *f32, y: *f32) -> void [doc: "Get mouse position"]
    fn mouse_down(button: i32) -> i32 [doc: "Check if mouse button is pressed"]
    fn mods() -> i32 [doc: "Get modifier key state (shift, ctrl, alt, super)"]

    # ── Timing ─────────────────────────────────────────────────────────────────
    fn time() -> f64 [doc: "Seconds since app start"]
    fn delta_time() -> f64 [doc: "Seconds since last frame"]
    fn frame_count() -> u64 [doc: "Total frames rendered"]

    # ── Theme ──────────────────────────────────────────────────────────────────
    fn set_theme(theme: *CosmoTheme) -> void [doc: "Apply a UI theme"]
    fn get_theme() -> *CosmoTheme [doc: "Get current theme"]
}

module cosmo_ui {
    # ── Common UI Components ───────────────────────────────────────────────────
    # These wrap Nuklear with consistent styling

    fn menu_bar(ctx: *nk_context, bar: *CosmoMenuBar) -> i32 [doc: "Render menu bar. Returns selected action ID or 0."]
    fn status_bar(ctx: *nk_context, bar: *CosmoStatusBar) -> void [doc: "Render status bar"]

    fn toolbar_begin(ctx: *nk_context, height: f32) -> i32 [doc: "Begin toolbar row"]
    fn toolbar_button(ctx: *nk_context, label: *char) -> i32 [doc: "Toolbar button. Returns 1 if clicked."]
    fn toolbar_separator(ctx: *nk_context) -> void [doc: "Toolbar separator"]
    fn toolbar_end(ctx: *nk_context) -> void [doc: "End toolbar row"]

    fn dialog_message(ctx: *nk_context, title: *char, message: *char) -> i32 [doc: "Show message dialog. Returns 1 when closed."]
    fn dialog_confirm(ctx: *nk_context, title: *char, message: *char) -> i32 [doc: "Show confirm dialog. Returns 1=OK, 2=Cancel, 0=still open."]
    fn dialog_input(ctx: *nk_context, title: *char, prompt: *char, buffer: *char, len: i32) -> i32 [doc: "Show input dialog. Returns 1=OK, 2=Cancel, 0=still open."]
}
