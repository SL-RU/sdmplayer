#ifndef at24cXXX_H
#define at24cXXX_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include "stm32f1xx_hal.h"
#include "i2c.h"

#include "hw_config.h"
#include "system.h"
	
uint8_t at24c_Init(void);

uint8_t at24c_read(uint32_t address, uint8_t *buffer, uint32_t len);
	
uint8_t at24c_write(uint32_t address, uint8_t *buffer, uint32_t len);

	
	
/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
