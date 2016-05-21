#ifndef FM_H
#define FM_H

#include "system.h"
#include "vs1053.h"

AppInfo* fm(void);


uint8_t fm_start(void);
uint8_t fm_stop(void);

void fm_draw(void);
void fm_update(void);
uint8_t fm_input_handler(int8_t key, uint32_t arg);


#endif
