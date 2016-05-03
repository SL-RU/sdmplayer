#include "app_test.h"


AppInfo appInfo_test = {
	1,
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

uint8_t app_test_start(void)
{
	gui_closeMessage();
	slog("test started");
	return SYS_OK;
}
uint8_t app_test_stop(void)
{
}

void app_test_draw(void)
{
	gui_lable("LEL", 10, 30, 100, 50, 1, 1);
}
void app_test_update(void)
{
}
void app_test_input_handler(uint8_t key, uint32_t arg)
{
}

