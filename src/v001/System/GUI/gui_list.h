#ifndef GUI_EL_H
#define GUI_EL_H 10
#include "gui.h"

typedef struct
{
	uint16_t id; /*!< Item's id */
	char* text;  /*!< Item's text*/
	uint32_t arg; /*!< custom value */
	void (*ClickHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user click OK button and this item is selected */
	void (*FocusHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user focused on this item */
	void (*DeFocusHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user defocused this item */
	
} GUI_ListItemData;

typedef struct
{
	char* header;  /*!< Header*/
	GUI_ListItemData **items;  /*!< Item's array*/
	uint16_t ItemsCount; /*!< Item's array*/
	uint8_t x, y, w, h;
	uint16_t selectedItem;
	
	void (*ClickHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user click OK */
	void (*FocusHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user focuses on item */
	void (*DeFocusHandler)(uint16_t id, uint32_t arg, uint8_t eventType); /*!< Function, which will be called when user defocused on item */

} GUI_ListData;
extern GUI_ListData *gui_CurList;
/**
 * @brief  Create list data structure for using listElement
 * @note   DO NOT FORGOT call @ref gui_remove_list(list);
 * @param  header: header 
 * @param  count: number of elements in items array
 * @param  items: array with elements
 * @param  x: X location of left corner
 * @param  y: Y location of up corner
 * @param  w: Width
 * @param  h: Height
 * @param  onClick: Function, which will be called when user click OK
 * @param  onFocus: Function, which will be called when user focuses on item
 * @param  onDeFocus: Function, which will be called when user defocused on item
 * @retval GUI_ListData
 */
GUI_ListData* gui_create_list(char* header, uint16_t count, GUI_ListItemData** items, uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
	void (*onClick)(uint16_t, uint32_t, uint8_t eventType), void (*onFocus)(uint16_t, uint32_t, uint8_t eventType), void (*onDeFocus)(uint16_t, uint32_t, uint8_t eventType));
/**
 * @brief  free memory from GUI_ListData
 * @param  list: GUI_ListData
 * @retval None
 */
void gui_remove_list(GUI_ListData* list);
/**
 * @brief  Create list's item data structure for using in listElement
 * @note   DO NOT FORGOT call @ref gui_remove_listItem(ld);
 * @param  text: text of the element 
 * @param  arg: custom variable  
 * @param  onClick: Function, which will be called when user click OK
 * @param  onFocus: Function, which will be called when user focuses on item
 * @param  onDeFocus: Function, which will be called when user defocused on item
 * @retval GUI_ListItemData
 */
GUI_ListItemData* gui_create_listItem(char* text, uint32_t arg,
	void (*onClick)(uint16_t, uint32_t, uint8_t eventType), void (*onFocus)(uint16_t, uint32_t, uint8_t eventType), void (*onDeFocus)(uint16_t, uint32_t, uint8_t eventType));
/**
 * @brief  free memory from GUI_ListItemData
 * @note   DO NOT FORGET gui_set_curList(0) to cancel drawing list
 * @param  ld: GUI_ListData
 * @retval None
 */
void gui_remove_listItem(GUI_ListItemData *ld);
/**
 * @brief  draw current GUI_List
 * @note   DO NOT USE IT BY YOURSELF! DO NOT FORGET gui_set_curList(data) first!
 * @param  ld: GUI_ListItemData
 * @retval None
 */
uint8_t gui_draw_list(GUI_ListData* list);
/**
 * @brief  draw current GUI_List
 * @note   DO NOT USE IT BY YOURSELF! DO NOT FORGET gui_set_curList(data) first!
 * @param  ld: GUI_ListItemData
 * @retval None
 */
uint8_t gui_input_list(GUI_ListData* list, int8_t key);
#endif
