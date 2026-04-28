#include "window/console.h"

#include "building/construction/build_planner.h"
#include "city/city.h"
#include "city/city_warnings.h"
#include "core/log.h"
#include "game/cheats.h"
#include "game/game_events.h"
#include "graphics/window.h"
#include "input/input.h"
#include "window/autoconfig_window.h"
#include "window/window_city.h"

namespace {

struct console_window_t : autoconfig_window {
    console_window_t() : autoconfig_window("console_window") {}

    virtual int handle_mouse(const mouse* m) override { return 0; }
    virtual int get_tooltip_text() override { return 0; }
    virtual void draw_foreground(UiFlags flags) override {}
    virtual xstring get_section() const override { return "console_window"; }

    virtual void init() override {
        autoconfig_window::init();
        ui["command_input"].set_value("");
        ui["back_btn"].onclick([this] { close(); });
        ui["send_btn"].onclick([this] { send_command(); });
    }

    virtual int draw_background(UiFlags flags) override {
        window_draw_underlying_window(UiFlags_None);
        return autoconfig_window::draw_background(flags);
    }

    virtual int ui_handle_mouse(const mouse* m) override {
        autoconfig_window::ui_handle_mouse(m);

        if (input_go_back_requested(m, hotkey_state())) {
            close();
            return 0;
        }

        if (ui["command_input"].dcast_einput() && input_box_is_accepted(&ui["command_input"].dcast_einput()->_box)) {
            send_command();
        }

        return 0;
    }

    void close() {
        ui::stop_active_input();
        window_go_back();
    }

    void send_command() {
        xstring command = ui["command_input"].get_value();
        close();
        logs::info("Command received: %s", command.c_str());
        events::emit(event_city_warning{ command });
        game_cheat_parse_command(command.c_str());
    }
};

console_window_t g_console_window;

}

void window_console_show() {
    static window_type window = {
        "window_file_dialog",
        [] (int flags) { g_console_window.draw_background(flags); },
        [] (int flags) { g_console_window.ui_draw_foreground(flags); },
        [] (const mouse* m, const hotkeys* h) { g_console_window.ui_handle_mouse(m); }
    };

    g_console_window.init();
    window_show(&window);
}

void window_show_cheat_console(bool force) {
    if (force) {
        game_cheat_force_activate();
    }

    if (game_cheat_is_active()) {
        g_city_planner.reset();
        window_city_show();
        window_console_show();
    }
}
ANK_FUNCTION_1(window_show_cheat_console)
