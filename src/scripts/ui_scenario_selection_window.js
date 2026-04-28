log_info("akhenaten: ui scenario selection window started")

scenario_selection_window {
    center_window : true
    pos [(sw(0) - px(40))/2, (sh(0) - px(30))/2]

    ui {
        background_image : background({cover:true, fill_width:true, fill_height:true})
        background       : outer_panel({size[40, 30]})
    }
}
