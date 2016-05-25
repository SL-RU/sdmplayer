#ifndef SPLAYER_H
#define SPLAYER_H 1

#include "system.h"
#include "vs1053.h"

AppInfo* splayer(void);
PlayerInfo* splayer_p(void);

uint8_t splayer_start(void);
uint8_t splayer_stop(void);

uint8_t splayer_pstart(void);
uint8_t splayer_pstop(void);
void splayer_pcomplete(void);


void splayer_pdraw(void);
uint8_t splayer_pinput(int8_t key, uint32_t arg);

void splayer_draw(void);
void splayer_update(void);
uint8_t splayer_input_handler(int8_t key, uint32_t arg);


#endif
