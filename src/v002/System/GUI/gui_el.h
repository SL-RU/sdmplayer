#ifndef GUI_EL
#define GUI_EL 1

#include "gui.h"

//running line
#define GUI_TickerSpeed 500
#define GUI_TickerEndDelay 3
typedef struct 
{
	char *text;
	uint8_t x, y, w, h, bg, border;
	uint32_t startTick;
} GUI_TickerData;
/**
 * @brief  Draw running text line using GUI_TickerData structure
 * @param  txt: Text
 * @param  td: GUI_TickerData
 * @retval None
 */
void gui_ticker_draw(GUI_TickerData *dt);
/**
 * @brief  Generate GUI_TickerData structure for running text line
 * @note   DO NOT FORGOT call @ref gui_removeTicker(dt);
 * @param  txt: Text
 * @param  x: X location of left corner
 * @param  y: Y location of up corner
 * @param  w: Width
 * @param  h: Height
 * @param  bg: lable's background color. Text color is !bg
 * @param  border: width of border. border's color is !bg
 * @retval None
 */
GUI_TickerData* gui_ticker_create(char *text, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bg, uint8_t border);
/**
 * @brief  Free memory from GUI_TickerData struct
 * @param  dt: GUI_TickerData
 * @retval None
 */
void gui_removeTicker(GUI_TickerData *dt);



//DigitInput
/**
 * @brief  draw digital input field
 * @param  num: number with didgit
 * @param  order: position of digit in num
 * @param  w: width. 0 - autosize
 * @param  h: height. 0 - autosize
 * @retval None
 */
void gui_DigitInput_draw(uint32_t num, uint8_t order, uint8_t focus, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
/**
 * @brief  input handler for digital input field
 * @param  num: number with didgit
 * @param  order: position of digit in num
 * @retval SYS_HANDLED
 */
uint8_t gui_DigitInput_input(uint32_t* num, uint8_t order, uint8_t key, uint32_t arg);



//Slider
/**
 * @brief  draw slider element. Height = 10;
 * @param  val: value
 * @param  maxval: max value
 * @retval None
 */
void gui_Slider_draw(uint32_t val, uint8_t maxval, uint8_t focus, uint8_t x, uint8_t y, uint8_t w);
/**
 * @brief  input handler for slider
 * @brief  draw digital input field
 * @brief  data: just variable for technical needs.
 * @brief  fulltime: if you don't know enter 0.
 * @param  val: value
 * @retval SYS_HANDLED
 */
uint8_t gui_Slider_input(uint32_t* val, uint8_t maxval, uint32_t *data, uint16_t fulltime, uint8_t key, uint32_t arg);

#endif
