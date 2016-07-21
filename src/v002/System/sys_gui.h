#ifndef SYS_GUI_H
#define SYS_GUI_H 1

#include "system.h"
#include "sys_gui_menu.h"

uint8_t sys_gui_draw_post(void);
void sys_gui_draw_pre(void);
void sys_gui_update(void);
uint8_t sys_gui_input_handler_post(uint8_t key, uint32_t arg);
uint8_t sys_gui_input_handler_pre(uint8_t key, uint32_t arg);


void sys_gui_header_draw(void);


void sys_gui_debug_start(void);
void sys_gui_debug_close(void);
void sys_gui_debug_draw(void);
uint8_t sys_gui_debug_input(int8_t key, uint32_t arg);



#endif
