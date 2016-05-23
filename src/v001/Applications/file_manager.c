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


char* fm_curPath;
uint32_t fm_filesCount = 0;
uint32_t fm_foldersCount = 0;
GUI_ListData* fm_list = 0;
GUI_ListItemData **fm_listData = 0;
char** fm_files = 0;
char** fm_folders = 0;
FIL* fm_selectedFIL = 0;

uint32_t count_files(char* path)
{
	uint32_t count = 0;
	DIR dir;
	FILINFO fno;
	FRESULT res;
	static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
	fno.lfname = lfn;
  fno.lfsize = sizeof(lfn);
	
	res = f_opendir(&dir, path);                       /* Open the directory */
	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
      if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
      if (fno.fname[0] == '.' || fno.fname[0] == 0xFF) continue;             /* Ignore dot entry */
      if (!(fno.fattrib & AM_DIR)) {                 /* It is a file. */
				count ++;
				fno.fname[0] = 0;
      }
    }
    f_closedir(&dir);
	}
	return count;
}
uint32_t count_folders(char* path)
{
	uint32_t count = 0;
	DIR dir;
	FILINFO fno;
	FRESULT res;
	static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
	fno.lfname = lfn;
  fno.lfsize = sizeof(lfn);
	
	res = f_opendir(&dir, path);                       /* Open the directory */
	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
      if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
      if (fno.fname[0] == '.'  || fno.fname[0] == 0xFF) continue;             /* Ignore dot entry */
      if ((fno.fattrib & AM_DIR)) {                 /* It is a folder. */
				count ++;
				fno.fname[0] = 0;
			}
    }
    f_closedir(&dir);
	}
	return count;
}

void fm_list_focused(uint16_t id, uint32_t arg, uint8_t ev)
{
	if(fm_selectedFIL != 0)
	{
		f_close(fm_selectedFIL);
	}
	else
	{
		fm_selectedFIL = pvPortMalloc(sizeof(FIL));
	}
	char pa[256] = {0};
	strcat(pa, "/leee/");
	strcat(pa, fm_files[arg]);
	f_open(fm_selectedFIL, pa, FA_OPEN_EXISTING);
}
FIL flll;
void pppp(void * argument)
{
	slog("p");
	VS1053_play_file(&flll, "");
}
TaskHandle_t xHandle = NULL;
uint8_t p = 0;
void fm_list_click(uint16_t id, uint32_t arg, uint8_t ev)
{
	if(p==0)
	{
		p = 1;
			char pa[256] = {0};
	strcat(pa, "/leee/");
	strcat(pa, fm_files[fm_list->selectedItem]);
		uint8_t res = f_open(&flll, pa, FA_OPEN_EXISTING | FA_READ);
			slog("file open: %d......%d", res,
			xTaskCreate(
                    pppp,       /* Function that implements the task. */
										"player",          /* Text name for the task. */
                    1000,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    osPriorityHigh,/* Priority at which the task is created. */
                    &xHandle ) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY );     /* Used to pass out the created task's handle. */
			
	}
}


void fm_open_folder(char* path)
{
	if(fm_selectedFIL != 0)
	{
		f_close(fm_selectedFIL);
		vPortFree(fm_selectedFIL);
		fm_selectedFIL = 0;
	}
	uint32_t i = 0;
	if(fm_folders != 0)
	{
		for(i =0; i<fm_foldersCount; i++)
		{
			vPortFree(fm_folders[i]);
		}
		for(i =0; i<fm_filesCount; i++)
		{
			vPortFree(fm_files[i]);
		}
		gui_list_remove(fm_list);
		vPortFree(fm_files);
		vPortFree(fm_folders);
		vPortFree(fm_listData);
		fm_folders = 0;
	}
	fm_foldersCount = count_folders(path);
	fm_filesCount = count_files(path);
	
	fm_files = pvPortMalloc(sizeof(char*)*fm_filesCount);
	fm_folders = pvPortMalloc(sizeof(char*)*fm_foldersCount);
	
	slog("f: %d, d: %d", fm_filesCount, fm_foldersCount);
	
	uint32_t Ifil = 0, Ifol = 0; //cur file's and folder's id
	DIR dir;
	FILINFO fno;
	FRESULT res;
	static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
	fno.lfname = lfn;
  fno.lfsize = sizeof(lfn);
	
	res = f_opendir(&dir, path);                       /* Open the directory */
	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
      if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
      if (fno.fname[0] == '.'  || fno.fname[0] == 0xFF) continue;             /* Ignore dot entry */
      if ((fno.fattrib & AM_DIR)) {                 /* It is a folder. */
				fm_folders[Ifol] = pvPortMalloc(sizeof(char)*(strlen(fno.lfname) + 1));
				strcpy(fm_folders[Ifol], fno.lfname);
				Ifol ++;
				fno.fname[0] = 0;
			}
			else /* It is a file. */
			{
				fm_files[Ifil] = pvPortMalloc(sizeof(char)*(strlen(fno.lfname) + 1));
				strcpy(fm_files[Ifil], fno.lfname);
				Ifil ++;
				fno.fname[0] = 0;
			}
    }
    f_closedir(&dir);
	}
	
	uint32_t cnt = fm_filesCount;
	fm_listData = pvPortMalloc(sizeof(GUI_ListItemData*)*cnt);
	for(i = 0; i<cnt; i++)
	{
		fm_listData[i] = gui_listItem_create(fm_files[i], i, 0, 0, 0);
	}
	fm_list = gui_list_create(0, cnt, fm_listData, 0, 0, 127, 57 - SYS_GUI_HEADER_HIGHT, &fm_list_click, &fm_list_focused, 0);
}
void _fm_open_folder_free()
{
	if(fm_selectedFIL != 0)
	{
		f_close(fm_selectedFIL);
		vPortFree(fm_selectedFIL);
		fm_selectedFIL = 0;
	}
	uint32_t i = 0;
	if(fm_folders != 0)
	{
		for(i =0; i<fm_foldersCount; i++)
		{
			vPortFree(fm_folders[i]);
		}
		vPortFree(fm_folders);
		fm_folders = 0;
	}
	if(fm_files != 0)
	{
		for(i =0; i<fm_filesCount; i++)
		{
			vPortFree(fm_files[i]);
		}
		vPortFree(fm_files);
		fm_files = 0;
	}
	if(fm_list != 0)
	{
		gui_list_remove(fm_list);
		vPortFree(fm_listData);
	}
}

uint8_t fm_start(void)
{
	fm_open_folder("/leee");
	return SYS_OK;
}
uint8_t fm_stop(void)
{
	_fm_open_folder_free();
	return SYS_OK;
}

void fm_draw(void)
{
	if(fm_list != 0)
	{
		gui_list_draw(fm_list);
		gui_setFont(&Font_4x6);
		if(fm_selectedFIL != 0)
		{
			if(fm_selectedFIL->fsize < 10000)
				gui_lablef(0, 58 - SYS_GUI_HEADER_HIGHT, 26, 6, 0, 0, "%db", fm_selectedFIL->fsize);
			else if(fm_selectedFIL->fsize/1024 < 10000)
				gui_lablef(0, 58 - SYS_GUI_HEADER_HIGHT, 26, 6, 0, 0, "%dkb", fm_selectedFIL->fsize/1024);
			else if(fm_selectedFIL->fsize/(1024*1024) < 10000)
				gui_lablef(0, 58 - SYS_GUI_HEADER_HIGHT, 26, 6, 0, 0, "%dMb", fm_selectedFIL->fsize/(1024*1024));
			else
				gui_lablef(0, 58 - SYS_GUI_HEADER_HIGHT, 26, 6, 0, 0, "%dGb", fm_selectedFIL->fsize/(1024*1024*1024));
			
			gui_line(25, 57 - SYS_GUI_HEADER_HIGHT, 25, 64 - SYS_GUI_HEADER_HIGHT, 1);
		}
		else
		{
			gui_lablef(0, 58 - SYS_GUI_HEADER_HIGHT, 26, 6, 0, 0, "size", fm_selectedFIL->fsize);
			gui_line(25, 57 - SYS_GUI_HEADER_HIGHT, 25, 64 - SYS_GUI_HEADER_HIGHT, 1);
		}
	}
}
void fm_update(void)
{
}
uint8_t fm_input_handler(int8_t key, uint32_t arg)
{
	if(fm_list != 0 && arg == KEYBOARD_UP)
	{
		return gui_list_input(fm_list, key);
	}
	return SYS_NOT_HANDLED;
}
