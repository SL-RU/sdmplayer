#include "gui_el.h"
#include "math.h"

void gui_ticker_draw(GUI_TickerData *dt)
{
	uint8_t maxlen = (dt->w - dt->border*2)/GUI_DefFont->FontWidth, len = 0;
	int	shift = 0;
	while(*(dt->text + len) != 0)
		len++;
	if(dt->startTick == 0)
	{
		dt->startTick = HAL_GetTick();
	}
	shift = ((HAL_GetTick() - dt->startTick) / GUI_TickerSpeed) 
		- maxlen/2; //start delay
	
	if(shift > len-maxlen)
	{
		if(shift > len-maxlen + GUI_TickerEndDelay)
		{
			shift = 0;
			dt->startTick = HAL_GetTick();
		}
		else
			shift = len-maxlen;
	}
	if(shift < 0)
	{
		shift = 0;
	}
	gui_lable(dt->text + shift, dt->x, dt->y, dt->w, dt->h, dt->bg, dt->border);
}
GUI_TickerData* gui_ticker_create(char *text, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bg, uint8_t border)
{
	GUI_TickerData *data = malloc(sizeof(GUI_TickerData));
	data->text = text;
	data->x = x;
	data->y = y;
	data->w = w;
	data->h = h;
	data->bg = bg;
	data->border = border;
	data->startTick = 0;
	return data;
}
void gui_removeTicker(GUI_TickerData *dt)
{
	free(dt->text);
	free(dt);
}



void gui_DigitInput_draw(uint32_t num, uint8_t order, uint8_t focus, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	if(w == 0)
		w = GUI_DefFont->FontWidth + 3;
	if(h == 0)
		h = GUI_DefFont->FontHeight + 2;
	
	gui_rect_fill(x, y, w, h, 0, 1);
	
	char str[2] = {0};
	str[0] = '0' + ((int)(num/pow(10,order))%10);
	gui_text(str, x + 2, y + 2, !focus);
	gui_rect(x+1, y+1, w - 2, h - 2, focus);
	gui_rect(x, y, w, h, !focus ? 1 : (HAL_GetTick()/500)%2 == 1);
	
	//////////
}
uint8_t gui_DigitInput_input(uint32_t* num, uint8_t order, uint8_t key, uint32_t arg)
{
	if(arg == KEYBOARD_UP)
	{
		if(key != 'a' && key != 'c')
		{
			uint32_t p = (((int)(*num / pow(10,order))%10) * pow(10, order));
			uint32_t d = (pow(10, order) * ((int8_t)key));
			uint32_t r = *num;
			*num = (*num - p) + d;
		}
		return key != 'a' && key != 'c' ? SYS_HANDLED : SYS_NOT_HANDLED ;
	}
	return SYS_NOT_HANDLED;
}
