#ifndef KEYBOARD_H
#define KEYBOARD_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include "adc.h"

#include "hw_config.h"
#include "system.h"

	

#define keyboard_adcDeltaError 150
	
	
uint8_t keyboard_init(void);

void keyboard_update(void);

void keyboard_setHandler(void (*KeyPressHandler)(int8_t, uint32_t));
	
void keyboard_handle(int8_t key, uint32_t arg);

int8_t keyboard_adc_to_lvl(uint16_t adc);

int8_t keyboard_lvl_to_key(int8_t left, int8_t right);

char keyboard_key_to_char(int8_t key);


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
