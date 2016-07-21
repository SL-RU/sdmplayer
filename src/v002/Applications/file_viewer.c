#include "file_viewer.h"

AppInfo appInfo_fview = {
	FVIEW_ID,
	"fview",
	&file_viewer_start,
	&file_viewer_stop,
	&file_viewer_draw,
	&file_viewer_update,
	&file_viewer_input_handler
};

AppInfo* file_viewer(void){return &appInfo_fview; }


FIL* fview_curFile = 0;
char* fview_curPath = 0;
uint8_t fview_opened = 0;
uint32_t fview_openTime = 0;

#define EXT_PLAIN 0
#define EXT_MUS   1
#define EXT_IMG   2

uint8_t fview_extention = 0;

char* fview_plainBuffer = 0;
uint8_t *fview_imgBuffer = 0;

uint8_t open_file(char* file)
{
	if(file == 0)
		return SYS_ERROR;
	if(strlen(file) == 0)
		return SYS_ERROR;
	
	slog("openning file: %s", file);
	char *ext = determine_file_extention(file);
	char *e = pvPortMalloc(sizeof(file)*(strlen(ext) + 1));
	strcpy(e, ext);
	e[strlen(ext)] = 0;
	stoupper(e);
	extlog("ext: %s", e);
	if(!strncmp(e, "MP3", strlen(ext)) || !strncmp(e, "WAV", strlen(ext)) )
	{
		slog("music player");
		splayer_playFile(file);
	}
	else
	{
		taskENTER_CRITICAL();
		fview_opened = 0;
		fview_curPath = pvPortMalloc(sizeof(char)*(strlen(file) + 1));
		fview_curPath[strlen(file)] = 0;
		strcpy(fview_curPath, file);
		taskEXIT_CRITICAL();
		
		app_set(FVIEW_ID);
		
		taskENTER_CRITICAL();
		extlog("self %s ", fview_curPath);
		fview_curFile = pvPortMalloc(sizeof(FIL));
		memset(fview_curFile, 0, sizeof(FIL));
		taskENTER_CRITICAL();
		uint8_t res = 0;
		res = f_open(fview_curFile, fview_curPath, FA_OPEN_EXISTING | FA_READ);
		extlog("op %d", res);
		
		uint32_t rd = 0;
		uint32_t i = 0;
		if(!strncmp(e, "IMG", strlen(ext)))
		{
			slog("image");
			fview_extention = EXT_IMG;
			
			uint8_t sz[5];
			
			f_read(fview_curFile, sz, 5, &rd); //frame size and frame count
			if(rd != 5)
			{
				vPortFree(e);	
				return SYS_ERROR;
			}
			
			fview_imgBuffer = pvPortMalloc((sz[2] * (sz[3] + (sz[4] << 8)) + 5)*sizeof(uint8_t));
			extlog("Frame sz: %d", sz[2] * (sz[3] + (sz[4] << 8)));
			taskEXIT_CRITICAL();
			res = f_read(fview_curFile, fview_imgBuffer + 5, sz[2] * (sz[3] + (sz[4] << 8)), &rd);
			taskENTER_CRITICAL();
			extlog("re %d %d", res, rd);
			for(i = 0; i < 5; i++)
				fview_imgBuffer[i] = sz[i];
			taskEXIT_CRITICAL();
		}
		else
		{
			taskENTER_CRITICAL();
			extlog("plain");
			fview_plainBuffer = pvPortMalloc((100)*sizeof(char));
			
			memset(fview_plainBuffer, 0, (100)*sizeof(char));
			taskEXIT_CRITICAL();
			res = f_read(fview_curFile, fview_plainBuffer, 97, &rd);
			extlog("read %d %d", res, rd);
			taskENTER_CRITICAL();
			fview_plainBuffer[rd] = 0;
			//extlog(fview_plainBuffer);
			fview_extention = EXT_PLAIN;
			taskEXIT_CRITICAL();
		}
		fview_opened = 1;
	}
	vPortFree(e);	
	slog("done");

	return SYS_OK;
}

uint8_t file_viewer_start(void)
{
	fview_openTime = HAL_GetTick();
	
	return SYS_OK;
}
uint8_t file_viewer_stop(void)
{
	if(fview_curFile != 0)
	{
		vPortFree(fview_curFile);
		fview_curFile = 0;
	}
	if(fview_curPath != 0)
	{
		vPortFree(fview_curPath);
		fview_curPath = 0;
	}
	if(fview_plainBuffer != 0)
	{
		vPortFree(fview_plainBuffer);
		fview_plainBuffer = 0;
	}
	if(fview_imgBuffer != 0)
	{
		vPortFree(fview_imgBuffer);
		fview_imgBuffer = 0;
	}
	return SYS_OK;
}

void file_viewer_draw(void)
{
	if(!fview_opened)
		return;
	uint8_t fr;
	taskENTER_CRITICAL();
	extlog("d");
	switch(fview_extention)
	{
		case EXT_PLAIN: 
			gui_lable_multiline(fview_plainBuffer, 0, 0, 128, 64 - SYS_GUI_HEADER_HIGHT, 0, 0); 
			break;
		case EXT_IMG:
			fr = ((HAL_GetTick() - fview_openTime) / 100) % fview_imgBuffer[2];
			ssd1306_image(fview_imgBuffer, fr, 0, SYS_GUI_HEADER_HIGHT);
			break;
	}
	taskEXIT_CRITICAL();
}
void file_viewer_update(void)
{
}
uint8_t file_viewer_input_handler(int8_t key, uint32_t arg)
{
	if(!fview_opened)
		return SYS_HANDLED;
	if(arg == KEYBOARD_UP && key != 'a')
	{
		app_set(FM_ID);
	}
	return SYS_NOT_HANDLED;
}
