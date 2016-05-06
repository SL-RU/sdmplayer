#ifndef SYSTEM_DBG_H
#define SYSTEM_DBG 100

#include "system.h"
#include "hardware.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "stm32f1xx_hal.h"


#define LOG_TO_EEPROM 1
#define LOG_TO_STD    1

uint8_t sys_debug_init(void);

void slog(const char *format, ...); //Log something

void wlog(const char *format, ...); //Log warning

void elog(const char *format, ...); //Log error

void SYS_ERROR_HANDLER(void);
#endif
