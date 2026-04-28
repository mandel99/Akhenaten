log_info("akhenaten: ui console window started")

console_window {
    center_window : true
    pos [(sw(0) - px(24))/2, (sh(0) - px(8))/2]

    ui {
        background     : outer_panel({size[24, 8]})
        title          : header({pos[0, 16], size[px(24), 20], text:"Console", align:"center"})
        command_label  : text({pos[72, 96], text[12, 0], font:FONT_NORMAL_BLACK_ON_LIGHT})
        send_label     : text({pos[224, 96], text[13, 5], font:FONT_NORMAL_BLACK_ON_LIGHT})
        command_input  : input({pos[32, 48], size[20, 2], font:FONT_NORMAL_WHITE_ON_DARK})
        back_btn       : image_button({pos[31, 89], size[31, 20], pack:PACK_GENERAL, id:90, offset:8})
        send_btn       : image_button({pos[336, 87], size[27, 27], pack:PACK_GENERAL, id:193, offset:4})
    }
}
