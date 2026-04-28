#include "config.h"

#include "graphics/window.h"
#include "input/input.h"
#include "window/autoconfig_window.h"

namespace {

struct hotkey_editor_window_t : autoconfig_window {
    int action = 0;
    int index = 0;
    e_key key = KEY_NONE;
    e_key_mode modifiers = KEY_MOD_NONE;
    void (*callback)(int, int, e_key, e_key_mode) = nullptr;

    hotkey_editor_window_t() : autoconfig_window("hotkey_editor_window") {}

    virtual int handle_mouse(const mouse* m) override { return 0; }
    virtual int get_tooltip_text() override { return 0; }
    virtual void draw_foreground(UiFlags flags) override {}
    virtual xstring get_section() const override { return "hotkey_editor_window"; }

    virtual void init() override {
        autoconfig_window::init();
        ui["btn_cancel"].onclick([this] { close(false); });
        ui["btn_ok"].onclick([this] { close(true); });
        sync_ui();
    }

    virtual int draw_background(UiFlags flags) override {
        sync_ui();
        return autoconfig_window::draw_background(flags);
    }

    virtual void ui_draw_foreground(UiFlags flags) override {
        sync_ui();
        autoconfig_window::ui_draw_foreground(flags);
    }

    virtual int ui_handle_mouse(const mouse* m) override {
        autoconfig_window::ui_handle_mouse(m);

        if (m->right.went_up || input_go_back_requested(m, hotkey_state())) {
            close(false);
        }

        return 0;
    }

    void sync_ui() {
        ui["key_value"] = (pcstr)key_combination_display_name(key, modifiers);
    }

    void close(bool save) {
        if (save && callback) {
            callback(action, index, key, modifiers);
        }

        window_go_back();
    }
};

hotkey_editor_window_t g_hotkey_editor_window;

}

void window_hotkey_editor_key_pressed(int key, int modifiers) {
    if (key == KEY_ENTER && modifiers == KEY_MOD_NONE) {
        g_hotkey_editor_window.close(true);
    } else if (key == KEY_ESCAPE && modifiers == KEY_MOD_NONE) {
        g_hotkey_editor_window.close(false);
    } else {
        if (key != KEY_NONE) {
            g_hotkey_editor_window.key = (e_key)key;
        }

        g_hotkey_editor_window.modifiers = (e_key_mode)modifiers;
    }
}

void window_hotkey_editor_key_released(int key, int modifiers) {
    if (g_hotkey_editor_window.key == KEY_NONE && key == KEY_NONE) {
        g_hotkey_editor_window.modifiers = (e_key_mode)modifiers;
    }
}

void window_hotkey_editor_show(int action, int index, void (*callback)(int, int, e_key, e_key_mode)) {
    window_type window = {
        "window_hotkey_editor",
        [] (int flags) { g_hotkey_editor_window.draw_background(flags); },
        [] (int flags) { g_hotkey_editor_window.ui_draw_foreground(flags); },
        [] (const mouse* m, const hotkeys* h) { g_hotkey_editor_window.ui_handle_mouse(m); }
    };

    g_hotkey_editor_window.action = action;
    g_hotkey_editor_window.index = index;
    g_hotkey_editor_window.callback = callback;
    g_hotkey_editor_window.key = KEY_NONE;
    g_hotkey_editor_window.modifiers = KEY_MOD_NONE;
    g_hotkey_editor_window.init();
    window_show(&window);
}
