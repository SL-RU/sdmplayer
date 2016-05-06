#ifndef SYS_GUI_H
#define SYS_GUI_H 1

#include "system.h"

void sys_draw_gui(void);
void sys_update_gui(void);
uint8_t sys_input_handler_gui(uint8_t key, uint32_t arg);


void sys_draw_gui_header(void);


void sys_draw_gui_menu(void);
void sys_update_gui_menu(void);
uint8_t sys_input_handler_gui_menu(int8_t key, uint32_t arg);



#endif
