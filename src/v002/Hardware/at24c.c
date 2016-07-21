#include "at24c.h"

uint8_t at24c_Init(void)
{
	slog("AT24 initing");
	
	uint32_t p = 250000;
	while(p>0)
		p--;
	if (HAL_I2C_IsDeviceReady(&at24c_I2C, at24c_I2C_address, 1, 20000) != HAL_OK) {
		/* Return false */
		elog("AT24 I2C device not found");
		return 0;
	}
	slog("AT24 init ok\n");
	return 1;
}

uint8_t at24c_read(uint32_t address, uint8_t *buffer, uint32_t len)
{
	HAL_I2C_Mem_Read(&at24c_I2C, at24c_I2C_address, address, I2C_MEMADD_SIZE_16BIT, buffer, len, 100);
}
	
uint8_t at24c_write(uint32_t address, uint8_t *buffer, uint32_t len)
{
	HAL_I2C_Mem_Write(&at24c_I2C, at24c_I2C_address, address, I2C_MEMADD_SIZE_16BIT, buffer, len, 100);
}