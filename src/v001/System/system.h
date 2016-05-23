#define SYS_OK        0x00
#define SYS_INITING   0x01
#define SYS_ERROR     0x02
#define SYS_BUSY      0x03

#define SYS_HANDLED       0x10
#define SYS_NOT_HANDLED   0x11

#ifndef SYSTEM_H
#define SYSTEM_H 100

#include "sys_debug.h"
#include "hardware.h"
#include "at24c.h"
#include "ssd1306.h"
#include "keyboard.h"
#include "gui.h"

#include "ff.h"
#include "rtc.h"

#include "stdlib.h"
#include "string.h"
#include "hw_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "vs1053.h"

#include "sys_gui.h"

#include "app.h"

uint8_t sys_init(void);

uint8_t sys_isInited(void);

uint8_t sys_start_threads(void);


void sys_draw(void const * argument);                            //thread drawing GUI
void sys_update(void const * argument);                          //thread updating system
void sys_input_handler(int8_t key, uint32_t state);           //keyboard handler

void sys_thread_keyboard(void const * argument);
void sys_thread_player(void const * argument);
void sys_thread_hw(void const * argument);

BYTE GetCPU_IDLE(void);


#endif
