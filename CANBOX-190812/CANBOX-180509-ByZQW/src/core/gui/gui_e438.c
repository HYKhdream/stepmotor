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
#include "gui_e438.h"
#include "hal_ebrd.h"
#include "guilib.h"
#include "canfifo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

enum {
	BTN_EXIT = 0,
	BTN_MOT_AUTO,
	BTN_MOT_TQ,
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

tBtnWidget btn_item[] =
{
	ButtonWidget(420, 5,  60,  30, 0x4000,  BTN_EXIT),
	ButtonWidget(1,  60,  80,  70, 0x4000,  BTN_MOT_AUTO),
	ButtonWidget(1,  60,  80,  70, 0x4000,  BTN_MOT_TQ),
};


char btn_item_text[][20] = 
{
	"Exit",
	"ON/OFF",
	"800mA",
};


static char mot_test_on = 0;
static int mot_sel_ma = 800;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

void gui_e438_test_process(void)
{
	int i;
	int x,y,w,h;
	int pt, chs;
	int ret;
	int evt, sts;

	
	hal_E438_init();

	GUI_Clr(VM_COLOR_BG);
	GUI_DrawRect(  1,  1, LCD_WIDTH-2, LCD_HEIGH-2, VM_COLOR_BLUE, 0x0);

	/*  title area */
	GUI_DrawLine(  1, 30, LCD_WIDTH-2,  30, VM_COLOR_BLUE);
	GUI_DrawLine(420,  1, 420, 30, VM_COLOR_BLUE);

	GUI_DrawAscii(	5,	5, "E438 Test", VM_COLOR_BLACK, VM_COLOR_BG);
	GUI_DrawAscii(430, 5, "Exit", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);

	for (i = 0; i < COUNTOF(btn_item)-1; i++)
	{
		tBtnWidget *pbtn;

		w = 100;
		h = 60;
		x = 16 + (i%4)*(w+16);
		y = 30 + 40 + (i/4)*(h+40);
		
		GUI_DrawRect(x, y, w, h, VM_COLOR_BLUE, 0x0);
		
		pbtn = &btn_item[i+1];
		pbtn->ha_x = x;
		pbtn->ha_y = y;
		pbtn->ha_w = w;
		pbtn->ha_h = h;

		chs = strlen(btn_item_text[i+1]);
		pt = chs * 8;
		pt = (w - pt)/2;

		GUI_DrawAscii(x+pt, y+h/2-10, btn_item_text[i+1], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	}

	if (mot_test_on)
	{
		GUI_DrawRect(btn_item[BTN_MOT_AUTO].ha_x+4, btn_item[BTN_MOT_AUTO].ha_y+4, 
		btn_item[BTN_MOT_AUTO].ha_w-8, btn_item[BTN_MOT_AUTO].ha_h-8, VM_COLOR_BLUE, 0x00);
	}

	evt = BTN_MOT_TQ;
	sprintf(btn_item_text[evt], "%dmA", mot_sel_ma);
	x = btn_item[evt].ha_x;
	y = btn_item[evt].ha_y;
	w = btn_item[evt].ha_w;
	h = btn_item[evt].ha_h;
	chs = strlen(btn_item_text[evt]);
	pt = chs * 8;
	pt = (w - pt)/2;
	GUI_DrawRect(x+2, y+2, w-4, h-4, VM_COLOR_WHITE, 0x80);
	GUI_DrawAscii(x+pt, y+h/2-10, btn_item_text[evt], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	
	E438_step_current(mot_sel_ma);

	touchctl_HotArea_Register(btn_item, COUNTOF(btn_item));

	while (1)
	{
		if (mot_test_on)
			hal_E438_mot_poll(0);//hal_E438_poll();

		ret = system_event_pop(&evt, &sts);
		if (ret <= 0)
			continue;

		switch (evt)
		{
			case BTN_EXIT:{
				pageid = PAGE_ID_TEST_MENU;
				touchctl_HotArea_Dregister(btn_item);
			}return;

			case BTN_MOT_AUTO:{
				if (mot_test_on)
				{
					mot_test_on = 0;
					GUI_DrawRect(btn_item[evt].ha_x+4, btn_item[evt].ha_y+4, btn_item[evt].ha_w-8, btn_item[evt].ha_h-8, VM_COLOR_WHITE, 0x00);
				}
				else
				{
					mot_test_on = 1;
					GUI_DrawRect(btn_item[evt].ha_x+4, btn_item[evt].ha_y+4, btn_item[evt].ha_w-8, btn_item[evt].ha_h-8, VM_COLOR_BLUE, 0x00);
				}
			}break;

			case BTN_MOT_TQ:{
				if (mot_sel_ma == 800)
					mot_sel_ma = 2000;
				else if (mot_sel_ma == 2000)
					mot_sel_ma = 3000;
				else
					mot_sel_ma = 800;
				
				sprintf(btn_item_text[evt], "%dmA", mot_sel_ma);

				x = btn_item[evt].ha_x;
				y = btn_item[evt].ha_y;
				w = btn_item[evt].ha_w;
				h = btn_item[evt].ha_h;
				chs = strlen(btn_item_text[evt]);
				pt = chs * 8;
				pt = (w - pt)/2;
				
				GUI_DrawRect(x+2, y+2, w-4, h-4, VM_COLOR_WHITE, 0x80);
				GUI_DrawAscii(x+pt, y+h/2-10, btn_item_text[evt], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);

				E438_step_current(mot_sel_ma);
			}break;

			
		}
	}
	
}


