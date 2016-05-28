#ifndef FM_H
#define FM_H

#include "system.h"
#include "vs1053.h"

AppInfo* fm(void);

void fm_open_folder(char* path);

uint8_t fm_start(void);
uint8_t fm_stop(void);

void fm_draw(void);
void fm_update(void);
uint8_t fm_input_handler(int8_t key, uint32_t arg);

uint32_t count_files(char* path);
uint32_t count_folders(char* path);


void _fm_open_folder_free(void);

#endif
