log_info("akhenaten: ui victory dialog window started")

victory_dialog_window {
    center_window : true
    pos [(sw(0) - px(34))/2, (sh(0) - px(15))/2]

    ui {
        background  : outer_panel({size[34, 15]})
        title       : header({pos[16, 16], size[px(32), 20], align:"center"})
        subtitle    : text({pos[16, 48], size[px(32), 48], align:"center", multiline:true, wrap:px(30), font:FONT_NORMAL_BLACK_ON_LIGHT})

        btn_primary   : button({pos[32, 112], size[480, 24], font:FONT_NORMAL_BLACK_ON_DARK})
        btn_secondary : button({pos[32, 144], size[480, 24], font:FONT_NORMAL_BLACK_ON_DARK})
        btn_tertiary  : button({pos[32, 176], size[480, 24], font:FONT_NORMAL_BLACK_ON_DARK})
    }
}
