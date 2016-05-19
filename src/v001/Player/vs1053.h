#include "spi.h"
#include "gpio.h"
#include <stdio.h>
#include "ff.h"
#include "keyboard.h"
#include "gui.h"
#include "vs1053_defs.h"


#define VS1053_rstD() HAL_GPIO_WritePin(VS1053_xRST, GPIO_PIN_RESET);
#define VS1053_rstU() HAL_GPIO_WritePin(VS1053_xRST, GPIO_PIN_SET)

void set_vol(uint8_t vol);
void VS1053_Init(void);
void VS1053_play(FIL* file, char* name);

void VS1053_draw(void);
uint8_t VS1053_input(uint8_t key);
