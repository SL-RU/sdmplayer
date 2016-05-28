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

PlayerInfo pInfo_splayer = {
	SPLAYER_ID,
	"spl",
	&splayer_pstart,
	&splayer_pstop,
	&splayer_pcomplete,
	&splayer_pdraw,
	&splayer_pinput
};

FIL* splayer_curFile;
char* splayer_curFolPath = 0;
char*	splayer_curFilPath = 0;

AppInfo* splayer(void)
{
	return &appInfo_splayer;
}
PlayerInfo* splayer_p(void)
{
	return &pInfo_splayer;
}

void splayer_playFile(char* path)
{
	VS1053_set_player(splayer_p());
	uint8_t res = f_open(splayer_curFile, path, FA_OPEN_EXISTING | FA_READ);
	slog("file: %s  ...# %d", path, res);
	VS1053_play_file(splayer_curFile);
	if(splayer_curFilPath != 0)
		vPortFree(splayer_curFilPath);
	splayer_curFilPath = pvPortMalloc(sizeof(char)*(strlen(path) + 1));
	strcpy(splayer_curFilPath, path);
}

uint8_t splayer_start(void)
{
	VS1053_set_player(&pInfo_splayer);
	return SYS_OK;
}
uint8_t splayer_stop(void)
{
	return SYS_OK;
}
//PLAYER
uint8_t splayer_pstart(void)
{
	splayer_curFile = pvPortMalloc(sizeof(FIL));
	return SYS_OK;
}
uint8_t splayer_pstop(void)
{
	vPortFree(splayer_curFile);
	if(splayer_curFilPath != 0)
		vPortFree(splayer_curFilPath);
	if(splayer_curFolPath != 0)
		vPortFree(splayer_curFolPath);
	return SYS_OK;
}
void splayer_pcomplete(void)
{
}
void splayer_pdraw(void)
{
	if(splayer_curFilPath != 0)
	{
		char* nm = splayer_curFilPath;
		uint16_t i = strlen(splayer_curFilPath) - 1;
		while(splayer_curFilPath[i] != '/')
		{
			i--;
		}
		nm += i + 1;
		//gui_setFont(&Font_4x6);
		gui_lable(nm, 0, 0, 128 - 32, 12, 0, 1);
	}
	gui_lablef(0, 25, 128, 12, 0, 0, "time: %d", parametric.positionMsec/1000);
	switch(VS1053_getState())
	{
		case PLAYER_STOP:
			gui_rect(14, 14, 10, 10, 1);
			break;
		case PLAYER_PLAY:
			gui_circle(19, 19, 5, 1);
			break;
		case PLAYER_PAUSE:
			gui_rect(14, 14, 4, 10, 1);
			gui_rect(20, 14, 4, 10, 1);
			break;
	}
}
uint8_t splayer_pinput(int8_t key, uint32_t arg)
{
	if(key == 5 && arg == KEYBOARD_UP)
	{
		if(VS1053_getState() == PLAYER_PLAY)
			VS1053_pause();
		else if (VS1053_getState() == PLAYER_PAUSE)
		{
			VS1053_unpause();
		}
		return SYS_HANDLED;
	}
	return SYS_NOT_HANDLED;
}

//APP
void splayer_draw(void)
{
	splayer_pdraw();
}
void splayer_update(void)
{
}
uint8_t splayer_input_handler(int8_t key, uint32_t arg)
{
	if(key == 5 && arg == KEYBOARD_UP)
	{
		if(VS1053_getState() == PLAYER_PLAY)
			VS1053_pause();
		else if (VS1053_getState() == PLAYER_PAUSE)
		{
			VS1053_unpause();
		}
		return SYS_HANDLED;
	}
	return SYS_NOT_HANDLED;
}
