/**
  *****************************************************************************
  * @file    gui_menu.c
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2016-08-18
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "gui_menu.h"
#include "hal_ebrd.h"
#include "guilib.h"
#include "canfifo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

tBtnWidget btn_test_menu[] =
{
	ButtonWidget(420, 5,  60,  30, 0x4000,  PAGE_ID_MENU),
		
	ButtonWidget(1,  50,  80,  50, 0x4000,  PAGE_ID_E534),
	ButtonWidget(1,  50,  80,  50, 0x4000,  PAGE_ID_E481),
	ButtonWidget(1,  50,  80,  50, 0x4000,  PAGE_ID_E438),
	ButtonWidget(1,  50,  80,  50, 0x4000,  PAGE_ID_TEST_XZQ),
	
	ButtonWidget(1, 130,  80,  50, 0x4000,  PAGE_ID_TEST_MOT),
	ButtonWidget(1, 130,  80,  50, 0x4000,  PAGE_ID_TEST_EMF),
	ButtonWidget(1, 130,  80,  50, 0x4000,  PAGE_ID_TEST_IN),
	ButtonWidget(1, 130,  80,  50, 0x4000,  PAGE_ID_TEST_E644),

	ButtonWidget(1, 130,  80,  50, 0x4000,  PAGE_ID_TEST_E697),
	
	ButtonWidget(1, 130,  80,  50, 0x4000,  PAGE_ID_TEST_E403),
	
};

char btn_test_text[][20] = 
{
	"Exit",
	"E534",
	"E481",
	"E438",
	"XZQ",
	"MOT",
	"EMF",
	"INPUT",
	"E644",
	"E697",	
	"E403",
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void gui_test_menu_process(void)
{
	int i;

	GUI_Clr(VM_COLOR_BG);
	GUI_DrawRect(  1,  1, LCD_WIDTH-2, LCD_HEIGH-2, VM_COLOR_BLUE, 0x0);

	/*  title area */
	GUI_DrawLine(  1, 30, LCD_WIDTH-2,  30, VM_COLOR_BLUE);
	GUI_DrawLine(420,  1, 420, 30, VM_COLOR_BLUE);

	GUI_DrawAscii(	5,	5, "Test Menu", VM_COLOR_BLACK, VM_COLOR_BG);
	GUI_DrawAscii(430, 5, "Exit", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);

	for (i = 0; i < COUNTOF(btn_test_menu)-1; i++)
	{
		int x,y,w,h;
		int pt, chs;

		w = 100;
		h = 40;
		x = 16 + (i%4)*(w+16);
		y = 10 + 40 + (i/4)*(h+40);
		
		GUI_DrawRect(x, y, w, h, VM_COLOR_BLUE, 0x0);
		
		btn_test_menu[i+1].ha_x = x;
		btn_test_menu[i+1].ha_y = y;
		btn_test_menu[i+1].ha_w = w;
		btn_test_menu[i+1].ha_h = h;

		chs = strlen(btn_test_text[i+1]);
		pt = chs * 8;
		pt = (w - pt)/2;

		GUI_DrawAscii(x+pt, y+h/2-10, btn_test_text[i+1], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	}

	touchctl_HotArea_Register(btn_test_menu, COUNTOF(btn_test_menu));

	while (pageid == PAGE_ID_TEST_MENU)
	{
		int ret;
		int evt, sts;
		
		ret = system_event_pop(&evt, &sts);
		if (ret <= 0)
			continue;

		pageid = evt;
	}
	
	touchctl_HotArea_Dregister(btn_test_menu);
}


