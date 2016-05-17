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


//Digital input
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
			//uint32_t r = *num;
			*num = (*num - p) + d;
		}
		return key != 'a' && key != 'c' ? SYS_HANDLED : SYS_NOT_HANDLED ;
	}
	return SYS_NOT_HANDLED;
}


int NumDigits(uint32_t x)  //Sorry, but this is the fastest solution
{  
    x = abs(x);  
    return (x < 10 ? 1 :   
    	(x < 100 ? 2 :   
    	(x < 1000 ? 3 :   
    	(x < 10000 ? 4 :   
    	(x < 100000 ? 5 :   
    	(x < 1000000 ? 6 :   
    	(x < 10000000 ? 7 :  
    	(x < 100000000 ? 8 :  
    	(x < 1000000000 ? 9 :  
    	10)))))))));  
}
//Slider
void gui_Slider_draw(uint32_t val, uint8_t maxval, uint8_t focus, uint8_t x, uint8_t y, uint8_t w)
{
	uint8_t s = x + 1, lw = (GUI_DefFont->FontWidth + 1) * NumDigits(maxval), e = x + w - lw - 3;
	gui_lablef(x + w - lw, y, lw, 10, 0, 0, "%d", val);			
	gui_line(s - 1, 5 + y, e + 1, 5 + y, 1); //line
	gui_line(s - 1, 2 + y, s - 1, 8 + y, 1); //start
	gui_line(e + 1, 2 + y, e + 1, 8 + y, 1); //end
	gui_rect_fill(s + (e - s - 3)*val/maxval, 3+y, 3, 4, focus, 1); //cur
	gui_rect_fill(s, 4+y, (e - s - 3)*val/maxval, 2, 1, 1); //cur
	//gui_line(x, y, x + w, y, 1);
}
uint8_t gui_Slider_input(uint32_t* val, uint8_t maxval, uint32_t *data, uint16_t fulltime, uint8_t key, uint32_t arg)
{
	if(fulltime == 0)
		fulltime = 2500;
	if(key == 4 || key == 6)
	{
		if(arg == KEYBOARD_DOWN)
		{
			if(*val >= maxval && key == 6)
				*val = maxval;
			else if (*val == 0 && key == 4)
				*val = 0;
			else
			{
				*val += (key == 6 ? 1 : -1);
			}
			*data = *val;
		}
		else if(arg != KEYBOARD_UP && arg > 300)
		{
			if(key == 6)
			{
				if(arg - 300 >  (maxval - *data) * (fulltime / maxval))
				{
					*val = maxval;
				}
				else
				{
					*val = *data + maxval * (arg - 300) / fulltime;				
				}
			}
			else
			{
				if(arg - 300 >  (*data) * (fulltime / maxval))
					*val = 0;
				else
					*val = *data - maxval * (arg - 300) / fulltime;
			}
		}
		return SYS_HANDLED;
	}
	return SYS_NOT_HANDLED;
}
