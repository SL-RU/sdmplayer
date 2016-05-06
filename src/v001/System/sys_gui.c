#include "sys_gui.h"




void sys_draw_gui(void)
{
	gui_setFont(&DEFAULT_FONT);
	sys_draw_gui_header();
	gui_setFont(&DEFAULT_FONT);
}
void sys_update_gui(void)
{
}
uint8_t sys_input_handler_gui(uint8_t key, uint32_t arg)
{
}


void sys_draw_gui_header(void)
{
	gui_setFont(&Font_4x6);
	RTC_TimeTypeDef time;
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	char bu[5] = {0};
	sprintf(bu, "%02d:%02d", time.Hours, time.Minutes);
	gui_lable(bu, 107, 0, 128, Font_4x6.FontHeight, 0, 0);
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
}
