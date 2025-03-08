SET(SOURCES screens/ui_Screen1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    fonts/ui_font_timeOutlineFont.c)

add_library(ui ${SOURCES})
