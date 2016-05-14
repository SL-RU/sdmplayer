#include "app_test.h"
#include "rtc.h"

AppInfo appInfo_test = {
	APP_TEST_ID,
	&app_test_start,
	&app_test_stop,
	&app_test_draw,
	&app_test_update,
	&app_test_input_handler
};

AppInfo* app_test(void)
{
	slog("test app");
	return &appInfo_test;
	
}

GUI_ListData* list;

uint8_t app_test_start(void)
{
	
	slog("test started");
	return SYS_OK;
}
uint8_t app_test_stop(void)
{
	return SYS_OK;
}

uint8_t app_test_focus = 0;
uint8_t app_test_focusMAX = 6;
uint32_t app_test_num = 4583;

void app_test_draw(void)
{
	uint8_t i = 0;
	for(i = 0; i<=6; i++)
	{
		gui_DigitInput_draw(app_test_num, i, app_test_focus == 6 - i, 80 - 10 * i, 5, 0, 0);
	}
	
	gui_lablef(10, 20, 100, 13, 0, 1, "n: %d", app_test_num);
	
	return;
	
	RTC_TimeTypeDef time;
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);

}
uint8_t ineeeee = 0;
void app_test_update(void)
{
	if(!ineeeee)
	{
		ineeeee = 1;
		VS1053_Init();
	}
}
FIL fl;

void player(void const * argument)
{
	VS1053_play(&fl, "Shockwave.mp3");
}
uint8_t p = 0;
uint8_t app_test_input_handler(int8_t key, uint32_t arg)
{
	if(arg == KEYBOARD_UP)
	{
		if(key == 'a')
		{
			if(app_test_focus == 0)
				app_test_focus = app_test_focusMAX;
			else
				app_test_focus --;
			return SYS_HANDLED;
		}
		if(key == 'c')
		{
			if(app_test_focus >= app_test_focusMAX)
				app_test_focus = 0;
			else
				app_test_focus ++;
			return SYS_HANDLED;
		}
		if(app_test_focus <= 6)
		{
			if (gui_DigitInput_input((uint32_t*)&app_test_num, 6 - app_test_focus, key, arg))
			{
				app_test_focus ++;
				if(app_test_focus > app_test_focusMAX)
					app_test_focus = 0;
			}
		}
		return SYS_HANDLED;
	}
	return SYS_NOT_HANDLED;
}

