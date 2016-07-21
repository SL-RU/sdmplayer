#ifndef SYS_GUI_MENU_H
#define SYS_GUI_MENU_H 1

#include "sys_gui.h"

//current menu state:
//0 - closed
//1 - press indicator
//2 - opened
extern uint8_t sys_gui_menu_mode;

void sys_gui_menu_open(void);
void sys_gui_menu_close(void);
void sys_gui_menu_draw(void);
void sys_gui_menu_update(void);
uint8_t sys_gui_menu_input_handler(int8_t key, uint32_t arg);

#endif
