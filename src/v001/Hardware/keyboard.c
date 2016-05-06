#include "keyboard.h"

uint16_t keyboard_adcLevels[] = {540, 1080, 1630, 2210, 2960, 4020};

uint32_t keyboard_ADC_values[40], keyboard_pressTime = 0;
int8_t keyboard_lastKey = -1,
	keyboard_lastLeftLvl = -1,
	keyboard_lastRightLvl = -1,
	keyboard_wasClick = 0;

void (*keyboard_KeyPressHandler)(int8_t, uint32_t);


void keyboard_handle(int8_t key, uint32_t arg)
{
	if(key >= 0)
		(*keyboard_KeyPressHandler)(key, arg);
}

void keyboard_setHandler(void (*KeyPressHandler)(int8_t, uint32_t))
{
	keyboard_KeyPressHandler = KeyPressHandler;
}


uint8_t keyboard_init()
{
	HAL_ADC_Start(&keyboard_hadc);
	HAL_ADC_Start_DMA(&keyboard_hadc, keyboard_ADC_values , 2);
	HAL_GPIO_WritePin(keyboard_on, GPIO_PIN_SET);
	return SYS_OK;
}


void keyboard_update()
{
	HAL_GPIO_WritePin(keyboard_on, GPIO_PIN_SET);
	HAL_ADC_Start_DMA(&keyboard_hadc, keyboard_ADC_values , 2);
	int8_t cul = keyboard_adc_to_lvl(keyboard_ADC_values[0]),
		cur = keyboard_adc_to_lvl(keyboard_ADC_values[1]), 
		kll = keyboard_lastLeftLvl, klr = keyboard_lastRightLvl;
	
	keyboard_lastLeftLvl = cul;
	keyboard_lastRightLvl = cur;
	
	HAL_GPIO_WritePin(keyboard_on, GPIO_PIN_RESET);
	
	if((cul >= 0 || cur >= 0))
	{
		int8_t cu = keyboard_lvl_to_key(cul, cur);
		if(cu == keyboard_lastKey) //if last levels equal to current
		{
			if(keyboard_wasClick == 0)
			{
				if(HAL_GetTick() - keyboard_pressTime >= 10)
				{
					keyboard_handle(cu, 0); //call handler for click button event
					keyboard_wasClick = 1;
				}
			}
			else
			{
				keyboard_handle(cu, HAL_GetTick() - keyboard_pressTime); //call handler for press button event
			}
		}
		else
		{
			if(keyboard_lastKey >= 0) //if there is last key
			{
				keyboard_handle(keyboard_lastKey, 1); //call handler for release button event
			}
			keyboard_lastKey = -1;
			keyboard_pressTime = 0;
			if(cu >= 0)
			{
				keyboard_pressTime = HAL_GetTick();
				keyboard_wasClick = 0;
			}
		}
		keyboard_lastKey = cu;
	}
	else
	{
		if(keyboard_lastKey >= 0) //if there is last key
			{
				keyboard_handle(keyboard_lastKey, 1); //call handler for release button event
			}
		keyboard_pressTime = 0;
		keyboard_lastKey = -1;
	}
}


int8_t keyboard_adc_to_lvl(uint16_t adc)
{
	if(adc < keyboard_adcLevels[0] - keyboard_adcDeltaError)
		return -1;
	
	int i = 0;
	while(!(adc >= keyboard_adcLevels[i] - keyboard_adcDeltaError && adc <= keyboard_adcLevels[i] + keyboard_adcDeltaError))
		i++;
	return i;
}

int8_t keyboard_lvl_to_key(int8_t l, int8_t r)
{
	if(l >= 0)
		return  0  * (l == 0) + (3 * (l - 1) + 2) * (l <= 3 && l >= 1) + 4 * (l == 4) + 'a' * (l == 5);
	else if(r >= 0)
		return 'c' * (r == 0) +  3 * r *            (r <= 3 && r >= 1) + 7 * (r == 4) +  1  * (r == 5);
	return -1;
}

char keyboard_key_to_char(int8_t key)
{
	if(key < 'a')
		return '0' + key;
	return key;
}
