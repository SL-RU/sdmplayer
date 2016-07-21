#include "app.h"

AppInfo *app_curr;


AppInfo* app_getByID(uint8_t id)
{
	switch(id)
	{
		case APP_TEST_ID: return app_test();
		case FM_ID      : return fm();
		case SPLAYER_ID : return splayer();
		case FVIEW_ID   : return file_viewer();
		default: return 0;
	}
}
uint8_t app_getCur(void)
{
	if(app_curr != 0)
		return app_curr->id;
	return 0;
}
void app_set(uint8_t id)
{
	gui_closeMessage();
	if(app_curr != 0)
	{
		if(app_curr->stop() == SYS_OK)
			slog("stop app id:%d - OK", app_curr->id);
		else
		{
			slog("stop app id:%d - ERR", app_curr->id);
			app_curr = 0;
			return;
		}
		app_curr = 0;
	}
	
	AppInfo *app = app_getByID(id);
	if(app == 0)
	{
		elog("App get by id error!", id);
		return;
	}
	slog("Starting app id:%d", id);
	if(app->start() != SYS_OK)
	{
		elog("App id:%d name:%s start - ERR!", id, app->name);
		return;
	}
	slog("Starting app id:%d - OK", id);
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
