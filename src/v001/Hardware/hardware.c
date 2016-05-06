#include "hardware.h"
#include "system.h"
/*basic functions for work with HW*/

uint32_t hw_sdcard_capacity() //return sdcard size in Mbytes 
{
	uint32_t c = 0, s = 0;
	disk_ioctl(0, GET_SECTOR_COUNT, &c);
	disk_ioctl(0, GET_SECTOR_SIZE,  &s);
	c=c/2;
	return c/(1024);
}
