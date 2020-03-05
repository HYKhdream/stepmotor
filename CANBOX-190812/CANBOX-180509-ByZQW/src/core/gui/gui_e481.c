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
#include "gui_e481.h"
#include "hal_ebrd.h"
#include "guilib.h"
#include "canfifo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

enum {
	BTN_EXIT = 0,
	BTN_MOT_AUTO,
	BTN_XZQ_AUTO,
	BTN_EMF_AUTO,
	
	BTN_MOT_TQ,
	BTN_XZQ_HAND,
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static tBtnWidget e481_btn_item[] =
{
	ButtonWidget(420, 5,  60,  30, 0x4000,  BTN_EXIT),
	
	ButtonWidget(1,  60,  80,  70, 0x4000,  BTN_MOT_AUTO),
	ButtonWidget(1,  60,  80,  70, 0x4000, BTN_XZQ_AUTO),
	ButtonWidget(1,  60,  80,  70, 0x4000, BTN_EMF_AUTO),
	ButtonWidget(1,  60,  80,  70, 0x4000, BTN_MOT_TQ),
	ButtonWidget(1,  60,  80,  70, 0x4000, BTN_XZQ_HAND),
};

static char e481_btn_text[][12] = 
{
	"Exit",
		
	"MOT",
	"XZQ",
	"EMF",
	
	"800mA",
	"XZQH"
};

static int mot_sel_ma = 800;
static char mot_test_on = 0;
static char xzq_test_on = 0;
static char xzq_test_mode = 0;
static char emf_test_on = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void gui_e481_test_process(void)
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

	GUI_DrawAscii(	5,	5, "E481 Test", VM_COLOR_BLACK, VM_COLOR_BG);
	GUI_DrawAscii(430, 5, "Exit", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);

	for (i = 0; i < COUNTOF(e481_btn_item)-1; i++)
	{
		tBtnWidget *pbtn;

		w = 100;
		h = 60;
		x = 16 + (i%4)*(w+16);
		y = 30 + 40 + (i/4)*(h+40);
		
		GUI_DrawRect(x, y, w, h, VM_COLOR_BLUE, 0x0);
		
		pbtn = &e481_btn_item[i+1];
		pbtn->ha_x = x;
		pbtn->ha_y = y;
		pbtn->ha_w = w;
		pbtn->ha_h = h;

		chs = strlen(e481_btn_text[i+1]);
		pt = chs * 8;
		pt = (w - pt)/2;

		GUI_DrawAscii(x+pt, y+h/2-10, e481_btn_text[i+1], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	}

#if 1
	mot_test_on = 0;
	xzq_test_on = 0;
	emf_test_on = 0;
#else
	if (mot_test_on)
	{
		GUI_DrawRect(e481_btn_item[BTN_MOT_AUTO].ha_x+4, e481_btn_item[BTN_MOT_AUTO].ha_y+4, 
		e481_btn_item[BTN_MOT_AUTO].ha_w-8, e481_btn_item[BTN_MOT_AUTO].ha_h-8, VM_COLOR_BLUE, 0x00);
	}

	if (xzq_test_on)
	{
		GUI_DrawRect(e481_btn_item[BTN_XZQ_AUTO].ha_x+4, e481_btn_item[BTN_XZQ_AUTO].ha_y+4, 
		e481_btn_item[BTN_XZQ_AUTO].ha_w-8, e481_btn_item[BTN_XZQ_AUTO].ha_h-8, VM_COLOR_BLUE, 0x00);
	}

	if (emf_test_on)
	{
		GUI_DrawRect(e481_btn_item[BTN_EMF_AUTO].ha_x+4, e481_btn_item[BTN_EMF_AUTO].ha_y+4, 
		e481_btn_item[BTN_EMF_AUTO].ha_w-8, e481_btn_item[BTN_EMF_AUTO].ha_h-8, VM_COLOR_BLUE, 0x00);
	}
#endif /* 0 */

	evt = BTN_MOT_TQ;
	sprintf(e481_btn_text[evt], "%dmA", mot_sel_ma);
	x = e481_btn_item[evt].ha_x;
	y = e481_btn_item[evt].ha_y;
	w = e481_btn_item[evt].ha_w;
	h = e481_btn_item[evt].ha_h;
	chs = strlen(e481_btn_text[evt]);
	pt = chs * 8;
	pt = (w - pt)/2;
	GUI_DrawRect(x+2, y+2, w-4, h-4, VM_COLOR_WHITE, 0x80);
	GUI_DrawAscii(x+pt, y+h/2-10, e481_btn_text[evt], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	
	E481_step_current(mot_sel_ma);

	touchctl_HotArea_Register(e481_btn_item, COUNTOF(e481_btn_item));

	while (1)
	{
		if (mot_test_on)
		{
			hal_E481_mot_poll(0);
			hal_E459_trig_poll(0);
		}
		if (xzq_test_on)
		{
			hal_E481_xzq_poll(xzq_test_mode);
		}
		
		if (emf_test_on)
		{
			hal_E481_emf_poll(0);
		}

		ret = system_event_pop(&evt, &sts);
		if (ret <= 0)
			continue;

		switch (evt)
		{
			case BTN_EXIT:{
				pageid = PAGE_ID_TEST_MENU;
				touchctl_HotArea_Dregister(e481_btn_item);
			}return;

			case BTN_MOT_AUTO:{
				if (mot_test_on)
				{
					mot_test_on = 0;
					GUI_DrawRect(e481_btn_item[evt].ha_x+4, e481_btn_item[evt].ha_y+4, e481_btn_item[evt].ha_w-8, e481_btn_item[evt].ha_h-8, VM_COLOR_WHITE, 0x00);
				}
				else
				{
					mot_test_on = 1;
					GUI_DrawRect(e481_btn_item[evt].ha_x+4, e481_btn_item[evt].ha_y+4, e481_btn_item[evt].ha_w-8, e481_btn_item[evt].ha_h-8, VM_COLOR_BLUE, 0x00);
				}
			}break;

			case BTN_XZQ_HAND:{
				xzq_test_on = 0;
				hal_E481_xzq_hand(xzq_test_mode);
			}break;

			case BTN_XZQ_AUTO:{
				if (xzq_test_on)
				{
					xzq_test_mode++;
					if (xzq_test_mode == 9)
					{
						sprintf(e481_btn_text[evt], "XZQ");
						xzq_test_on = 0;
						xzq_test_mode = 0;
						GUI_DrawRect(e481_btn_item[evt].ha_x+4, e481_btn_item[evt].ha_y+4, e481_btn_item[evt].ha_w-8, e481_btn_item[evt].ha_h-8, VM_COLOR_WHITE, 0x00);
					}
					else
					{
						sprintf(e481_btn_text[evt], "M%d", xzq_test_mode);
					}
					
					{
						x = e481_btn_item[evt].ha_x;
						y = e481_btn_item[evt].ha_y;
						w = e481_btn_item[evt].ha_w;
						h = e481_btn_item[evt].ha_h;
						chs = strlen(e481_btn_text[evt]);
						pt = chs * 8;
						pt = (w - pt)/2;
						
						GUI_DrawRect(x+5, y+5, w-10, h-10, VM_COLOR_WHITE, 0x80);
						GUI_DrawAscii(x+pt, y+h/2-10, e481_btn_text[evt], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
					}
				}
				else
				{
					xzq_test_on = 1;
					GUI_DrawRect(e481_btn_item[evt].ha_x+4, e481_btn_item[evt].ha_y+4, e481_btn_item[evt].ha_w-8, e481_btn_item[evt].ha_h-8, VM_COLOR_BLUE, 0x00);
				}
			}break;

			case BTN_EMF_AUTO:{
				if (emf_test_on)
				{
					emf_test_on = 0;
					GUI_DrawRect(e481_btn_item[evt].ha_x+4, e481_btn_item[evt].ha_y+4, e481_btn_item[evt].ha_w-8, e481_btn_item[evt].ha_h-8, VM_COLOR_WHITE, 0x00);
				}
				else
				{
					emf_test_on = 1;
					GUI_DrawRect(e481_btn_item[evt].ha_x+4, e481_btn_item[evt].ha_y+4, e481_btn_item[evt].ha_w-8, e481_btn_item[evt].ha_h-8, VM_COLOR_BLUE, 0x00);
				}
			}break;

			case BTN_MOT_TQ:{
				if (mot_sel_ma == 800)
					mot_sel_ma = 2000;
				else
					mot_sel_ma = 800;
				
				sprintf(e481_btn_text[evt], "%dmA", mot_sel_ma);

				x = e481_btn_item[evt].ha_x;
				y = e481_btn_item[evt].ha_y;
				w = e481_btn_item[evt].ha_w;
				h = e481_btn_item[evt].ha_h;
				chs = strlen(e481_btn_text[evt]);
				pt = chs * 8;
				pt = (w - pt)/2;
				
				GUI_DrawRect(x+2, y+2, w-4, h-4, VM_COLOR_WHITE, 0x80);
				GUI_DrawAscii(x+pt, y+h/2-10, e481_btn_text[evt], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);

				E481_step_current(mot_sel_ma);
			}break;
		}
	}
	
}


