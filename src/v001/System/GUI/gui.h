#ifndef GUI_H
#define GUI_H 120


#define DEFAULT_FONT Font_7x10

//Event types:
#define GUI_CLICK 1
#define GUI_FOCUS_GET 2
#define GUI_FOCUS_LEAVE 3

#include <stdio.h>
#include "fonts.h"
#include "ssd1306.h"
#include "gui_el.h"
#include "gui_list.h"


extern FontDef_t* GUI_DefFont;

/**
 * @brief  Initialize gui
 * @param  None
 * @retval None
 */
uint8_t gui_init(void);


void gui_setOrigin(uint8_t x, uint8_t y);
uint8_t gui_getOriginX(void);
uint8_t gui_getOriginY(void);

void gui_point(uint8_t x, uint8_t y, uint8_t col);
void gui_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t col);
void gui_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t brdColor);
void gui_circle(uint8_t x, uint8_t y, uint8_t r, uint8_t brdColor);
void gui_rect_fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bgColor, uint8_t brdColor);
void gui_circle_fill(uint8_t x, uint8_t y, uint8_t r, uint8_t bgColor, uint8_t brdColor);


void gui_setFont(FontDef_t* font);

/**
 * @brief  Simple draw text
 * @param  txt: Text
 * @param  x: X location of left corner
 * @param  y: Y location of up corner
 * @param  col: Color
 * @retval None
 */
void gui_text(char* txt, uint8_t x, uint8_t y, uint8_t col);
/**
 * @brief  Draw text with params
 * @param  txt: Text
 * @param  x: X location of left corner
 * @param  y: Y location of up corner
 * @param  w: Width
 * @param  h: Height
 * @param  bg: lable's background color. Text color is !bg
 * @param  border: width of border. border's color is !bg
 * @retval None
 */
void gui_lable(char* txt, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bg, uint8_t border);

/**
 * @brief  Draw formated text with params
 * @param  txt: Text
 * @param  x: X location of left corner
 * @param  y: Y location of up corner
 * @param  w: Width
 * @param  h: Height
 * @param  bg: lable's background color. Text color is !bg
 * @param  border: width of border. border's color is !bg
 * @param  format: Text
 * @param  ...: arguments
 * @retval None
 */
void gui_lablef(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bg, uint8_t border, char* format, ...);


/**
 * @brief  Draw multiline text with params
 * @param  txt: Text
 * @param  x: X location of left corner
 * @param  y: Y location of up corner
 * @param  w: Width
 * @param  h: Height
 * @param  bg: lable's background color. Text color is !bg
 * @param  border: width of border. border's color is !bg
 * @retval None
 */
void gui_lable_multiline(char* txt, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bg, uint8_t border);



void gui_showMessage(char* text);
void gui_showCustomMessage(void (*drawmsg)(), uint8_t (*msginput)(uint8_t));
void gui_closeMessage(void);
uint8_t gui_draw_message(void);
uint8_t gui_input_message(uint8_t key);


void gui_input(int8_t key);

void gui_draw(void);
#endif
