log_info("akhenaten: ui numeric input window started")

numeric_input_window {
    ui {
        background    : outer_panel({size[8, 14]})
        value_back    : border({pos[16, 16], size[96, 30]})
        value         : text({pos[16, 19], size[92, 20], font:FONT_NORMAL_BLUE, align:"center"})

        num1          : button({pos[21, 51], size[25, 25], text:"1", font:FONT_NORMAL_BLUE})
        num2          : button({pos[51, 51], size[25, 25], text:"2", font:FONT_NORMAL_BLUE})
        num3          : button({pos[81, 51], size[25, 25], text:"3", font:FONT_NORMAL_BLUE})
        num4          : button({pos[21, 81], size[25, 25], text:"4", font:FONT_NORMAL_BLUE})
        num5          : button({pos[51, 81], size[25, 25], text:"5", font:FONT_NORMAL_BLUE})
        num6          : button({pos[81, 81], size[25, 25], text:"6", font:FONT_NORMAL_BLUE})
        num7          : button({pos[21, 111], size[25, 25], text:"7", font:FONT_NORMAL_BLUE})
        num8          : button({pos[51, 111], size[25, 25], text:"8", font:FONT_NORMAL_BLUE})
        num9          : button({pos[81, 111], size[25, 25], text:"9", font:FONT_NORMAL_BLUE})
        num0          : button({pos[21, 141], size[25, 25], text:"0", font:FONT_NORMAL_BLUE})
        accept_btn    : button({pos[51, 141], size[55, 25], text[44, 16], font:FONT_SMALL_PLAIN})
        cancel_btn    : button({pos[21, 171], size[85, 25], text[44, 17], font:FONT_SMALL_PLAIN})
    }
}
