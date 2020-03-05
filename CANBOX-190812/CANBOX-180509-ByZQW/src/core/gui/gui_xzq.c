/**
  *****************************************************************************
  * @file    gui_xzq.c
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
#include "gui_xzq.h"
#include "guilib.h"
#include "canfifo.h"

#include "hal_ebrd.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

enum {
	BTN_EXIT = 0,
	BTN_XZQ_AUTO,
	BTN_XZQ_HAND,
	BTN_NUM
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

tBtnWidget xzq_page_buttons[] =
{
	ButtonWidget(420, 5,  60,  30, 0x4000,  BTN_EXIT),
	ButtonWidget(1,  60,  80,  70, 0x4000, BTN_XZQ_AUTO),
	ButtonWidget(1,  60,  80,  70, 0x4000, BTN_XZQ_HAND),
};

static char xzq_btn_text[][12] = 
{
	"Exit",
	"XZQ",
	"XZQH"
};

static char xzq_test_on = 0;
static char xzq_test_mode = 0;
static char xzq_mask;
static char xzq_dao;

extern EBRD_INFO_t E481;
extern EBRD_INFO_t E534;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void gui_xzq_hand(int mode)
{
	int i;
	
	if (mode == 5)
		xzq_mask ^= 0x1F;
	else if (mode == 6)
		xzq_mask ^= 0x3F;
	else if (mode == 7)
		xzq_mask ^= 0x7F;
	else if (mode == 8)
		xzq_mask ^= 0xFF;
	else 
		return;
	
	for (i = 0; i < E481.xzqs; i++)
	{
		E481_exec_xzq(i, xzq_mask);
	}
	
	for (i = 0; i < E534.xzqs; i++)
	{
		E534_exec_xzq(i, xzq_mask);
	}
}


void gui_xzq_poll(int mode)
{
	static int ntime;

	static char prevmode = 0;
	unsigned int ctime;
	int i;

	ctime = arch_get_systicks();
	if (ntime > ctime)
		return;
	
	ntime = ctime + E481.xzq_tick;
	
	if (prevmode != mode)
	{
		if (xzq_mask)
		{
			if (xzq_mask & (1 << xzq_dao))
				xzq_mask ^= (0x1 << xzq_dao);//单刀循环
			xzq_dao++;
		}
		else
		{
			xzq_dao = 0;
			prevmode = mode;
		}
	}/*
	else if (prevmode == 0)	//全上全下
	{
		xzq_mask = xzq_mask ? 0 : 0xFF;
	}*/
	else if (prevmode == 1)
	{
		xzq_mask ^= (0x1 << xzq_dao);//单刀循环
		xzq_dao++;
	}
	else if (prevmode == 2)	//
	{
		xzq_mask ^= (0x3 << xzq_dao);
		xzq_dao += 2;
	}
	else if (prevmode == 3)
	{
		xzq_mask ^= (0x7 << xzq_dao);
		xzq_dao += 3;
	}
	else if (prevmode == 4)
	{
		xzq_mask ^= (0xF << xzq_dao);
		xzq_dao += 4;
	}
	else	// 一隔一
	{
		return;//xzq_mask = xzq_mask != 0x55 ? 0x55 : 0xaa;
	}
	
	if (xzq_dao >= 8)
	{
		xzq_dao = 0;
	}
	
	for (i = 0; i < E481.xzqs; i++)
	{
		E481_exec_xzq(i, xzq_mask);
	}
	
	for (i = 0; i < E534.xzqs; i++)
	{
		E534_exec_xzq(i, xzq_mask);
	}
}


/* Public functions ----------------------------------------------------------*/

void gui_xzq_process(void)
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

	GUI_DrawAscii(	5,	5, "XZQ Test", VM_COLOR_BLACK, VM_COLOR_BG);
	GUI_DrawAscii(430, 5, "Exit", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);

	for (i = 0; i < COUNTOF(xzq_page_buttons)-1; i++)
	{
		tBtnWidget *pbtn;

		w = 100;
		h = 60;
		x = 16 + (i%4)*(w+16);
		y = 30 + 40 + (i/4)*(h+40);
		
		GUI_DrawRect(x, y, w, h, VM_COLOR_BLUE, 0x0);
		
		pbtn = &xzq_page_buttons[i+1];
		pbtn->ha_x = x;
		pbtn->ha_y = y;
		pbtn->ha_w = w;
		pbtn->ha_h = h;

		chs = strlen(xzq_btn_text[i+1]);
		pt = chs * 8;
		pt = (w - pt)/2;

		GUI_DrawAscii(x+pt, y+h/2-10, xzq_btn_text[i+1], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	}

	xzq_test_on = 0;
	touchctl_HotArea_Register(xzq_page_buttons, COUNTOF(xzq_page_buttons));

	while (1)
	{
		if (xzq_test_on)
		{
			gui_xzq_poll(xzq_test_mode);
		}

		ret = system_event_pop(&evt, &sts);
		if (ret <= 0)
			continue;

		switch (evt)
		{
			case BTN_EXIT:{
				pageid = PAGE_ID_TEST_MENU;
				touchctl_HotArea_Dregister(xzq_page_buttons);
			}return;

			case BTN_XZQ_HAND:{
				xzq_test_on = 0;
				gui_xzq_hand(xzq_test_mode);
			}break;

			case BTN_XZQ_AUTO:{
				if (xzq_test_on)
				{
					xzq_test_mode++;
					if (xzq_test_mode == 9)
					{
						sprintf(xzq_btn_text[evt], "XZQ");
						xzq_test_on = 0;
						xzq_test_mode = 0;
						GUI_DrawRect(xzq_page_buttons[evt].ha_x+4, xzq_page_buttons[evt].ha_y+4, xzq_page_buttons[evt].ha_w-8, xzq_page_buttons[evt].ha_h-8, VM_COLOR_WHITE, 0x00);
					}
					else
					{
						sprintf(xzq_btn_text[evt], "M%d", xzq_test_mode);
					}
					
					{
						x = xzq_page_buttons[evt].ha_x;
						y = xzq_page_buttons[evt].ha_y;
						w = xzq_page_buttons[evt].ha_w;
						h = xzq_page_buttons[evt].ha_h;
						chs = strlen(xzq_btn_text[evt]);
						pt = chs * 8;
						pt = (w - pt)/2;
						
						GUI_DrawRect(x+5, y+5, w-10, h-10, VM_COLOR_WHITE, 0x80);
						GUI_DrawAscii(x+pt, y+h/2-10, xzq_btn_text[evt], VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
					}
				}
				else
				{
					xzq_test_on = 1;
					GUI_DrawRect(xzq_page_buttons[evt].ha_x+4, xzq_page_buttons[evt].ha_y+4, xzq_page_buttons[evt].ha_w-8, xzq_page_buttons[evt].ha_h-8, VM_COLOR_BLUE, 0x00);
				}
			}break;
		}
	}
	
}


