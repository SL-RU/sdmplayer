#ifndef PLAYER_H
#define PLAYER_H

#include "system.h"
#include "vs1053.h"

AppInfo* splayer(void);


uint8_t splayer_start(void);
uint8_t splayer_stop(void);

void splayer_draw(void);
void splayer_update(void);
uint8_t splayer_input_handler(int8_t key, uint32_t arg);


#endif
