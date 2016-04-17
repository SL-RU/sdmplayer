#include "fm.h"

FATFS FatFs;
uint8_t fm_inited = 0;
char *txt;
GUI_ListData *fm_flist;
GUI_ListItemData **fm_curfiles;
char file_buff[100];

uint8_t *img_buf;
uint32_t img_start_tick;
FIL img_fil;

void benchmarkRW_FS()
{
	char *nm = "test.mp3", *om = "test_out.mp3";
	FIL fl;
	UINT res = f_open(&fl, "test.mp3", FA_READ | FA_OPEN_EXISTING);
	printf("res r %d\n", res);

	uint32_t wr = 0;
	uint8_t buf[32] = {0};
	uint32_t sms = HAL_GetTick(), count = 0;
	do
	{
		f_read(&fl, &buf, 32, &wr);
		count += wr;
	}	while(wr == 32);
	f_close(&fl);
	uint32_t ems = HAL_GetTick();
	printf("bench read: %dms, %d bytes\n", ems - sms, count);
}

void benchmarkR_FS()
{
	char *nm = "test.mp3", *om = "test_out.mp3";
	FIL fl, wl;
	UINT res = f_open(&fl, "test.mp3", FA_READ | FA_OPEN_EXISTING);
	printf("res r %d\n", res);
	res = f_open(&wl, "test_out.mp3", FA_WRITE | FA_CREATE_ALWAYS);
	printf("res w %d\n", res);

	uint32_t wr = 0;
	uint8_t buf[32] = {0};
	uint32_t sms = HAL_GetTick(), count = 0;
	do
	{
		f_read(&fl, &buf, 32, &wr);
		f_write(&wl, &buf, wr, &wr);
		count += wr;
	}	while(wr == 32);
	f_close(&fl);
	f_close(&wl);
	uint32_t ems = HAL_GetTick();
	printf("bench read-write: %dms, %d bytes\n", ems - sms, count);
}

void fm_init()
{
	if (f_mount(&FatFs, "", 1) == FR_OK) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
		fm_inited = 1;

	}
	else
	{
		fm_inited = 0;
		return;
	}
	create_files_list();
	//f_mount(0, "", 1);
}

void fm_draw()
{
	if(!fm_inited)
	{
		gui_lable("SD card don't inited", 1, 1, 125, 20, 0, 1);
	}
	else
	{
		gui_lable("SD card OK", 0, 0, 128, 20, 0, 1);
		gui_lable_multiline(txt, 1, 21, 125, 42, 0, 1);
	}
}

uint8_t fm_input(uint8_t key)
{

}

uint32_t count_files(char *path)
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
      if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
      if (!(fno.fattrib & AM_DIR)) {                 /* It is a file. */
				count ++;
      }
    }
    f_closedir(&dir);
	}
	return count;
}

void fm_draw_img()
{
	uint8_t fr = ((HAL_GetTick() - img_start_tick) / 100) % img_buf[2];
	ssd1306_image(img_buf, fr, 0,0);
}
uint8_t fm_input_img(uint8_t key)
{
	gui_closeMessage();
	free(img_buf);
	f_close(&img_fil);
	return 1;
}

void fm_handle_file_open(uint16_t id, uint32_t arg, uint8_t act)
{
	char ext[4] = {'\0'};
	uint16_t i = 0;
	FIL fil;
	UINT rd = 0;
	while(fm_curfiles[id]->text[i] != '\0')
	{
		ext[0] = ext[1];
		ext[1] = ext[2];
		ext[2] = fm_curfiles[id]->text[i];
		i++;
	}
	printf("Ext: %s\n", ext);
	if(strcmp(ext, "img") == 0)
	{
		printf("image\n");
		uint8_t sz[5];
		f_open(&img_fil, fm_curfiles[id]->text, FA_OPEN_EXISTING | FA_READ);
		f_read(&img_fil, sz, 5, &rd);
		if(rd != 5)
			return;

		img_buf = malloc((sz[2] * (sz[3] + (sz[4] << 8)) + 5)*sizeof(uint8_t));
		printf("Frame sz: %d\n", sz[2] * (sz[3] + (sz[4] << 8)));
		f_read(&img_fil, img_buf + 5, sz[2] * (sz[3] + (sz[4] << 8)), &rd);
		for(i = 0; i < 5; i++)
			img_buf[i] = sz[i];
		
		img_start_tick = HAL_GetTick();
		gui_showCustomMessage(&fm_draw_img, &fm_input_img);
		return;
	}
	
	if(strcmp(ext, "mp3") == 0)
	{
		printf("music\n");
		f_open(&fil, fm_curfiles[id]->text, FA_OPEN_EXISTING | FA_READ);
		gui_showCustomMessage(&VS1053_draw, &VS1053_input);
		VS1053_play(&fil, fm_curfiles[id]->text);
		f_close(&fil);
		return;
	}
	
	memset(file_buff, '\0', sizeof(file_buff));
	f_open(&fil, fm_curfiles[id]->text, FA_OPEN_EXISTING | FA_READ);
	f_read(&fil, file_buff, 99, &rd); 
	gui_showMessage(file_buff);
	f_close(&fil);
}

void create_files_list()
{	
	printf("cre lst\n");
	char path[] = "/";
	uint32_t count = count_files(path);
	printf("count: %d\n", count);
	if(count == 0)
		return;
	
	uint32_t ci = 0;
	fm_curfiles = malloc(count*sizeof(GUI_ListItemData));

	FRESULT res;
  FILINFO fno;
  DIR dir;
  int i;
  char *fn;   /* This function assumes non-Unicode configuration */
  static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
  fno.lfname = lfn;
  fno.lfsize = sizeof(lfn);
	printf("cre lst\n");
	res = f_opendir(&dir, path);											 /* Open the directory */
	if (res == FR_OK) {
		i = strlen(path);
		for (;;) {
			res = f_readdir(&dir, &fno);									 /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0) break;	/* Break on error or end of dir */
			if (fno.fname[0] == '.') continue;						 /* Ignore dot entry */
			fn = *fno.lfname ? fno.lfname : fno.fname;
			if (fno.fattrib & AM_DIR) {										/* It is a directory */
				//sprintf(&path[i], "/%s", fn);
				//path[i] = 0;
				printf("fldr\n");
				if (res != FR_OK) break;
			} else {																			 /* It is a file. */
				char *bbb = malloc((strlen(fn) + 1) * sizeof(char));
				fm_curfiles[ci] = gui_create_listItem(bbb, 0, 0, 0, 0);
				strcpy(bbb, fn);
				printf("kek %s\n", fn);
				ci++;
			}
		}
		f_closedir(&dir);
	}
	

	fm_flist = gui_create_list("sl", count, fm_curfiles, 0, 0, 128, 64, &fm_handle_file_open, 0, 0);
	gui_set_curList(fm_flist);
}
