#ifndef FILE_VIEWER_ID
#define FILE_VIEWER_ID 1
#include "system.h"

AppInfo* file_viewer(void);

/**
 * @brief  open file. WARN! COULD CLOSE current app
 * @param  path: file's path
 * @retval SYS_OK - if opened else - SYS_ERROR
 */
uint8_t open_file(char* file); 


uint8_t file_viewer_start(void);
uint8_t file_viewer_stop(void);

void file_viewer_draw(void);
void file_viewer_update(void);
uint8_t file_viewer_input_handler(int8_t key, uint32_t arg);
#endif
