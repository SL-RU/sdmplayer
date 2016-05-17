#include "app.h"

AppInfo *app_curr;


void app_set(uint8_t id)
{
	gui_closeMessage();
	if(app_curr != 0)
	{
		app_curr->stop();
		app_curr = 0;
	}
	
	AppInfo *app = app_test();
	if(app == 0)
	{
		return;
	}
	if(app->start() != SYS_OK)
		return;
	
	app_curr = app;
}

void app_update(void)
{
	if(app_curr != 0)
		app_curr->update();

}
void app_draw(void)
{
	if(app_curr != 0)
		app_curr->draw();

}
uint8_t app_input_handler(int8_t key, uint32_t arg)
{
	if(app_curr != 0)
		return app_curr->input_handler(key, arg);
	return SYS_NOT_HANDLED;
}
