#include "window_mission_briefing.h"

#include "core/profiler.h"
#include "game/game_events.h"
#include "game/game.h"
#include "game/mission.h"
#include "graphics/graphics.h"
#include "graphics/window.h"
#include "js/js_game.h"
#include "platform/renderer.h"
#include "scenario/scenario.h"
#include "window/intermezzo.h"

ui::mission_briefing_window g_mission_briefing;

ANK_BOUND_INT(__mission_briefing_scenario_id, g_mission_briefing.scenario_id)
ANK_BOUND_BOOL(__mission_briefing_is_review, g_mission_briefing.is_review)

void __game_mission_branch_start(int scenario_id) {
    ui::mission_briefing_window::mission_start(scenario_id);
}
ANK_FUNCTION_1(__game_mission_branch_start)

void ui::mission_briefing_window::init() {
    autoconfig_window::init();
    const bool is_custom_map = (g_scenario.mode() != e_scenario_normal);
    const int mission = g_scenario.campaign_scenario_id();
    const int image_base = image_id_from_group(GROUP_INTERMEZZO_BACKGROUND);
    if (const image_t* bg = image_get(is_custom_map ? image_base + 1 : image_base + 1 + (mission >= 20))) {
        ui["background_image"].image(bg->desc());
    }

    if (!g_mission_briefing.campaign_mission_loaded) {
        g_mission_briefing.campaign_mission_loaded = 1;
    }
}

void ui::mission_briefing_window::update_goals_list(const xstring& goal) {
    if (last_goal != goal) {
        last_goal = goal;
        ui["goal_immediate"].enabled = !!last_goal;
        ui["goal_immediate"] = last_goal;
    }
}

int ui::mission_briefing_window::draw_background(UiFlags flags) {
    const bool is_custom_map = (g_scenario.mode() != e_scenario_normal);
    const int mission = g_scenario.campaign_scenario_id();
    const int image_base = image_id_from_group(GROUP_INTERMEZZO_BACKGROUND);
    const int image_id = is_custom_map ? image_base + 1 : image_base + 1 + (mission >= 20);

    g_render.clear_screen();
    painter ctx = game.painter();
    ImageDraw::img_background_cover(ctx, image_id);
    autoconfig_window::draw_background(flags);

    update_goals_list(g_scenario.goal_tooltip);

    return 0;
}

void window_mission_briefing_show_impl() {
    static window_type window = {
        "window_mission_briefing",
        [] (int flags) { g_mission_briefing.draw_background(flags); },
        [] (int flags) { g_mission_briefing.ui_draw_foreground(flags); },
        [] (const mouse *m, const hotkeys *h) { g_mission_briefing.ui_handle_mouse(m); }
    };

    events::emit(event_update_mission_goal{ g_mission_briefing.scenario_id });
    g_mission_briefing.init();
    window_show(&window);
}

void ui::mission_briefing_window::mission_start(int scenario_id) {
    auto &data = g_mission_briefing;
    data.scenario_id = scenario_id;
    data.is_review = false;
    data.campaign_mission_loaded = false;
    window_intermezzo_show(scenario_id, INTERMEZZO_MISSION_BRIEFING, window_mission_briefing_show_impl);
}

void ui::mission_briefing_window::mission_review() {
    auto &data = g_mission_briefing;
    data.scenario_id = g_scenario.campaign_scenario_id();
    data.is_review = true;
    data.campaign_mission_loaded = true;
    window_intermezzo_show(data.scenario_id, INTERMEZZO_MISSION_BRIEFING, window_mission_briefing_show_impl);
}
