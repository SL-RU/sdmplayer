#include "sys_gui_menu.h"

uint8_t sys_gui_menu_mode = 0;
uint8_t sys_gui_menu_focus = 0;

#define SYS_GUI_MENU_MAIN 0
#define SYS_GUI_MENU_APPSEL 1

uint8_t sys_gui_menu_cur_page = SYS_GUI_MENU_MAIN;


void sys_gui_menu_debug_click(uint16_t id, uint32_t arg, uint8_t eventType)
{
	sys_gui_debug_start();
	sys_gui_menu_mode = 0;
	sys_gui_menu_cur_page = SYS_GUI_MENU_MAIN;
}
char* sys_gui_menu_lables[] = {
	"vol",
	"apps",
	"mus",
	"sett",
	"dbg"
};
void sys_gui_menu_open(void)
{
	sys_gui_menu_mode = 2;
	sys_gui_menu_focus = 0;
	sys_gui_menu_cur_page = SYS_GUI_MENU_MAIN;
}
void sys_gui_menu_close(void)
{
	sys_gui_menu_mode = 0;
	sys_gui_menu_cur_page = SYS_GUI_MENU_MAIN;
}


///APP SEL
GUI_ListData* sys_gui_menu_draw_appsel_list;
GUI_ListItemData** sys_gui_menu_draw_appsel_listData;
void sys_gui_menu_appsel_close(void)
{
	sys_gui_menu_cur_page = SYS_GUI_MENU_MAIN;
	sys_gui_menu_close();
	gui_list_remove(sys_gui_menu_draw_appsel_list);
	vPortFree(sys_gui_menu_draw_appsel_listData);
}
void sys_gui_menu_appsel_click(uint16_t id, uint32_t arg, uint8_t evType)
{
	if(app_getCur() != id + 1 && app_getByID(id + 1) != 0)
	{
		app_set(id+1);
	}
	sys_gui_menu_appsel_close();
}
void sys_gui_menu_appsel_open(uint8_t key)
{
	uint8_t i = 1;
	sys_gui_menu_draw_appsel_listData = pvPortMalloc(sizeof(GUI_ListItemData*)*MAX_ID);
	for(i = 1; i<=MAX_ID; i++)
	{
		sys_gui_menu_draw_appsel_listData[i - 1] = gui_listItem_create(app_getByID(i)->name, i, 0, 0, 0);
	}
	sys_gui_menu_draw_appsel_list = gui_list_create(0, MAX_ID, sys_gui_menu_draw_appsel_listData, 0, 0, 128, 64-SYS_GUI_HEADER_HIGHT, sys_gui_menu_appsel_click, 0, 0);
}
void sys_gui_menu_appsel_draw(void)
{
	gui_list_draw(sys_gui_menu_draw_appsel_list);
}
void sys_gui_menu_appsel_input(uint8_t key)
{
	gui_list_input(sys_gui_menu_draw_appsel_list, key);
}


//MAIN
uint32_t someval = 60;
uint32_t sys_gui_menu_vol_slider_data = 0;
void sys_gui_menu_draw(void)
{
	if(sys_gui_menu_mode == 1)
	{
		gui_rect_fill(0, 59, 64, 5, (HAL_GetTick()/300)%2, (HAL_GetTick()/300)%2);
		return;
	}
	if(sys_gui_menu_mode != 2)
		return;
	
	gui_setFont(&Font_7x10);
	gui_setOrigin(0, SYS_GUI_HEADER_HIGHT);
	gui_rect_fill(0,0, 128, 64 - SYS_GUI_HEADER_HIGHT + 1, 0, 0);
	
	if(sys_gui_menu_cur_page == SYS_GUI_MENU_APPSEL) //If current page is appselect
	{
		sys_gui_menu_appsel_draw();
		return;
	}
	
	uint8_t i = 0, h = (64-SYS_GUI_HEADER_HIGHT-2)/5;
	for(i=0;i<5;i++)
  {
		gui_lable(sys_gui_menu_lables[i], 0, h*i, 31, h, sys_gui_menu_focus == i, sys_gui_menu_focus == i);
	}
	gui_line(30, 0, 30, 64-SYS_GUI_HEADER_HIGHT, 1);

	gui_Slider_draw(someval, 128, sys_gui_menu_focus == 0, 32, 0, 128 - 32); //volume slider
	if(app_getCur() != 0)   //curapp
		gui_lable(app_getByID(app_getCur())->name, 32, h*1, 128 - 32, h, 0, 0); //curapp name
	else
		gui_lable("none", 32, h*1, 128 - 32, h, 0, 0); //curapp none

}
void sys_gui_menu_update(void)
{
}
uint8_t sys_gui_menu_input_handler(int8_t key, uint32_t arg)
{
	if(sys_gui_menu_cur_page == SYS_GUI_MENU_APPSEL) //If current page is appselect
	{
		if(arg == KEYBOARD_UP)
			sys_gui_menu_appsel_input(key);
		return SYS_HANDLED;
	}
	//norm
		switch(sys_gui_menu_focus)
		{
			case 0: //vol
				if(gui_Slider_input((uint32_t*)&someval, 128, &sys_gui_menu_vol_slider_data, 1500, key, arg) == SYS_HANDLED)
				{
					set_vol(someval);
					return SYS_HANDLED;
				}
				break;
			case 1: //app
				if(key == 0)
				{
					if(arg == KEYBOARD_UP)
					{
						sys_gui_menu_cur_page = SYS_GUI_MENU_APPSEL;
						sys_gui_menu_appsel_open(key);
					}
					return SYS_HANDLED;
				}
				break;
		}
	//default
	if(arg == KEYBOARD_UP)
	{
		if(key == 'a')
		{
			sys_gui_menu_close();
			return SYS_HANDLED;
		}
		if(key == 2)
		{
			if(sys_gui_menu_focus == 0)
				sys_gui_menu_focus = 4;
			else
				sys_gui_menu_focus--;
			return SYS_HANDLED;
		}
		if(key == 8)
		{
			if(sys_gui_menu_focus == 4)
				sys_gui_menu_focus = 0;
			else
				sys_gui_menu_focus++;
			return SYS_HANDLED;
		}
		//return gui_list_input(sys_gui_menu_list, key);
	}
	return SYS_NOT_HANDLED;
}
