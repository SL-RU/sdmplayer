//SSD1306
/* SSD1306 I2C address */
#ifndef SSD1306_I2C_ADDR
#define SSD1306_I2C_ADDR         0x78
#define SSD1306_I2C         		 hi2c2
//#define SSD1306_I2C_ADDR       0x7A
#endif



//Keyboard
#define keyboard_on GPIOB, GPIO_PIN_2
#define keyboard_hadc hadc1


//EEPROM
#define at24c_I2C          hi2c1
#define at24c_I2C_useDMARx 1
#define at24c_I2C_address  0x50 << 1

//LEDs
#define LED1 GPIOC, GPIO_PIN_5
#define LED2 GPIOC, GPIO_PIN_4

#define LED1_on()	HAL_GPIO_WritePin(LED1, GPIO_PIN_SET)
#define LED1_off()	HAL_GPIO_WritePin(LED1, GPIO_PIN_RESET)
#define LED2_on()	HAL_GPIO_WritePin(LED2, GPIO_PIN_SET)
#define LED2_off()	HAL_GPIO_WritePin(LED2, GPIO_PIN_RESET)

//VS1053
#define VS1053_SPI hspi2
#define VS1053_xCS GPIOC, GPIO_PIN_7
#define VS1053_xDCS GPIOA, GPIO_PIN_6
#define VS1053_xRST GPIOB, GPIO_PIN_12
#define VS1053_DREQ GPIOC, GPIO_PIN_6
