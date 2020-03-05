/**
  *****************************************************************************
  * @file    gui_ebrd.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui_ebrd.h"
#include "guilib.h"
#include "canfifo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define BTN_EVENT_EXIT					102
#define BTN_EVENT_PAGEUP				104
#define BTN_EVENT_PAGEDN				105
#define BTN_EVENT_CLEAN					103
#define BTN_EVENT_AUTO_TEST				100
#define BTN_EVENT_FN1					101
#define BTN_EVENT_FN2					106

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static tBtnWidget ebrd_buttons[] =
{
	ButtonWidget(420, 5,  60,  30, 0x0000,  BTN_EVENT_EXIT),
	ButtonWidget(1,  30,  80,  30, 0x0000,  BTN_EVENT_CLEAN),
	ButtonWidget(1,  60,  80,  70, 0x0000,  BTN_EVENT_AUTO_TEST),
	ButtonWidget(1, 130,  80,  70, 0x4000,  BTN_EVENT_FN1),
	ButtonWidget(1, 200,  80,  70, 0x4000,  BTN_EVENT_FN2),
	
	ButtonWidget(BTN_PGUP_X, BTN_PGUP_Y, BTN_PGUP_W, BTN_PGUP_H, 0x0000, BTN_EVENT_PAGEUP),
	ButtonWidget(BTN_PGDN_X, BTN_PGDN_Y, BTN_PGDN_W, BTN_PGDN_H, 0x0000, BTN_EVENT_PAGEDN),
};

static int ebrd_list_start = 0;
static char ebrd_loop_enable = 0;
static char ebrd_title[20] = "Exx Aged Test";

void (*pfnPoll)(void);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void btn_auto_test_deal(int type)
{
	if (type == 0)
	{
		ebrd_loop_enable = 0;
	}
	else
	{
		ebrd_loop_enable = ebrd_loop_enable ? 0 : 1;
	}
	
	if (ebrd_loop_enable)
	{
		GUI_DrawAscii(16, 83, "AutoOFF", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	}
	else
	{
		GUI_DrawAscii(16, 83, "AutoON ", VM_COLOR_RGB(0xFF, 0x00, 0x00), VM_COLOR_BG);
	}
}

/* Public functions ----------------------------------------------------------*/

void gui_ebrd_reg(char *title, void (*pfn)(void))
{
	if (title)
	{
		strncpy(ebrd_title, title, sizeof(ebrd_title));
	}
	pfnPoll = pfn;
}

void gui_ebrd_init()
{

}


void gui_show_ledshow()
{
	static unsigned int showledx=0;
	static unsigned long showdelay=0;
	unsigned long nc;
	unsigned char r=6;
	unsigned short x_d=0;

	nc= arch_get_systicks();
	if (showdelay>=nc)
	{
		return;
	}
	showdelay = nc+2000;
	
	GUI_DrawRect(2,148,476,24,VM_COLOR_BG,0x80);
	x_d = 2+r + (r<<2) * showledx;
	RA8875_DrawCircle(x_d, 160, r, VM_COLOR_GRY, 0x80);

	if ((x_d)>=480)
	{
		showledx=0;
	}
	else
		showledx++;
	
}

void gui_show_Vs_draw(unsigned int VS)
{
	static unsigned int ShowVs=0xffffffff;
	char buf[32];

	if (ShowVs==VS)
		return;
	ShowVs = VS;
	
	if (VS==0)
	{
		sprintf(buf,"V----.---");
	}
	else
		sprintf(buf,"V%1X%1X%1X%1X.%03d",(VS>>12)& 0X0F,(VS>>8)& 0X0F,(VS>>4)& 0X0F,(VS>>0)& 0X0F,(VS>>16)& 0XFFFF);

	GUI_DrawRect(280,32,200-2,28,VM_COLOR_BG,0x80);
	//RA8875_SetTextZoom(1, 1);
	GUI_DrawAscii(  280,  36, buf, VM_COLOR_BLACK, VM_COLOR_BG);
	//RA8875_SetTextZoom(0, 0);
}

void gui_show_12V_draw(uint16_t v12)
{
	static uint16_t show12v=0xffff;
	char buf[32];

	if (show12v==v12)
		return;
	show12v = v12;
	
	if (v12==0)
	{
		sprintf(buf,"--.- V");
	}
	else
		sprintf(buf,"%.1f V",v12/10.0);

	GUI_DrawRect(80,32,200,32,VM_COLOR_BG,0x80);
	RA8875_SetTextZoom(1, 1);
	GUI_DrawAscii(  80,  32, buf, VM_COLOR_BLACK, VM_COLOR_BG);
	RA8875_SetTextZoom(0, 0);

}

void gui_senser_draw(uint16_t stat)
{
	static uint16_t stat_bkp = 0;
	
	uint16_t msk;
	int i;
	int plus_y=0;
	
	if (stat == stat_bkp)
	{
		return;
	}

	msk = stat ^ stat_bkp;
	
	for (i = 0; i < 12; i++)
	{
		if (msk & (1 << i))
		{
			if (stat & (1 << i))
			{
				//RA8875_DrawCircle(160 + 60 * (i / 8), 45+MSG_LIST_H*(i % 8), 10, VM_COLOR_RED, 0x80);
				//RA8875_DrawCircle(160 + 60 * (i / 8), 45+MSG_LIST_H*(i % 8), 9, VM_COLOR_WHITE, 0x80);//ÃðµÆ
				RA8875_DrawCircle(80 + 40 * (i % 8), plus_y+80+40*(i / 8), 10, VM_COLOR_RED, 0x80);
				RA8875_DrawCircle(80 + 40 * (i % 8), plus_y+80+40*(i / 8), 9, VM_COLOR_WHITE, 0x80);//ÃðµÆ
			}
			else
			{
				RA8875_DrawCircle(80 + 40 * (i % 8), plus_y+80+40*(i / 8), 10, VM_COLOR_GREEN, 0x80);
			}
		}
	}
	stat_bkp = stat; 
}

void gui_senser_draw_ex(uint16_t stat,uint16_t wh,unsigned char notdraw )
{
	static uint16_t stat_bkp = 0;
	static uint16_t stat_bkp1 = 0;
	
	uint16_t msk;
	int i;
	int plus_y=wh?100:0;
	
	if (stat == (wh?stat_bkp1:stat_bkp))
	{
		return;
	}

	msk = stat ^ (wh?stat_bkp1:stat_bkp);
	
	for (i = 0; i < 10; i++)
	{
		if ((msk & (1 << i))&&(!notdraw))
		{
			if ((stat & (1 << i))==0)
			{
				//RA8875_DrawCircle(160 + 60 * (i / 8), 45+MSG_LIST_H*(i % 8), 10, VM_COLOR_RED, 0x80);
				//RA8875_DrawCircle(160 + 60 * (i / 8), 45+MSG_LIST_H*(i % 8), 9, VM_COLOR_WHITE, 0x80);//ÃðµÆ
				RA8875_DrawCircle(68 + 76 * (i % 5), plus_y+92+70*(i / 5), 6, VM_COLOR_RED, 0x80);
				RA8875_DrawCircle(68 + 76 * (i % 5), plus_y+92+70*(i / 5), 5, VM_COLOR_WHITE, 0x80);//ÃðµÆ
			}
			else
			{
				RA8875_DrawCircle(68 + 76 * (i % 5), plus_y+92+70*(i / 5), 6, VM_COLOR_GREEN, 0x80);
			}
		}
	}
	if(wh)
	{
		stat_bkp1 = stat; 
	}
	else
		stat_bkp = stat; 
}



void gui_ebrd_process(void)
{
	int i;
	int Msg_Top = -1;
	int Msg_Cnt = -1;

	GUI_Clr(VM_COLOR_BG);

	GUI_DrawRect(  1,  1, LCD_WIDTH-2, LCD_HEIGH-2, VM_COLOR_BLUE, 0x0);

	/*  title area */
	GUI_DrawLine(  1, 30, LCD_WIDTH-2,  30, VM_COLOR_BLUE);
	
	GUI_DrawLine(140,  1, 140, 30, VM_COLOR_BLUE);
	GUI_DrawLine(420,  1, 420, 30, VM_COLOR_BLUE);
	
	/*  */

	GUI_DrawLine( 80, 30,  80, 271, VM_COLOR_BLUE);
	
	GUI_DrawLine(  1, 60,  80, 60, VM_COLOR_BLUE);

	GUI_DrawLine(  1, 60,  6,  65, VM_COLOR_BLUE);
	GUI_DrawLine(  1,130,  6, 125, VM_COLOR_BLUE);
	GUI_DrawLine( 80, 60, 75,  65, VM_COLOR_BLUE);
	GUI_DrawLine( 80,130, 75, 125, VM_COLOR_BLUE);
	GUI_DrawRect(  6, 65, 69,  60, VM_COLOR_BLUE, 0x0);

	GUI_DrawLine(  1,130, 80, 130, VM_COLOR_BLUE);

	GUI_DrawLine(  1,130,  6, 135, VM_COLOR_BLUE);
	GUI_DrawLine(  1,200,  6, 195, VM_COLOR_BLUE);
	GUI_DrawLine( 80,130, 75, 135, VM_COLOR_BLUE);
	GUI_DrawLine( 80,200, 75, 195, VM_COLOR_BLUE);
	GUI_DrawRect(  6,135, 69,  60, VM_COLOR_BLUE, 0x0);

	GUI_DrawLine(  1,200, 80, 200, VM_COLOR_BLUE);

	GUI_DrawLine(  1,200,  6, 205, VM_COLOR_BLUE);
	GUI_DrawLine(  1,270,  6, 265, VM_COLOR_BLUE);
	GUI_DrawLine( 80,200, 75, 205, VM_COLOR_BLUE);
	GUI_DrawLine( 80,270, 75, 265, VM_COLOR_BLUE);
	GUI_DrawRect(  6,205, 69,  60, VM_COLOR_BLUE, 0x0);
	
	/* Draw MSG List Box */
	// col 
	GUI_DrawLine(MSG_LIST_X, MSG_LIST_Y,
		MSG_LIST_X, MSG_LIST_Y+MSG_LIST_H*MSG_LIST_NUM, VM_COLOR_BLUE);
	GUI_DrawLine(MSG_LIST_X+MSG_LIST_IDX_W, MSG_LIST_Y, 
		MSG_LIST_X+MSG_LIST_IDX_W, MSG_LIST_Y+MSG_LIST_H*MSG_LIST_NUM, VM_COLOR_BLUE);
	GUI_DrawLine(MSG_LIST_X+MSG_LIST_W, MSG_LIST_Y,
		MSG_LIST_X+MSG_LIST_W, MSG_LIST_Y+MSG_LIST_H*MSG_LIST_NUM, VM_COLOR_BLUE);

	// row
	for (i = 0; i <= MSG_LIST_NUM; i++)
	{
		GUI_DrawLine(MSG_LIST_X,  MSG_LIST_Y+MSG_LIST_H*(i), MSG_LIST_X + MSG_LIST_W, MSG_LIST_Y+MSG_LIST_H*(i), VM_COLOR_BLUE);
	}
	/* End Draw MSG List Box */

	/* Draw Button text */
	GUI_DrawAscii(  5,  5, ebrd_title, VM_COLOR_BLACK, VM_COLOR_BG);
	GUI_DrawAscii(425,  5, "Exit", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	
	GUI_DrawAscii( 16, 35, "Clean", VM_COLOR_RED, VM_COLOR_BG);
	
	GUI_DrawAscii( 16, 155, "Mode", VM_COLOR_RED, VM_COLOR_BG);
	GUI_DrawAscii( 16, 223, "Speed", VM_COLOR_RED, VM_COLOR_BG);

	GUI_DrawRect(BTN_PGUP_X,BTN_PGUP_Y, BTN_PGUP_W, BTN_PGUP_H, VM_COLOR_BLUE, 0x0);
	GUI_DrawAscii(BTN_PGUP_X+10, BTN_PGUP_Y+BTN_PGUP_H/2 - 8, "PgUp", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	GUI_DrawRect(BTN_PGDN_X,BTN_PGDN_Y, BTN_PGDN_W, BTN_PGDN_H, VM_COLOR_BLUE, 0x0);
	GUI_DrawAscii(BTN_PGDN_X+10, BTN_PGDN_Y+BTN_PGDN_H/2 - 8, "PgDn", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);

	touchctl_HotArea_Register(ebrd_buttons, COUNTOF(ebrd_buttons));

	btn_auto_test_deal(0);

	while (1)
	{
		int ret;
		int evt, sts;
		CAN_MSG_DEF *canmsg = NULL;

		if((Msg_Top != CanRecv_TopIdx()) ||
		   (Msg_Cnt != CanRecv_Count()))
		{
			int cnt;
			char buf[64];

			Msg_Top = CanRecv_TopIdx();
			Msg_Cnt =  CanRecv_Count();
			if(Msg_Cnt > MSG_LIST_NUM)
			{
				ebrd_list_start = Msg_Cnt - MSG_LIST_NUM;
			}
			else
				ebrd_list_start = 0;
			
		show_it:
			
			if (Msg_Cnt < MSG_LIST_NUM)
				cnt = Msg_Cnt;
			else
				cnt = MSG_LIST_NUM;
			
			sprintf(buf, "%-10d", CanRecv_Total());
			GUI_DrawAscii(145, 5, buf, VM_COLOR_BLACK, VM_COLOR_BG);
			/*
			for (i = 0; i < cnt; i++)
			{
				//CanRecv_FramePrint_loc(i + ebrd_list_start, buf);
				ret = CanRecv_pick(i + ebrd_list_start, &canmsg);
				if (ret < 0)
					continue;
				
				if (canmsg->msg_id)
				{
					sprintf(buf, "%-4d", i + ebrd_list_start + 1);
					GUI_DrawAscii(MSG_LIST_IDX_X+5, MSG_LIST_IDX_Y+MSG_LIST_IDX_H*(i) + 4, buf, VM_COLOR_RGB(0xFF, 0x00, 0x00), VM_COLOR_BG);
				
					CanRecv_FramePrint_loc(i + ebrd_list_start, buf);
					GUI_DrawAscii(MSG_LIST_X+MSG_LIST_IDX_W+5, MSG_LIST_Y+MSG_LIST_H*(i) + 4, buf, VM_COLOR_RGB(0xFF, 0x00, 0x00), VM_COLOR_BG);
				}
			}
			*/
		}
		
		if (ebrd_loop_enable)
		{
			if (pfnPoll)
				pfnPoll();
		}

		ret = system_event_pop(&evt, &sts);
		if(ret <= 0)
			continue;

		switch(evt) {
			
		case BTN_EVENT_EXIT:
			pageid = PAGE_ID_TEST_MENU;
			goto exit;

		case BTN_EVENT_PAGEUP:
			if(ebrd_list_start + MSG_LIST_NUM <= Msg_Cnt - MSG_LIST_NUM) {
				ebrd_list_start += MSG_LIST_NUM;
			}
			else if(ebrd_list_start + MSG_LIST_NUM < Msg_Cnt) {
				ebrd_list_start = Msg_Cnt - MSG_LIST_NUM;
			}
			else
				ebrd_list_start = 0;
			goto show_it;
			
		case BTN_EVENT_PAGEDN:
			if(ebrd_list_start >= MSG_LIST_NUM)
				ebrd_list_start -= MSG_LIST_NUM;
			else if(ebrd_list_start > 0)
				ebrd_list_start = 0;
			else if(Msg_Cnt > MSG_LIST_NUM) {
				ebrd_list_start = Msg_Cnt - MSG_LIST_NUM;
			}
			else
				ebrd_list_start = 0;
			goto show_it;
			
		case BTN_EVENT_AUTO_TEST:
			btn_auto_test_deal(1);
			break;
			
		case BTN_EVENT_FN1:
			
			break;
		case BTN_EVENT_FN2:
			
			break;
			
		case BTN_EVENT_CLEAN:
			CanRecv_Clear();
			for (i = 0; i < MSG_LIST_NUM; i++)
			{
				GUI_DrawRect(MSG_LIST_X+1, MSG_LIST_Y + MSG_LIST_H*(i) + 1,
							MSG_LIST_IDX_W-2, MSG_LIST_IDX_H-2, VM_COLOR_BG, 0x80);
				GUI_DrawRect(MSG_LIST_X+MSG_LIST_IDX_W+1, MSG_LIST_Y + MSG_LIST_H*(i) + 1,
							MSG_LIST_W-MSG_LIST_IDX_W-2, MSG_LIST_H-2, VM_COLOR_BG, 0x80);
			}
			break;
		}
	}
	
exit:
	touchctl_HotArea_Dregister(ebrd_buttons);
	pfnPoll = NULL;
}


