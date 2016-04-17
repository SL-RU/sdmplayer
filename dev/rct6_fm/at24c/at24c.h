#ifndef at24cXXX_H
#define at24cXXX_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include "stm32f1xx_hal.h"
#include "i2c.h"
#include <stdio.h>

#define at24c_I2C          hi2c2
#define at24c_I2C_useDMARx 1
#define at24c_I2C_address  0x50 << 1

	
uint8_t at24c_Init(void);

uint8_t at24c_read(uint32_t address, uint8_t *buffer, uint32_t len);
	
uint8_t at24c_write(uint32_t address, uint8_t *buffer, uint32_t len);

	
	
/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
