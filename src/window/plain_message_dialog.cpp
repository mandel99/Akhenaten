#include "plain_message_dialog.h"

#include "graphics/window.h"
#include "input/input.h"
#include "window/autoconfig_window.h"

namespace {

struct plain_message_dialog_window_t : autoconfig_window {
    xstring title;
    xstring message;
    xstring debug_info;

    plain_message_dialog_window_t() : autoconfig_window("plain_message_dialog_window") {}

    virtual int handle_mouse(const mouse* m) override { return 0; }
    virtual int get_tooltip_text() override { return 0; }
    virtual void draw_foreground(UiFlags flags) override {}
    virtual xstring get_section() const override { return "plain_message_dialog_window"; }

    virtual void init() override {
        autoconfig_window::init();
        ui["btn_ok"].onclick([this] { close(); });
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

        const hotkeys* h = hotkey_state();
        if (input_go_back_requested(m, h) || h->enter_pressed) {
            close();
        }

        return 0;
    }

    void sync_ui() {
        ui["title"] = title;
        ui["message"] = message;
    }

    void close() {
        window_go_back();
    }
};

plain_message_dialog_window_t g_plain_message_dialog_window;

int init_dialog(pcstr title, pcstr message, pcstr info) {
    if (g_window_manager.window_is("window_plain_message_dialog")) {
        return 0;
    }

    g_plain_message_dialog_window.debug_info = info;
    g_plain_message_dialog_window.title = title;
    g_plain_message_dialog_window.message = message;
    return 1;
}

}

void window_plain_message_dialog_show(pcstr title, pcstr message, pcstr debug_info) {
    if (!init_dialog(title, message, debug_info)) {
        return;
    }

    static window_type window = {
        "window_plain_message_dialog",
        [] (int flags) { g_plain_message_dialog_window.draw_background(flags); },
        [] (int flags) { g_plain_message_dialog_window.ui_draw_foreground(flags); },
        [] (const mouse* m, const hotkeys* h) { g_plain_message_dialog_window.ui_handle_mouse(m); }
    };

    g_plain_message_dialog_window.init();
    window_show(&window);
}
