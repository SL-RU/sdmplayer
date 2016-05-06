#include "sys_debug.h"


char *logBuffer[50]; //buffer of last 
uint8_t f = 0;

uint8_t sys_debug_init(void)
{
	return SYS_OK;
}

void slog(const char *format, ...)
{
    va_list args;
    va_start(args, format);
		printf("\n");
    vprintf(format, args);
    va_end(args);
}

void wlog(const char *format, ...)
{
    va_list args;
    va_start(args, format);
		printf("\n!");
    vprintf(format, args);
    va_end(args);
}

void elog(const char *format, ...)
{
    va_list args;
    va_start(args, format);
		printf("\n!ERROR!");
    vprintf(format, args);
    va_end(args);
}

void SYS_ERROR_HANDLER(void)
{
	while(1)
	{
		LED1_off();
		LED2_on();
		HAL_Delay(100);
		LED1_on();
		LED2_off();
		HAL_Delay(100);
	}
}
