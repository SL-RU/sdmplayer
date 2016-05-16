#include "gui_list.h"

GUI_ListData* gui_list_create(char* header, uint16_t count, GUI_ListItemData** items, uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
	void (*onClick)(uint16_t, uint32_t, uint8_t eventType), void (*onFocus)(uint16_t, uint32_t, uint8_t eventType), void (*onDeFocus)(uint16_t, uint32_t, uint8_t eventType))
{
	GUI_ListData *ld = pvPortMalloc(sizeof(GUI_ListData));
	ld->header = header;
	ld->x = x;
	ld->y = y;
	ld->w = w;
	ld->h = h;
	ld->selectedItem = 0;
	ld->ItemsCount = count;
	
	ld->ClickHandler = onClick;
	ld->FocusHandler = onFocus;
	ld->DeFocusHandler = onDeFocus;
	
	uint16_t i;
	for(i = 0; i < count; i++)
	{
		items[i]->id = i;
	}

	
	ld->items = items;
	
	return ld;
}
void gui_list_remove(GUI_ListData* list)
{
	vPortFree(list->header);
	uint16_t i;
	for(i = 0; i < list->ItemsCount; i++)
	{
		gui_listItem_remove(list->items[i]);
	}
	vPortFree(list);
}
GUI_ListItemData* gui_listItem_create(char* text, uint32_t arg,
	void (*onClick)(uint16_t, uint32_t, uint8_t eventType), void (*onFocus)(uint16_t, uint32_t, uint8_t eventType), void (*onDeFocus)(uint16_t, uint32_t, uint8_t eventType))
{
	GUI_ListItemData *dt = pvPortMalloc(sizeof(GUI_ListItemData));
	dt->text = text;
	dt->arg = arg;
	dt->ClickHandler = onClick;
	dt->FocusHandler = onFocus;
	dt->DeFocusHandler = onDeFocus;
	return dt;
}
void gui_listItem_remove(GUI_ListItemData *ld)
{
	vPortFree(ld->text);
	vPortFree(ld);
}

uint8_t gui_list_draw(GUI_ListData* gui_CurList)
{
	gui_rect_fill(gui_CurList->x, gui_CurList->y, gui_CurList->w, gui_CurList->h, 0, 1);
	
	uint8_t ry = gui_CurList->y + 2;
	if(gui_CurList->header != 0)
	{
		gui_text(gui_CurList->header, gui_CurList->x + 1, gui_CurList->y, 1);
		ry += GUI_DefFont->FontHeight;
	}
	
	uint8_t maxC = ((gui_CurList->h - 3) / GUI_DefFont->FontHeight) - (gui_CurList->header != 0);
	
	uint16_t i;
	if(maxC >= gui_CurList->ItemsCount)
	{
		for(i = 0; i < gui_CurList->ItemsCount; i++)
		{
			if(i != gui_CurList->selectedItem)
				gui_lable(gui_CurList->items[i]->text, gui_CurList->x + 1, ry + i*GUI_DefFont->FontHeight, gui_CurList->w - 3, GUI_DefFont->FontHeight, 0, 0);
			else
				gui_lable(gui_CurList->items[i]->text, gui_CurList->x +1 , ry + i*GUI_DefFont->FontHeight, gui_CurList->w - 3, GUI_DefFont->FontHeight, 1, 0);
		}
	}
	else
	{
		if(gui_CurList->ItemsCount - 1 - gui_CurList->selectedItem < maxC / 2)
		{
			for(i = gui_CurList->ItemsCount - maxC; i < gui_CurList->ItemsCount; i++)
			{
				if(i != gui_CurList->selectedItem)
					gui_lable(gui_CurList->items[i]->text, gui_CurList->x + 1, ry + (i - gui_CurList->ItemsCount + maxC)*GUI_DefFont->FontHeight, gui_CurList->w - 3, GUI_DefFont->FontHeight, 0, 0);
				else
					gui_lable(gui_CurList->items[i]->text, gui_CurList->x + 1, ry + (i - gui_CurList->ItemsCount + maxC)*GUI_DefFont->FontHeight, gui_CurList->w - 3, GUI_DefFont->FontHeight, 1, 0);
			}
		}
		else if(gui_CurList->selectedItem < maxC / 2)
		{
			for(i = 0; i < maxC; i++)
			{
				if(i != gui_CurList->selectedItem)
					gui_lable(gui_CurList->items[i]->text, gui_CurList->x + 1, ry + i*GUI_DefFont->FontHeight, gui_CurList->w - 3, GUI_DefFont->FontHeight, 0, 0);
				else
					gui_lable(gui_CurList->items[i]->text, gui_CurList->x + 1, ry + i*GUI_DefFont->FontHeight, gui_CurList->w - 3, GUI_DefFont->FontHeight, 1, 0);
			}
		}
		else
		{
			for(i = gui_CurList->selectedItem - maxC/2; i < gui_CurList->selectedItem - maxC/2 + maxC; i++)
			{
				if(i != gui_CurList->selectedItem)
					gui_lable(gui_CurList->items[i]->text, gui_CurList->x + 1, ry + (i - gui_CurList->selectedItem + maxC/2)*GUI_DefFont->FontHeight, gui_CurList->w - 3, GUI_DefFont->FontHeight, 0, 0);
				else
					gui_lable(gui_CurList->items[i]->text, gui_CurList->x + 1, ry + (i - gui_CurList->selectedItem + maxC/2)*GUI_DefFont->FontHeight, gui_CurList->w - 3, GUI_DefFont->FontHeight, 1, 0);
			}
		}
	}
	uint8_t sli_h = gui_CurList->h / gui_CurList->ItemsCount;
	if(sli_h < 10)
		sli_h = 10;
	uint8_t yy = ((gui_CurList->h) * gui_CurList->selectedItem) / gui_CurList->ItemsCount;
	gui_line(gui_CurList->x, ry - 2,  gui_CurList->x + gui_CurList->w, ry - 2, 1);
	gui_line(gui_CurList->x + gui_CurList->w - 1, gui_CurList->y + yy,  gui_CurList->x + gui_CurList->w - 1, gui_CurList->y + yy + sli_h, 1);
	
	return SYS_OK;
}

uint8_t gui_list_input(GUI_ListData* gui_CurList, int8_t key)
{
	if(gui_CurList == 0)
		return SYS_NOT_HANDLED;
	if(key == 2)  //KEY UP
	{
		if(gui_CurList->items[gui_CurList->selectedItem]->DeFocusHandler != 0) //event handler
		{
			gui_CurList->items[gui_CurList->selectedItem]->DeFocusHandler(gui_CurList->selectedItem, gui_CurList->items[gui_CurList->selectedItem]->arg, GUI_FOCUS_LEAVE);
		}
		else if(gui_CurList->DeFocusHandler != 0) //event handler
		{
			gui_CurList->DeFocusHandler(gui_CurList->selectedItem, gui_CurList->items[gui_CurList->selectedItem]->arg, GUI_FOCUS_LEAVE);
		}
		
		if(gui_CurList->selectedItem != 0)
			gui_CurList->selectedItem --;
		else
			gui_CurList->selectedItem = gui_CurList->ItemsCount - 1;
		
		if(gui_CurList->items[gui_CurList->selectedItem]->FocusHandler != 0) //event handler
		{
			gui_CurList->items[gui_CurList->selectedItem]->FocusHandler(gui_CurList->selectedItem, gui_CurList->items[gui_CurList->selectedItem]->arg, GUI_FOCUS_GET);
		}
		else if(gui_CurList->FocusHandler != 0) //event handler
		{
			gui_CurList->FocusHandler(gui_CurList->selectedItem, gui_CurList->items[gui_CurList->selectedItem]->arg, GUI_FOCUS_GET);
		}		
		return SYS_HANDLED;
	}
	if(key == 8) //KEY DOWN
	{
		if(gui_CurList->items[gui_CurList->selectedItem]->DeFocusHandler != 0) //event handler
		{
			gui_CurList->items[gui_CurList->selectedItem]->DeFocusHandler(gui_CurList->selectedItem, gui_CurList->items[gui_CurList->selectedItem]->arg, GUI_FOCUS_LEAVE);
		}
		else if(gui_CurList->DeFocusHandler != 0) //event handler
		{
			gui_CurList->DeFocusHandler(gui_CurList->selectedItem, gui_CurList->items[gui_CurList->selectedItem]->arg, GUI_FOCUS_LEAVE);
		}
		if(gui_CurList->selectedItem != gui_CurList->ItemsCount - 1)
			gui_CurList->selectedItem ++;
		else
			gui_CurList->selectedItem = 0;
		
		if(gui_CurList->items[gui_CurList->selectedItem]->FocusHandler != 0) //event handler
		{
			gui_CurList->items[gui_CurList->selectedItem]->FocusHandler(gui_CurList->selectedItem, gui_CurList->items[gui_CurList->selectedItem]->arg, GUI_FOCUS_GET);
		}
		else if(gui_CurList->FocusHandler != 0) //event handler
		{
			gui_CurList->FocusHandler(gui_CurList->selectedItem, gui_CurList->items[gui_CurList->selectedItem]->arg, GUI_FOCUS_GET);
		}		
		return SYS_HANDLED;
	}
	if(key == 0) //KEY OK
	{
		if(gui_CurList->items[gui_CurList->selectedItem]->ClickHandler != 0) //event handler
		{
			gui_CurList->items[gui_CurList->selectedItem]->ClickHandler(gui_CurList->selectedItem, gui_CurList->items[gui_CurList->selectedItem]->arg, GUI_CLICK);
		}
		else if(gui_CurList->ClickHandler != 0) //event handler
		{
			gui_CurList->ClickHandler(gui_CurList->selectedItem, gui_CurList->items[gui_CurList->selectedItem]->arg, GUI_CLICK);
		}
	}
	return SYS_NOT_HANDLED;
}

