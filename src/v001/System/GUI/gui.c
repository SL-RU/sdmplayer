#include "gui.h"

uint8_t GUI_Inited = HAL_ERROR;


//global vars for message
char *GUI_MessageText;
void (*GUI_MessageDraw)();
uint8_t (*GUI_MessageInput)(uint8_t);

FontDef_t* GUI_DefFont;

uint8_t gui_xOrigin = 0;
uint8_t gui_yOrigin = 0;

uint8_t gui_init()
{
	GUI_Inited = SSD1306_Init();
	gui_setFont(&Font_7x10);
	return GUI_Inited;
}

void gui_setOrigin(uint8_t x, uint8_t y)
{
	gui_xOrigin = x;
	gui_yOrigin = y;
}
uint8_t gui_getOriginX()
{
	return gui_xOrigin;
}
uint8_t gui_getOriginY()
{
	return gui_yOrigin;
}

void gui_point(uint8_t x, uint8_t y, uint8_t col)
{
	SSD1306_DrawPixel(x, y, col);
}
void gui_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t col)
{
	SSD1306_DrawLine(x1 + gui_xOrigin, y1 + gui_yOrigin, x2 + gui_xOrigin, y2 + gui_yOrigin, col);
}
void gui_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t brdColor)
{
	SSD1306_DrawRectangle(x + gui_xOrigin, y + gui_yOrigin, w, h, brdColor);
}
void gui_circle(uint8_t x, uint8_t y, uint8_t r, uint8_t brdColor)
{
	SSD1306_DrawCircle(x + gui_xOrigin, y + gui_yOrigin, r, brdColor);
}
void gui_rect_fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bgColor, uint8_t brdColor)
{
	SSD1306_DrawFilledRectangle(x + gui_xOrigin, y + gui_yOrigin, w, h, bgColor);
	if(bgColor != brdColor)
		SSD1306_DrawRectangle(x + gui_xOrigin, y + gui_yOrigin, w, h, brdColor);
}
void gui_circle_fill(uint8_t x, uint8_t y, uint8_t r, uint8_t bgColor, uint8_t brdColor)
{
	SSD1306_DrawFilledCircle(x + gui_xOrigin, y + gui_yOrigin, r, brdColor);
	if(bgColor != brdColor)
		SSD1306_DrawCircle(x + gui_xOrigin, y + gui_yOrigin, r, brdColor);
}

void gui_setFont(FontDef_t* font)
{
	GUI_DefFont = font;
}

void gui_text(char* txt, uint8_t x, uint8_t y, uint8_t col)
{
	SSD1306_GotoXY(x + gui_xOrigin, y + gui_yOrigin);
	SSD1306_Puts(txt, GUI_DefFont, col);
}


void gui_lable(char* txt, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bg, uint8_t border)
{
	gui_rect_fill(x, y, w, h, bg, border);
	if(border)
		gui_rect(x, y, w, h, !bg);
	uint8_t max_x = x + border + gui_xOrigin;
	SSD1306_GotoXY(x + border + gui_xOrigin, y + border  + gui_yOrigin + (h - GUI_DefFont->FontHeight)/2 + 1);
	while(*txt && max_x + GUI_DefFont->FontWidth < SSD1306_WIDTH && max_x + GUI_DefFont->FontWidth < x + w + gui_xOrigin)
	{
		max_x += GUI_DefFont->FontWidth;
		SSD1306_Putc(*txt, GUI_DefFont, !bg);
		*txt++;
	}
}
void gui_lablef(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bg, uint8_t border, char* format, ...)
{
	char *buf = pvPortMalloc(w / GUI_DefFont->FontWidth);
	
	memset(buf, 0, w / GUI_DefFont->FontWidth);
	va_list args;
  va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);
	gui_lable(buf, x, y, w, h, bg, border);

	vPortFree(buf);
}
void gui_lable_multiline(char* txt, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bg, uint8_t border)
{
	gui_rect_fill(x, y, w, h, bg, bg);
	uint8_t max_x = x + border + gui_xOrigin, cy = y + border + gui_yOrigin;
	SSD1306_GotoXY(x+border + gui_xOrigin, cy);
	while(*txt)
	{
		if(max_x + GUI_DefFont->FontWidth > SSD1306_WIDTH || max_x + GUI_DefFont->FontWidth > x + w || *txt == '\n' || *txt == '\r')
		{
			cy += GUI_DefFont->FontHeight;
			SSD1306_GotoXY(x+border + gui_xOrigin, cy);
			max_x = x + border;
			if(cy + GUI_DefFont->FontHeight > y + h - border)
			{
				if(border)
					gui_rect(x, y, w, h, !bg);
				return;
			}
		}
		max_x += GUI_DefFont->FontWidth;
		if(*txt != '\n' && *txt != '\r')
			SSD1306_Putc(*txt, GUI_DefFont, !bg);
		*txt++;
	}
	if(border)
		gui_rect(x, y, w, h, !bg);
}





void gui_showMessage(char* text)
{
	//if(GUI_MessageText != 0)
	//	free(GUI_MessageText);
	GUI_MessageText = text;
	GUI_MessageDraw = 0;
	GUI_MessageInput = 0;
}
void gui_showCustomMessage(void (*drawmsg)(), uint8_t (*msginput)(uint8_t))
{
	GUI_MessageText = 0;
	GUI_MessageDraw = drawmsg;
	GUI_MessageInput = msginput;
}
void gui_closeMessage(void)
{
	GUI_MessageText = 0;
	GUI_MessageDraw = 0;
	GUI_MessageInput = 0;
}
uint8_t gui_draw_message()
{
	if(GUI_MessageText == 0 && GUI_MessageInput == 0 && GUI_MessageDraw == 0)
		return 0;
	if(GUI_MessageDraw != 0)
	{
		GUI_MessageDraw();
		return 1;
	}
	
	gui_rect(0, 0, 128, 64, 1);
	gui_lable_multiline(GUI_MessageText, 2, 2, 122, 60, 0, 1);
	return 1;
}
uint8_t gui_input_message(uint8_t key)
{
	if(GUI_MessageText == 0 && GUI_MessageInput == 0 && GUI_MessageDraw == 0)
		return 0;
	if(GUI_MessageInput != 0)
	{
		if(GUI_MessageInput(key))
		{
			return 1;
		}
	}
	gui_closeMessage();
	return 1;
}

void gui_input(int8_t key)
{
	if(gui_input_message(key))
		return;
}
void gui_upd_display()
{
	SSD1306_UpdateScreen();
	SSD1306_Fill(SSD1306_COLOR_BLACK);
}
void gui_draw(void)
{
	if(gui_draw_message())
	{
		gui_upd_display();
		return;
	}
	gui_upd_display();
}
