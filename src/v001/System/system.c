#include "system.h"

uint8_t sys_Inited = HAL_ERROR;

FATFS FatFs;

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
	
	if(keyboard_init() == SYS_ERROR)
	{
		gui_showMessage("SDMPlayer initing\nSSD1306 - OK\nKeyboard - FAIL");
		gui_draw();
		elog("KEYBOARD NOT INITIALIZED! HALT!");
		SYS_ERROR_HANDLER();
	}
	keyboard_setHandler(&sys_input_handler);
	gui_showMessage("SDMPlayer initing\nSSD1306 - OK\nKeyboard - OK\n...");
	gui_draw();
	
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
	sprintf(msg, "SDMPlayer initing\nSSD1306 - OK\nKeyboard - OK\nFatFS - OK\nSD size: %iMb", sz);
	gui_showMessage(msg);
	gui_draw();
	osDelay(300);
	
	VS1053_Init();
	
	sys_Inited = SYS_OK;
	sys_start_threads();
	app_set(FM_ID);
	slog("System inited");
	
	return 1;
}


uint8_t sys_isInited(void)
{
	return sys_Inited;
}

uint16_t sys_draw_THREAD_delay = 10, 
			 sys_update_THREAD_delay = 100,
		 sys_keyboard_THREAD_delay = 5;

uint8_t sys_start_threads(void)
{
	osThreadDef(sys_draw_THREAD, sys_draw, osPriorityNormal, 0, 1300);
  osThreadCreate(osThread(sys_draw_THREAD), NULL);

	//osThreadDef(sys_update_THREAD, sys_update, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  //osThreadCreate(osThread(sys_update_THREAD), NULL);
	
	osThreadDef(sys_keyboard_THREAD, sys_thread_keyboard, osPriorityNormal, 0, 1900);
  osThreadCreate(osThread(sys_keyboard_THREAD), NULL);
	
	TaskHandle_t xHandle = NULL;
	slog("vs thread: %d",
			xTaskCreate(
                    VS1053_thread,       /* Function that implements the task. */
										"player",          /* Text name for the task. */
                    700,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    osPriorityHigh,/* Priority at which the task is created. */
                    &xHandle ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY );     /* Used to pass out the created task's handle. */
	
	return SYS_OK;
}

void sys_draw(void const * argument)
{
	while(sys_Inited != SYS_OK) osDelay(1);
	slog("draw thread");
  for(;;)
	{
		//pre sys
		gui_setOrigin(0, SYS_GUI_HEADER_HIGHT);
		gui_setFont(&DEFAULT_FONT);
		sys_gui_draw_pre();
		
		//app
		if(sys_gui_menu_mode != 2)
		{
			gui_setOrigin(0, SYS_GUI_HEADER_HIGHT);
			gui_setFont(&DEFAULT_FONT);
			app_draw();
		}
		//post sys
		gui_setOrigin(0, SYS_GUI_HEADER_HIGHT);
		gui_setFont(&DEFAULT_FONT);
		sys_gui_draw_post();
		
		//update screen
		gui_setOrigin(0, SYS_GUI_HEADER_HIGHT);
		gui_setFont(&DEFAULT_FONT);
		gui_draw();
		osDelay(sys_draw_THREAD_delay);
	}
}
uint32_t co = 0;
char bu[30];
void sys_update(void const * argument)
{
	while(sys_isInited() != SYS_OK);
	for(;;)
	{
		app_update();
		osDelay(sys_update_THREAD_delay);
	}
}

void sys_input_handler(int8_t key, uint32_t state)
{
	if(sys_gui_input_handler_pre(key, state) == SYS_HANDLED)
		return;
	if(app_input_handler(key, state) == SYS_HANDLED)
		return;
	if(sys_gui_input_handler_post(key, state) == SYS_HANDLED)
		return;
}


void sys_thread_keyboard(void const * argument)
{
	while(sys_Inited != SYS_OK) osDelay(1);
	slog("Keyboard thread");
	for(;;)
	{
		keyboard_update();
		osDelay(sys_keyboard_THREAD_delay);
	}
}
void sys_thread_player(void const * argument)
{
}
void sys_thread_hw(void const * argument)
{
}

