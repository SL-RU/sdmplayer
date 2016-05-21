#include "file_manager.h"


AppInfo appInfo_fm = {
	FM_ID,
	"fm",
	&fm_start,
	&fm_stop,
	&fm_draw,
	&fm_update,
	&fm_input_handler
};

AppInfo* fm(void)
{
	return &appInfo_fm;
}


uint8_t fm_start(void)
{
	return SYS_OK;
}
uint8_t fm_stop(void)
{
	return SYS_OK;
}

void fm_draw(void)
{
	gui_lable("file manager", 10, 10, 100, 20, 0, 1);
}
void fm_update(void)
{
}
uint8_t fm_input_handler(int8_t key, uint32_t arg)
{
	return SYS_NOT_HANDLED;
}
