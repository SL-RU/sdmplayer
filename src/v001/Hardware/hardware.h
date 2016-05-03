#ifndef SYSTEM_H
#define SYSTEM_H 100

#include "at24c.h"
#include "ssd1306.h"
#include "keyboard.h"
#include "diskio.h"
#include "system.h"

/*basic functions for work with HW*/

uint32_t hw_sdcard_capacity(void); //return sdcard size in Mbytes 


#endif