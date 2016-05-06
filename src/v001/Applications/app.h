#ifndef APP_H
#define APP_H 1


#include "system.h"

typedef struct{
	uint8_t id;
	
	uint8_t (*start)(void);
	uint8_t (*stop)(void);
	void (*draw)(void);
	void (*update)(void);
	void (*input_handler)(int8_t, uint32_t);
} AppInfo;

#include "app_def.h"


void app_set(uint8_t id);

void app_update(void); //thread updating current app
void app_draw(void);   //draw current app
uint8_t app_input_handler(int8_t key, uint32_t arg); //current app's keyboard handler


#endif
