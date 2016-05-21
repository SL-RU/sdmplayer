#include "splayer.h"

AppInfo appInfo_splayer = {
	SPLAYER_ID,
	"splayer",
	&splayer_start,
	&splayer_stop,
	&splayer_draw,
	&splayer_update,
	&splayer_input_handler
};

AppInfo* splayer(void)
{
	return &appInfo_splayer;
}


uint8_t splayer_start(void)
{
	return SYS_OK;
}
uint8_t splayer_stop(void)
{
	return SYS_OK;
}

void splayer_draw(void)
{
	gui_lable("splayer leeee", 10, 10, 100, 20, 0, 1);
}
void splayer_update(void)
{
}
uint8_t splayer_input_handler(int8_t key, uint32_t arg)
{
	return SYS_NOT_HANDLED;
}