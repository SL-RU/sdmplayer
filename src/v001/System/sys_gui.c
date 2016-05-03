#include "sys_gui.h"




void sys_draw_gui(void)
{
	sys_draw_gui_header();
}
void sys_update_gui(void)
{
}
uint8_t sys_input_handler_gui(uint8_t key, uint32_t arg)
{
}


void sys_draw_gui_header(void)
{
	gui_lable("HEADER", 0, 0, 128, 13, 1, 1);
}


void sys_draw_gui_menu(void)
{
}
void sys_update_gui_menu(void)
{
}
uint8_t sys_input_handler_gui_menu(uint8_t key, uint32_t arg)
{
}
