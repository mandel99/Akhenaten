#include "victory_dialog.h"

#include "city/city.h"
#include "city/victory.h"
#include "graphics/window.h"
#include "scenario/scenario.h"
#include "sound/sound.h"
#include "window/autoconfig_window.h"
#include "window/window_city.h"

namespace {

struct victory_dialog_window_t : autoconfig_window {
    victory_dialog_window_t() : autoconfig_window("victory_dialog_window") {}

    virtual int handle_mouse(const mouse* m) override { return 0; }
    virtual int get_tooltip_text() override { return 0; }
    virtual void draw_foreground(UiFlags flags) override {}
    virtual xstring get_section() const override { return "victory_dialog_window"; }

    virtual void init() override {
        autoconfig_window::init();
        ui["btn_primary"].onclick([] { window_city_show(); });
        ui["btn_secondary"].onclick([] { continue_governing(24); });
        ui["btn_tertiary"].onclick([] { continue_governing(60); });
    }

    virtual int draw_background(UiFlags flags) override {
        window_draw_underlying_window(UiFlags_None);
        sync_ui();
        return autoconfig_window::draw_background(flags);
    }

    virtual void ui_draw_foreground(UiFlags flags) override {
        sync_ui();
        autoconfig_window::ui_draw_foreground(flags);
    }

    virtual int ui_handle_mouse(const mouse* m) override {
        return autoconfig_window::ui_handle_mouse(m);
    }

    void sync_ui() {
        const bool is_custom_map = (g_scenario.mode() != e_scenario_normal);
        const int current_scenario = g_scenario.settings.campaign_mission_rank;
        const int next_scenario = current_scenario + 1;
        const bool won = (g_city.victory_state.state == e_victory_state_won);
        const bool show_continue = (current_scenario >= 2 || is_custom_map) && won;

        if (current_scenario < 10 || is_custom_map) {
            ui["title"] = ui::str(62, 0);
            ui["subtitle"].text_var("%s\n%s", ui::str(62, 2), ui::str(32, next_scenario));
        } else {
            ui["title"] = (pcstr)city_player_name();
            ui["subtitle"] = ui::str(62, 26);
        }

        ui["btn_primary"] = ui::str(62, won ? (current_scenario < 10 || is_custom_map ? 3 : 27) : 6);
        ui["btn_secondary"] = ui::str(62, 4);
        ui["btn_tertiary"] = ui::str(62, 5);

        ui["btn_secondary"].enabled = show_continue;
        ui["btn_tertiary"].enabled = show_continue;
    }

    static void continue_governing(int months) {
        g_city.victory_state.continue_governing(months);
        window_city_show();
        g_city.victory_state.reset();
        g_sound.music_update(true);
    }
};

victory_dialog_window_t g_victory_dialog_window;

}

void window_victory_dialog_show(void) {
    static window_type window = {
        "window_victory_dialog",
        [] (int flags) { g_victory_dialog_window.draw_background(flags); },
        [] (int flags) { g_victory_dialog_window.ui_draw_foreground(flags); },
        [] (const mouse* m, const hotkeys* h) { g_victory_dialog_window.ui_handle_mouse(m); }
    };

    g_victory_dialog_window.init();
    window_show(&window);
}
