#include "sys_gui_menu.h"

uint8_t sys_gui_menu_mode = 0;
uint8_t sys_gui_menu_focus = 0;
void sys_gui_menu_debug_click(uint16_t id, uint32_t arg, uint8_t eventType)
{
	sys_gui_debug_start();
	sys_gui_menu_mode = 0;
}
char* sys_gui_menu_lables[] = {
	"vol",
	"apps",
	"mus",
	"sett",
	"dbg"
};
void sys_gui_menu_open(void)
{
	sys_gui_menu_mode = 2;
	sys_gui_menu_focus = 0;
}
void sys_gui_menu_close(void)
{
	sys_gui_menu_mode = 0;
}
void sys_gui_menu_draw(void)
{
	if(sys_gui_menu_mode == 1)
	{
		gui_rect_fill(0, 59, 64, 5, (HAL_GetTick()/300)%2, (HAL_GetTick()/300)%2);
		return;
	}
	if(sys_gui_menu_mode != 2)
		return;
	gui_setFont(&Font_7x10);
	
	gui_setOrigin(0, SYS_GUI_HEADER_HIGHT);
	
	gui_rect_fill(0,0, 128, 64 - SYS_GUI_HEADER_HIGHT + 1, 0, 0);
	uint8_t i = 0, h = (64-SYS_GUI_HEADER_HIGHT-2)/5;
	for(i=0;i<5;i++)
  {
		gui_lable(sys_gui_menu_lables[i], 0, h*i, 31, h, sys_gui_menu_focus == i, sys_gui_menu_focus == i);
	}
	gui_line(30, 0, 30, 64-SYS_GUI_HEADER_HIGHT, 1);
	switch(sys_gui_menu_focus)
	{
		case 0: break;
		case 1: 
			gui_lable("123456789012345678901234567890", 31, 0, 97, h, 0, 1);
			break;
	}
}
void sys_gui_menu_update(void)
{
}
uint8_t sys_gui_menu_input_handler(int8_t key, uint32_t arg)
{
	if(arg == KEYBOARD_UP)
	{
		if(key == 'a')
		{
			sys_gui_menu_close();
			return SYS_HANDLED;
		}
		if(key == 2)
		{
			if(sys_gui_menu_focus == 0)
				sys_gui_menu_focus = 4;
			else
				sys_gui_menu_focus--;
			return SYS_HANDLED;
		}
		if(key == 8)
		{
			if(sys_gui_menu_focus == 4)
				sys_gui_menu_focus = 0;
			else
				sys_gui_menu_focus++;
			return SYS_HANDLED;
		}
		//return gui_list_input(sys_gui_menu_list, key);
	}
	return SYS_NOT_HANDLED;
}
