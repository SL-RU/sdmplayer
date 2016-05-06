#ifndef APP_TEST_H
#define APP_TEST_H

#include "app.h"

AppInfo* app_test(void);

uint8_t app_test_start(void);
uint8_t app_test_stop(void);

void app_test_draw(void);
void app_test_update(void);
void app_test_input_handler(int8_t key, uint32_t arg);

#endif
