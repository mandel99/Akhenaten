#include "numeric_input.h"

#include "graphics/window.h"
#include "input/input.h"
#include "input/keyboard.h"
#include "sound/sound.h"
#include "window/autoconfig_window.h"

namespace {

void numeric_input_on_digit(int number);

struct numeric_input_window_t : autoconfig_window {
    int max_digits = 0;
    int max_value = 0;
    void (*callback)(int) = nullptr;
    int num_digits = 0;
    int value = 0;

    numeric_input_window_t() : autoconfig_window("numeric_input_window") {}

    virtual int handle_mouse(const mouse* m) override { return 0; }
    virtual int get_tooltip_text() override { return 0; }
    virtual void draw_foreground(UiFlags flags) override {}
    virtual xstring get_section() const override { return "numeric_input_window"; }

    virtual void init() override {
        autoconfig_window::init();

        ui["num0"].onclick([this] { input_number(0); });
        ui["num1"].onclick([this] { input_number(1); });
        ui["num2"].onclick([this] { input_number(2); });
        ui["num3"].onclick([this] { input_number(3); });
        ui["num4"].onclick([this] { input_number(4); });
        ui["num5"].onclick([this] { input_number(5); });
        ui["num6"].onclick([this] { input_number(6); });
        ui["num7"].onclick([this] { input_number(7); });
        ui["num8"].onclick([this] { input_number(8); });
        ui["num9"].onclick([this] { input_number(9); });
        ui["accept_btn"].onclick([this] { input_accept(); });
        ui["cancel_btn"].onclick([this] { close(); });

        keyboard_start_capture_numeric(numeric_input_on_digit);
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
        if (input_go_back_requested(m, h)) {
            close();
        }

        if (h->enter_pressed) {
            input_accept();
        }

        return 0;
    }

    void sync_ui() {
        if (num_digits > 0) {
            ui["value"].text_var("%d", value);
        } else {
            ui["value"] = "";
        }
    }

    void input_number(int number) {
        if (num_digits < max_digits) {
            value = value * 10 + number;
            num_digits++;
            g_sound.play_effect(SOUND_EFFECT_BUILD);
        }
    }

    void input_accept() {
        close();
        if (value > max_value) {
            value = max_value;
        }
        if (callback) {
            callback(value);
        }
    }

    void close() {
        keyboard_stop_capture_numeric();
        window_go_back();
    }
};

numeric_input_window_t g_numeric_input_window;

void numeric_input_on_digit(int number) {
    g_numeric_input_window.input_number(number);
}

}

void window_numeric_input_accept(void) {
    g_numeric_input_window.input_accept();
}

void window_numeric_input_show(int x, int y, int max_digits, int max_value, void (*callback)(int)) {
    window_type window = {
        "window_numeric_input",
        [] (int flags) { g_numeric_input_window.draw_background(flags); },
        [] (int flags) { g_numeric_input_window.ui_draw_foreground(flags); },
        [] (const mouse* m, const hotkeys* h) { g_numeric_input_window.ui_handle_mouse(m); }
    };

    g_numeric_input_window.pos = {x, y};
    g_numeric_input_window.max_digits = max_digits;
    g_numeric_input_window.max_value = max_value;
    g_numeric_input_window.callback = callback;
    g_numeric_input_window.num_digits = 0;
    g_numeric_input_window.value = 0;
    g_numeric_input_window.init();
    window_show(&window);
}
