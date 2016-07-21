#include "sys_gui.h"

uint8_t sys_gui_debug_mode = 0;


uint16_t sys_gui_menu_openKeyDelay = 400;


uint8_t sys_gui_draw_post(void)
{
	gui_setFont(&DEFAULT_FONT);
	sys_gui_header_draw();
	if(sys_gui_menu_mode)
	{
		sys_gui_menu_draw();
		gui_setFont(&DEFAULT_FONT);
	}
	if(sys_gui_debug_mode)
		sys_gui_debug_draw();
	gui_setFont(&DEFAULT_FONT);
	return 0;
}
void sys_gui_draw_pre(void)
{
}
void sys_gui_update(void)
{
}
uint8_t sys_gui_input_handler_post(uint8_t key, uint32_t arg)
{
	if(key == 'a')
	{
		if(arg == KEYBOARD_UP)
			{
				//if(sys_gui_menu_list == 0)
				//	sys_gui_menu_init();
				sys_gui_menu_open();
			}
			else
			{
				sys_gui_menu_mode = 1;
			}
			return SYS_HANDLED;
	}
	return SYS_NOT_HANDLED;
}
uint32_t sys_gui_input_handler_pre_lstarg = 0;
uint8_t sys_gui_input_handler_pre(uint8_t key, uint32_t arg)
{
	if(sys_gui_debug_mode)
		return sys_gui_debug_input(key, arg);
	if(sys_gui_menu_mode == 2)
	{
		sys_gui_menu_input_handler(key, arg);
		return SYS_HANDLED;
	}
	if(key == 'a')
	{
		if(sys_gui_input_handler_pre_lstarg >= sys_gui_menu_openKeyDelay)
		{
			sys_gui_menu_mode = 1;
			if(arg == KEYBOARD_UP)
			{
				sys_gui_menu_open();
			}
			sys_gui_input_handler_pre_lstarg = arg;
			return SYS_HANDLED;
		}
		sys_gui_input_handler_pre_lstarg = arg;
	}
	return SYS_NOT_HANDLED;
}


void sys_gui_header_draw(void)
{
	xPortGetFreeHeapSize();
	gui_setOrigin(0, 0);
	gui_setFont(&Font_4x6);
	
	
	RTC_TimeTypeDef time;
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	gui_lablef(28, 0, 100, Font_4x6.FontHeight, 0, 0, "R:%lu M:%d", xPortGetFreeHeapSize(), xPortGetMinimumEverFreeHeapSize());
	gui_lablef(104, 0, 40, Font_4x6.FontHeight, 0, 0, "%02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);
	gui_line(0, 6, 128, 6, 1);
	//SSD1306_DrawFilledRectangle(0, 8, 128, 59, 0);
}





uint8_t sys_gui_debug_x = 30, sys_gui_debug_y = 30;
void sys_gui_debug_start(void)
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
