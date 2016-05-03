#define SYS_OK        0x00
#define SYS_INITING   0x01
#define SYS_ERROR     0x02
#define SYS_BUSY      0x03

#ifndef SYSTEM_H
#define SYSTEM_H 100

#include "sys_debug.h"
#include "hardware.h"
#include "at24c.h"
#include "ssd1306.h"
#include "keyboard.h"
#include "gui.h"

#include "ff.h"

#include "stdlib.h"
#include "string.h"
#include "hw_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "sys_gui.h"

typedef struct{
	uint8_t id;
	
	uint8_t (*start)(void);
	uint8_t (*stop)(void);
	void (*draw)(void);
	void (*update)(void);
	void (*input_handler)(uint8_t, uint32_t);
} AppInfo;


uint8_t sys_init(void);

uint8_t sys_isInited(void);

uint8_t sys_start_threads(void);


void sys_draw(void const * argument);                            //thread drawing GUI
void sys_update(void const * argument);                          //thread updating system
uint8_t sys_input_handler(uint8_t key, uint32_t state);           //keyboard handler



void sys_start_app_id(uint8_t id);

void sys_update_app(void); //thread updating current app
void sys_draw_app(void);   //draw current app
uint8_t sys_input_handler_app(uint8_t key, uint32_t arg); //current app's keyboard handler



#endif