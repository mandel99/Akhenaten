log_info("akhenaten: ui plain message dialog window started")

plain_message_dialog_window {
    center_window : true
    pos [(sw(0) - px(30))/2, (sh(0) - px(12))/2]

    ui {
        background  : outer_panel({size[30, 12]})
        title       : header({pos[0, 20], size[px(30), 20], align:"center"})
        message     : text({
            pos[20, 56]
            size[px(27), px(6)]
            wrap:px(27)
            multiline:true
            font:FONT_NORMAL_BLACK_ON_LIGHT
        })
        btn_ok      : ok_button({margin{centerx:-20, bottom:-60}})
    }
}
