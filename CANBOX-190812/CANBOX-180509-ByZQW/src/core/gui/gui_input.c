/**
  *****************************************************************************
  * @file    gui_e438.c
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2016-12-05
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "gui_input.h"
#include "hal_ebrd.h"
#include "guilib.h"
#include "canfifo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

enum {
	BTN_EXIT = 0,
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static tBtnWidget input_btn_item[] =
{
	ButtonWidget(420, 5,  60,  30, 0x4000,  BTN_EXIT),
		
};

static char input_btn_text[][12] = 
{
	"Exit",
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void gui_input_process_E644(void)
{

	int i;
	int x,y,w,h;
	int pt, chs;
	int ret;
	int evt, sts;
	
	
	

	GUI_Clr(VM_COLOR_BG);
	GUI_DrawRect(  1,  1, LCD_WIDTH-2, LCD_HEIGH-2, VM_COLOR_BLUE, 0x0);

	/*  title area */
	GUI_DrawLine(  1, 30, LCD_WIDTH-2,  30, VM_COLOR_BLUE);
	GUI_DrawLine(420,  1, 420, 30, VM_COLOR_BLUE);

	GUI_DrawAscii(	5,	5, "E644 input Test", VM_COLOR_BLACK, VM_COLOR_BG);
	GUI_DrawAscii(430, 5, "Exit", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);

	for (i = 0; i < COUNTOF(input_btn_item)-1; i++)
	{
		tBtnWidget *pbtn;

		w = 100;
		h = 60;
		x = 16 + (i%4)*(w+16);
		y = 30 + 40 + (i/4)*(h+40);
		
		GUI_DrawRect(x, y, w, h, VM_COLOR_BLUE, 0x0);
		
		pbtn = &input_btn_item[i+1];
		pbtn->ha_x = x;
		pbtn->ha_y = y;
		pbtn->ha_w = w;
		pbtn->ha_h = h;

		chs = strlen(input_btn_text[i+1]);
		pt = chs * 8;
		pt = (w - pt)/2;

		GUI_DrawAscii(x+pt, y+h/2-10, input_btn_text[i+1], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	}

	touchctl_HotArea_Register(input_btn_item, COUNTOF(input_btn_item));
	//E481_step_son(0);

	

	hal_e644_input_poll(1);
	while (1)
	{
		hal_e644_input_poll(0);
		
		ret = system_event_pop(&evt, &sts);
		if (ret <= 0)
			continue;

		switch (evt)
		{
			case BTN_EXIT:{
				pageid = PAGE_ID_TEST_MENU;
				touchctl_HotArea_Dregister(input_btn_item);
			}return;
		}
	}
}





void gui_input_process(void)
{
	int i;
	int x,y,w,h;
	int pt, chs;
	int ret;
	int evt, sts;
	
	hal_E481_init();

	GUI_Clr(VM_COLOR_BG);
	GUI_DrawRect(  1,  1, LCD_WIDTH-2, LCD_HEIGH-2, VM_COLOR_BLUE, 0x0);

	/*  title area */
	GUI_DrawLine(  1, 30, LCD_WIDTH-2,  30, VM_COLOR_BLUE);
	GUI_DrawLine(420,  1, 420, 30, VM_COLOR_BLUE);

	GUI_DrawAscii(	5,	5, "input Test", VM_COLOR_BLACK, VM_COLOR_BG);
	GUI_DrawAscii(430, 5, "Exit", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);

	for (i = 0; i < COUNTOF(input_btn_item)-1; i++)
	{
		tBtnWidget *pbtn;

		w = 100;
		h = 60;
		x = 16 + (i%4)*(w+16);
		y = 30 + 40 + (i/4)*(h+40);
		
		GUI_DrawRect(x, y, w, h, VM_COLOR_BLUE, 0x0);
		
		pbtn = &input_btn_item[i+1];
		pbtn->ha_x = x;
		pbtn->ha_y = y;
		pbtn->ha_w = w;
		pbtn->ha_h = h;

		chs = strlen(input_btn_text[i+1]);
		pt = chs * 8;
		pt = (w - pt)/2;

		GUI_DrawAscii(x+pt, y+h/2-10, input_btn_text[i+1], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	}

	touchctl_HotArea_Register(input_btn_item, COUNTOF(input_btn_item));
	E481_step_son(0);

	hal_e481_input_poll(1);
	while (1)
	{
		hal_e481_input_poll(0);
		ret = system_event_pop(&evt, &sts);
		if (ret <= 0)
			continue;

		switch (evt)
		{
			case BTN_EXIT:{
				pageid = PAGE_ID_TEST_MENU;
				touchctl_HotArea_Dregister(input_btn_item);
			}return;
		}
	}
	
}






