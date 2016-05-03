#include "system.h"

uint8_t sys_Inited = HAL_ERROR;

FATFS FatFs;

AppInfo *sys_curr_app;

uint8_t sys_init(void)
{
	slog("");
	slog("SDMPlayer by Alexander Lutsai");
	slog("System initing");
	if(gui_init() == SYS_ERROR)
	{	
		elog("DISPLAY NOT INITIALIZED! HALT!");
		SYS_ERROR_HANDLER();
	}
	
	gui_showMessage("SDMPlayer initing\nSSD1306 - OK\n...");
	gui_draw();
	HAL_Delay(100);
	
	if(keyboard_init() == SYS_ERROR)
	{
		gui_showMessage("System initing...\nSSD1306 - OK\nKeyboard - FAIL");
		gui_draw();
		elog("KEYBOARD NOT INITIALIZED! HALT!");
		SYS_ERROR_HANDLER();
	}
	gui_showMessage("System initing...\nSSD1306 - OK\nKeyboard - OK\n...");
	gui_draw();
	HAL_Delay(100);
	
	uint8_t res = 0;
	res = f_mount(&FatFs, "", 1);
	if(res != FR_OK)
	{
		elog("FatFS error %d", res);
	}
	slog("FatFS - OK");
	uint16_t sz = hw_sdcard_capacity();
	slog("Size: %iMb", sz);
	char msg[100] = {0};
	sprintf(msg, "System initing...\nSSD1306 - OK\nKeyboard - OK\nFatFS - OK\nSD size: %iMb", sz);
	gui_showMessage(msg);
	gui_draw();
	HAL_Delay(3000);
	
	
	sys_start_threads();
	sys_Inited = SYS_OK;
	slog("System inited");
	
	sys_start_app_id(0);
	
	return 1;
}

uint8_t sys_isInited(void)
{
	return sys_Inited;
}

uint8_t sys_start_threads(void)
{
	osThreadDef(sys_draw_THREAD, sys_draw, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(sys_draw_THREAD), NULL);

	osThreadDef(sys_update_THREAD, sys_update, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(sys_update_THREAD), NULL);
}

void sys_draw(void const * argument)
{
  for(;;)
	{
		sys_draw_app();
		sys_draw_gui();
		gui_draw();
		osDelay(10);
	}
}
uint32_t co = 0;
char bu[30];
void sys_update(void const * argument)
{
	for(;;)
	{
		sys_update_app();
		osDelay(100);
	}
}

#include "app_test.h"

void sys_start_app_id(uint8_t id)
{
	if(sys_curr_app != 0)
		sys_curr_app = 0;
	
	AppInfo *app = app_test();
	if(app == 0)
	{
		return;
	}
	if(app->start() != SYS_OK)
		return;
	
	sys_curr_app = app;
}

void sys_update_app(void)
{
	if(sys_curr_app != 0)
		sys_curr_app->update();
}
void sys_draw_app(void)
{
	if(sys_curr_app != 0)
		sys_curr_app->draw();
}
uint8_t sys_input_handler_app(uint8_t key, uint32_t arg)
{
	if(sys_curr_app != 0)
		sys_curr_app->input_handler(key, arg);
}