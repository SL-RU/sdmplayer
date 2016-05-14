#include "sys_gui.h"

uint8_t sys_gui_debug_mode = 1;


void sys_draw_gui(void)
{
	gui_setFont(&DEFAULT_FONT);
	sys_draw_gui_header();
	if(sys_gui_debug_mode)
		sys_gui_debug_draw();
	gui_setFont(&DEFAULT_FONT);
}
void sys_update_gui(void)
{
}
uint8_t sys_input_handler_gui(uint8_t key, uint32_t arg)
{
	if(sys_gui_debug_mode)
		return sys_gui_debug_input(key, arg);
	return SYS_NOT_HANDLED;
}


void sys_draw_gui_header(void)
{
	xPortGetFreeHeapSize();
	gui_setOrigin(0, 0);
	gui_setFont(&Font_4x6);
	
	
	RTC_TimeTypeDef time;
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	gui_lablef(107, 0, 128, Font_4x6.FontHeight, 0, 0, "%02d:%02d", time.Hours, time.Minutes);
	gui_lablef(28, 0, 50, Font_4x6.FontHeight, 0, 0, "R:%lu", xPortGetFreeHeapSize());
	gui_line(0, 6, 128, 6, 1);
	//SSD1306_DrawFilledRectangle(0, 8, 128, 59, 0);
}




void sys_draw_gui_menu(void)
{
}
void sys_update_gui_menu(void)
{
}
uint8_t sys_input_handler_gui_menu(int8_t key, uint32_t arg)
{
	return SYS_NOT_HANDLED;
}


uint8_t sys_gui_debug_x = 30, sys_gui_debug_y = 30;
void sys_gui_debug_show(void)
{
	sys_gui_debug_mode = 1;
	sys_gui_debug_x = 30;
	sys_gui_debug_y = 30;
}
void sys_gui_debug_close(void)
{
	sys_gui_debug_mode = 0;
}
void sys_gui_debug_draw(void)
{
	gui_setFont(&Font_4x6);
	gui_setOrigin(0, 0);
	
	gui_lablef(0, 0, 27, Font_4x6.FontHeight, 0, 0, "%d;%d", sys_gui_debug_x, sys_gui_debug_y);
	
	gui_point(sys_gui_debug_x, sys_gui_debug_y, (HAL_GetTick()/300)%2);
	gui_point(sys_gui_debug_x + 1, sys_gui_debug_y, 1);
	gui_point(sys_gui_debug_x - 1, sys_gui_debug_y, 1);
	gui_point(sys_gui_debug_x, sys_gui_debug_y + 1, 1);
	gui_point(sys_gui_debug_x, sys_gui_debug_y - 1, 1);
	
	
	gui_setFont(&Font_4x6);
	
}
uint8_t sys_gui_debug_xs = 30, sys_gui_debug_ys = 30;
uint32_t sys_gui_debug_lst = 0;
uint8_t sys_gui_debug_input(int8_t key, uint32_t arg)
{
	uint8_t speed = 60;
	if(arg == KEYBOARD_DOWN)
	{
		sys_gui_debug_xs = sys_gui_debug_x;
		sys_gui_debug_ys = sys_gui_debug_y;
		sys_gui_debug_lst = arg;
		return SYS_HANDLED;
	}
	if(arg != KEYBOARD_UP)
	{
		sys_gui_debug_lst = arg;
		if(key == 4 || key == 6)
			sys_gui_debug_x = sys_gui_debug_xs + (arg/ speed) * (key == 6 ? 1 : -1);
		if(key == 2 || key == 8)
			sys_gui_debug_y = sys_gui_debug_ys + (arg/ speed) * (key == 8 ? 1 : -1);
		return SYS_HANDLED;
	}
	else
	{
		if(sys_gui_debug_lst < 130)
		{
			if(key == 4 || key == 6)
				sys_gui_debug_x = sys_gui_debug_xs + (key == 6 ? 1 : -1);
			if(key == 2 || key == 8)
				sys_gui_debug_y = sys_gui_debug_ys + (key == 8 ? 1 : -1);
		}
		if(key == 0)
		{
			sys_gui_debug_mode = 0;
		}
		sys_gui_debug_xs = sys_gui_debug_x;
		sys_gui_debug_ys = sys_gui_debug_y;
		sys_gui_debug_lst = arg;
		return SYS_HANDLED;
	}
	return SYS_NOT_HANDLED;
}
