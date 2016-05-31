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
char** fm_files = 0;     //files names
char** fm_folders = 0;   //folders names
char** fm_visFolders = 0;//folders names for gui

FIL* fm_selectedFIL = 0;
uint32_t fm_selectedFIL_ID = 0;  //file name's id if fm_files

uint8_t fm_proceed = 0;

char fm_lfn[_MAX_LFN + 1];

uint32_t count_files(char* path)
{
	uint32_t count = 0;
	DIR dir;
	FILINFO fno;
	FRESULT res;
	memset(fm_lfn, 0, _MAX_LFN + 1);
	fno.lfname = fm_lfn;
  fno.lfsize = sizeof(fm_lfn);
	
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
	memset(fm_lfn, 0, _MAX_LFN + 1);
	fno.lfname = fm_lfn;
  fno.lfsize = sizeof(fm_lfn);
	
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

char* determine_file_extention(char* path)
{
	if(path == 0)
		return 0;
	uint32_t i = strlen(path) - 1;
	while(path[i] != '.' && i > 0)
		i --;
	if(i == 0) return 0;
	return path + i + 1;
}

void fm_list_focused(uint16_t id, uint32_t arg, uint8_t ev) //FOCUS
{
	if(fm_selectedFIL != 0)
	{
		f_close(fm_selectedFIL);
		vPortFree(fm_selectedFIL);
		fm_selectedFIL = 0;
	}
	if(id >= fm_foldersCount + 1)
	{
		fm_selectedFIL = pvPortMalloc(sizeof(FIL));
		char* pa = pvPortMalloc(sizeof(char)*(strlen(fm_files[arg]) + strlen(fm_curPath) + 2));
		memset(pa, 0, sizeof(char)*(strlen(fm_files[arg]) + strlen(fm_curPath) + 2));
		strcpy(pa, fm_curPath);
		if(fm_curPath[strlen(fm_curPath) - 1] != '/')
			strcat(pa, "/");
		strcat(pa, fm_files[arg]);
		f_open(fm_selectedFIL, pa, FA_OPEN_EXISTING);
		vPortFree(pa);
		fm_selectedFIL_ID = arg;
	}
}

void fm_list_click(uint16_t id, uint32_t arg, uint8_t ev)  //CLICK
{
	slog("ll");
	if(id >= 1 + fm_foldersCount)  //FILES
	{
	  char pa[256] = {0};
	  strcat(pa, fm_curPath);
		if(fm_curPath[strlen(fm_curPath) - 1] != '/')
			strcat(pa, "/");
	  strcat(pa, fm_files[arg]);
		open_file(pa);
		//splayer_playFile(pa);
	}
	else if(id == 0)               //UP
	{
		slog("up %s", fm_curPath);
		if(fm_curPath != 0)
		{
			slog("up");
			if(strlen(fm_curPath) == 0)
			{
				return;
			}
			if(fm_curPath[1] == 0)
			{
				slog("root");
				fm_open_folder("/");
				return;
			}
			slog("%s %d", fm_curPath, strlen(fm_curPath));
			uint32_t i = strlen(fm_curPath) - 1;
			if(fm_curPath[i] == '/')
				i--;
			slog("%d", i);
			while(fm_curPath[i] != '/' && i > 0)
				i --;
			if(i == 0)
			{
				fm_open_folder("/");
				return;
			}
			char* pa = pvPortMalloc(sizeof(char)*(i+2));
			memset(pa, 0, sizeof(char)*(i+2));
			strncpy(pa, fm_curPath, i);
			slog("up: %s", pa);
			fm_open_folder(pa);
			vPortFree(pa);
		}
	}
	else if(id < 1 + fm_foldersCount) //FOLDERS
	{
		slog("op fol %s %s", fm_curPath, fm_folders[arg]);
		char* pa = pvPortMalloc(sizeof(char)*(strlen(fm_folders[arg]) + strlen(fm_curPath) + 3));
		memset(pa, 0, (strlen(fm_folders[arg]) + strlen(fm_curPath) + 3));
		strcat(pa, fm_curPath);
		if(fm_curPath[strlen(fm_curPath) - 1] != '/')
			strcat(pa, "/");
		strcat(pa, fm_folders[arg]);
		slog("fldr: %s", pa);
		fm_open_folder(pa);
		vPortFree(pa);
		slog("fldr: %s - ok", pa);
	}
}


void fm_open_folder(char* path)  //OPEN FOLDER
{
	extlog("openning dir: %s", path);
	if(fm_proceed)
	{
		slog("fm: already somthing is proceeded");
		return;
	}
	taskENTER_CRITICAL();
	fm_proceed = 1;
	_fm_open_folder_free();
	uint32_t i = 0;

	fm_foldersCount = count_folders(path);
	extlog("folders count: %d", fm_foldersCount);
	fm_filesCount = count_files(path);
	extlog("files count: %d", fm_filesCount);
	
	fm_files = pvPortMalloc(sizeof(char*)*fm_filesCount);
	extlog("fm_files - ok");
	fm_folders = pvPortMalloc(sizeof(char*)*fm_foldersCount);
	extlog("fm_folders - ok");
	fm_visFolders  = pvPortMalloc(sizeof(char*)*fm_foldersCount);
	extlog("fm_visFolders - ok");
	
	slog("f: %d, d: %d", fm_filesCount, fm_foldersCount);
	
	uint32_t Ifil = 0, Ifol = 0; //cur file's and folder's id
	DIR dir;
	FILINFO fno;
	FRESULT res;
	memset(fm_lfn, 0, _MAX_LFN + 1);
	fno.lfname = fm_lfn;
  fno.lfsize = sizeof(fm_lfn);
	extlog("start dir read");
	res = f_opendir(&dir, path);                       /* Open the directory */
	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno);                   /* Read a directory item */
      if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
      if (fno.fname[0] == '.'  || fno.fname[0] == 0xFF) continue;             /* Ignore dot entry */
      if ((fno.fattrib & AM_DIR)) {                 /* It is a folder. */
				if(fno.lfname[0] != 0)
				{
					fm_folders[Ifol] = pvPortMalloc(sizeof(char)*(strlen(fno.lfname) + 1));
					strcpy(fm_folders[Ifol], fno.lfname);
				}
				else
				{
					fm_folders[Ifol] = pvPortMalloc(sizeof(char)*(strlen(fno.fname) + 1));
					strcpy(fm_folders[Ifol], fno.fname);
				}
				Ifol ++;
				fno.fname[0] = 0;
			}
			else /* It is a file. */
			{
				if(fno.lfname[0] != 0)
				{
					fm_files[Ifil] = pvPortMalloc(sizeof(char)*(strlen(fno.lfname) + 1));
					strcpy(fm_files[Ifil], fno.lfname);
				}
				else
				{
					fm_files[Ifil] = pvPortMalloc(sizeof(char)*(strlen(fno.fname) + 1));
					strcpy(fm_files[Ifil], fno.fname);
				}
				Ifil ++;
				fno.fname[0] = 0;
			}
    }
    f_closedir(&dir);
	}
	extlog("scanning - OK");
	//set cur path
	if(fm_curPath != 0)
		vPortFree(fm_curPath);
	fm_curPath = pvPortMalloc(sizeof(char)*(strlen(path) + 1));
	memset(fm_curPath, 0, sizeof(char)*(strlen(path) + 1));
	strcpy(fm_curPath, path);
	extlog("curpath set - OK");
	//create list
	uint32_t cnt = fm_filesCount + 1 + fm_foldersCount;
	fm_listData = pvPortMalloc(sizeof(GUI_ListItemData*)*cnt);
	fm_listData[0] = gui_listItem_create("[..]", i, 0, 0, 0);
	for(i = 0; i<fm_foldersCount; i++)
	//add folders
	{
		char* pa = pvPortMalloc(sizeof(char)*(strlen(fm_folders[i]) + 5));
		pa[0] = '[';
		strcpy(pa + 1, fm_folders[i]);
		strcat(pa, "/]");
		//slog("%s --- %s", fm_folders[i], pa);
		fm_listData[i + 1] = gui_listItem_create(pa, i, 0, 0, 0);
		fm_visFolders[i] = pa;
	}
	extlog("add folders to list - OK");
	//add files
	for(i = 0; i<fm_filesCount; i++)
	{
		fm_listData[i + 1 + fm_foldersCount] = gui_listItem_create(fm_files[i], i, 0, 0, 0);
	}
	extlog("add files to list - OK");
	//create list
	fm_list = gui_list_create(0, cnt, fm_listData, 0, 0, 127, 57 - SYS_GUI_HEADER_HIGHT, &fm_list_click, &fm_list_focused, 0);
	taskEXIT_CRITICAL();

	fm_proceed = 0;
	slog("folder %s opened", path);
}
void _fm_open_folder_free(void) //FREE LIST STAFF
{
	extlog("fm - free everything");
	if(fm_selectedFIL != 0)
	{
		extlog("1-1");
		f_close(fm_selectedFIL);
		vPortFree(fm_selectedFIL);
		fm_selectedFIL = 0;
	}
	extlog("2");
	uint32_t i = 0;
	if(fm_folders != 0)
	{
		extlog("2-1");
		for(i =0; i<fm_foldersCount; i++)
		{
			vPortFree(fm_folders[i]);
		}
		extlog("2-2");
		vPortFree(fm_folders);
		fm_folders = 0;
	}
	extlog("2-3");
	if(fm_visFolders != 0)
	{
		extlog("2-4");
		for(i =0; i<fm_foldersCount; i++)
		{
			vPortFree(fm_visFolders[i]);
		}
		extlog("2-5");
		vPortFree(fm_visFolders);
		fm_visFolders = 0;
	}
	extlog("3");
	if(fm_files != 0)
	{
		extlog("3-1");
		for(i =0; i < fm_filesCount; i++)
		{
			vPortFree(fm_files[i]);
		}
		extlog("3-3");
		vPortFree(fm_files);
		fm_files = 0;
	}
	extlog("4");
	if(fm_list != 0)
	{
		extlog("4-4");
		gui_list_remove(fm_list);
		vPortFree(fm_listData);
		fm_list = 0;
	}
	extlog("5");
}

uint8_t fm_start(void) //start
{
	fm_open_folder("/");
	return SYS_OK;
}
uint8_t fm_stop(void) //stop
{
	_fm_open_folder_free();
	return SYS_OK;
}

void fm_draw(void) //draw
{
	if(fm_proceed)
	{
		gui_rect_fill((HAL_GetTick()/10)%135, 0, 5, 64, 1, 0);
		return;
	}
	//BOTTOM
	if(fm_list != 0)
	{
		gui_list_draw(fm_list);
		gui_setFont(&Font_4x6);
		 //FILE SIZE
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
		//FILE's EXTENTION
		uint8_t cpd = 0;
		if(fm_selectedFIL != 0)
		{
			char* ext = determine_file_extention(fm_files[fm_selectedFIL_ID]);
			if(ext != 0)
			{
				if(strlen(ext) < 4)
				{
					char e[5] = {0};
					
					cpd = strlen(ext) * 4 + 2;
					strcpy(e, ext);
					stoupper(e);
					gui_lable(e, 99 - cpd, 58 - SYS_GUI_HEADER_HIGHT, cpd, 6, 0, 0);
					cpd ++;
					gui_line(98 - cpd, 57 - SYS_GUI_HEADER_HIGHT, 98 - cpd, 64 - SYS_GUI_HEADER_HIGHT, 1);
				}
			}
		}
		//CURRENT PATH
		if(fm_curPath!=0)
		{
			gui_lable(fm_curPath, 26, 57 - SYS_GUI_HEADER_HIGHT, 128 - cpd - 29 - 28, 6, 1, 0);
		}
		//selected id, total count
		gui_lablef(100, 58 - SYS_GUI_HEADER_HIGHT, 30, 6, 0, 0, "% 3d/%d", fm_list->selectedItem + 1, fm_list->ItemsCount);
		gui_line(99, 57 - SYS_GUI_HEADER_HIGHT, 99, 64 - SYS_GUI_HEADER_HIGHT, 1);
	}
}
void fm_update(void) //update
{
}
uint8_t fm_input_handler(int8_t key, uint32_t arg) //input
{
	if(fm_proceed)
	{
		return SYS_HANDLED;
	}
	if(fm_list != 0 && arg == KEYBOARD_UP)
	{
		return gui_list_input(fm_list, key);
	}
	return SYS_NOT_HANDLED;
}


void stoupper(char *s) //let it be here just for a while
{
    for(; *s; s++)
        if(('a' <= *s) && (*s <= 'z'))
            *s = 'A' + (*s - 'a');
}