log_info("akhenaten: ui hotkey editor window started")

hotkey_editor_window {
    center_window : true
    pos [(sw(0) - px(19))/2, (sh(0) - px(9))/2]

    ui {
        background   : outer_panel({size[19, 9]})
        title        : header({pos[8, 16], size[px(18), 20], text:"#TR_HOTKEY_EDIT_TITLE", align:"center"})
        key_panel    : inner_panel({pos[24, 56], size[16, 2]})
        key_value    : text({pos[24, 65], size[px(16), 20], font:FONT_NORMAL_WHITE_ON_DARK, align:"center"})
        btn_cancel   : button({pos[24, 100], size[120, 24], text:"#TR_BUTTON_CANCEL"})
        btn_ok       : button({pos[160, 100], size[120, 24], text:"#TR_BUTTON_OK"})
    }
}
