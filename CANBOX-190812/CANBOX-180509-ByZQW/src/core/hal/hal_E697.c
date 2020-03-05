/**
  *****************************************************************************
  * @file    hal_E697.c
  * @author  HongLC
  * @version V1.0.0
  * @date    2019-04-17
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/
  
/* Includes ------------------------------------------------------------------*/
#include "hal_E697.h"
#include "hal_can.h"
#include "hal_ebrd.h"
#include "canfifo.h"
#include "guilib.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MOTOR_TYPE_UNDEF	(0)			//未定义 
#define MOTOR_TYPE_DENSITY	(1)		//度目
#define MOTOR_TYPE_SKINER	(2)			//申克
#define MOTOR_TYPE_ACTION	(3)			//动作简易型
#define MOTOR_TYPE_FEET	(4)			//压脚
#define MOTOR_TYPE_COUNT	(MOTOR_TYPE_FEET)			//所有电机类型数量

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static EXX_CMD_t *pe697_cmd = &ebrd_cmd;
EBRD_INFO_t E697;

static char xzq_mask;
static char xzq_dao;
static char emf_st_set[10]={0,0,0,0,0,0,0,0,0,0};
static char auto_test_do =0;

enum {
	BTN_EXIT = 0,
	BTN_OUT_1,
	BTN_OUT_2,
	BTN_OUT_3,
	BTN_OUT_4,
	BTN_OUT_5,
	BTN_OUT_6,
	BTN_OUT_7,
	BTN_OUT_8,
	BTN_OUT_9,
	BTN_OUT_10,
	BTN_OUT_AUTO,
	
};

tBtnWidget btn_E697_menu[] =
{
	ButtonWidget(420, 5,  60,  30, 0x4000,  PAGE_ID_MENU),
		
	ButtonWidget(1,  50,  80,  50, 0x4000,  BTN_OUT_1),
	ButtonWidget(1,  50,  80,  50, 0x4000,  BTN_OUT_2),
	ButtonWidget(1,  50,  80,  50, 0x4000,  BTN_OUT_3),
	ButtonWidget(1,  50,  80,  50, 0x4000,  BTN_OUT_4),
	
	ButtonWidget(1, 130,  80,  50, 0x4000,  BTN_OUT_5),
	ButtonWidget(1, 130,  80,  50, 0x4000,  BTN_OUT_6),
	ButtonWidget(1, 130,  80,  50, 0x4000,  BTN_OUT_7),
	ButtonWidget(1, 130,  80,  50, 0x4000,  BTN_OUT_8),

	ButtonWidget(1, 130,  80,  50, 0x4000,  BTN_OUT_9),
	ButtonWidget(1, 130,  80,  50, 0x4000,  BTN_OUT_10),
	
	ButtonWidget(1, 130,  80,  50, 0x4000,  BTN_OUT_AUTO),
	
	
};

char btn_E697_text[][20] = 
{
	"Exit",
	"Out_1",
	"Out_2",
	"Out_3",
	"Out_4",
	"Out_5",
	"Out_6",
	"Out_7",
	"Out_8",
	"Out_9",	
	"Out_10",	
	"AUTO_START",	
	
};


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void gui_input_process_E697(void);


void E697_read_12V()
{
	pe697_cmd->type = CMDTYPE_PERIPHERAL;
	pe697_cmd->data = CMD_GET_AD_CUR;
	pe697_cmd->arg1.word = 2;
	hal_can_send(BRD_E697_CANID, (uint8_t *)pe697_cmd, 4);
}



void E697_Set_Workmode()
{
	pe697_cmd->type = CMDTYPE_DCT;
	pe697_cmd->data = CMD_DCT_E644MODE;
	pe697_cmd->arg1.word = 0;		//0-1，1-0 都要回复
	pe697_cmd->arg2.word = 1;		// 1-0 的时候不需要上报时间间隔
	hal_can_send(BRD_E697_CANID, (uint8_t *)pe697_cmd, 6);
}

void E697_check_online(uint32_t msgid)
{
	pe697_cmd->type = CMDTYPE_OTHERSET;
	pe697_cmd->data = CMD_OTHERSET_GET_VER;
	hal_can_send(msgid, (uint8_t *)pe697_cmd, 2);
}

void E697_read_Vision()
{
	E697_check_online(BRD_E697_CANID);
}

void E697_do_emf_exec(unsigned char whichid,unsigned char onoff)
{
	pe697_cmd->type = CMDTYPE_E697;
	pe697_cmd->data = 0x01;
	pe697_cmd->arg1.byte.l = 0x00;
	pe697_cmd->arg1.byte.h = whichid;
	pe697_cmd->arg2.byte.l = onoff?1:0;
	pe697_cmd->arg2.byte.h = 0x32;/* 50 ms*/
	pe697_cmd->arg3.word = 0;
	
	hal_can_send(BRD_E697_CANID, (uint8_t *)pe697_cmd, 8);
}


/*----------------------------------------------------------------------------*/



void hal_e697_input_poll(unsigned char isfirst)
{
	static uint8_t init = 0;
	unsigned long tMsgID;
	unsigned char data[8];
	static unsigned long ntime;
	static unsigned long v12time=0;
	static unsigned long v12timeshow=0;
	static unsigned long HaveData=0;
	static unsigned long GetVercnt=0;	
	static unsigned long Autodoemftimer=0;	
	
	unsigned long ctime;
	unsigned char needsetworkmode=0;
	static unsigned short V12=0x00;
	static unsigned int Vs=0x0000;
	static char nextid=0;
	

	if (isfirst)
	{
		init =0;
		ntime =0;
		v12time =0;
		HaveData =0;
		V12 =0;
		GetVercnt =0;
		Vs = E697.Version_32;
		nextid =0;
	}
	
	if (init < 2)
	{
		if (init == 0)
		{	
			gui_senser_draw_ex(0xFFFF,0,1);
			//gui_senser_draw_ex(0xFFFF,1,1);	
			//gui_show_12V_draw(0xffff);
			gui_show_Vs_draw(0xffffffff);
		}
		else
		{
			gui_senser_draw_ex(0,0,0);
			//gui_senser_draw_ex(0,1,0);
			//gui_show_12V_draw(0);
			gui_show_Vs_draw(0);
		}
		init++;
	}
	
	ctime = arch_get_systicks();
	if (ntime > ctime)
	{
		return;
	}
	ntime = ctime + 10;
	if (CanRecv_Count())
	{
		CanRecv_Data(&tMsgID, data);
		if(tMsgID == BRD_E697_CANID_RX )
		{
			switch (data[0])
			{
				case CMDTYPE_OTHERSET:
					if(data[1] == CMD_OTHERSET_GET_VER)
					{
						Vs = data[2] |(data[3]<<8)|(data[4]<<16);
					}						
					break;
			}
			HaveData = ctime+20000;
			
		}	
	}

	//gui_show_12V_draw(V12);
#if 0
	if(needsetworkmode)
	{
		E697_Set_Workmode();
	}

	if (v12time<ctime)
	{
		E697_read_12V();
		v12time=ctime+10000;//间隔1S之后获取一次12V电压值
	}
#endif
	if ((HaveData<ctime)||(Vs==0))
	{
		Vs=0;
		if(GetVercnt<ctime)
		{
			E697_read_Vision();
			GetVercnt = ctime+13000;//如果没数据，那就每隔1.3秒联系一次。
		}		
	}
	gui_show_Vs_draw(Vs);

	if((auto_test_do)&&(Autodoemftimer<ctime))
	{
		//E697_do_emf_exec(nextid,)

		tBtnWidget *pbtn;	
		pbtn = &btn_E697_menu[BTN_OUT_AUTO];										
		GUI_DrawRect(pbtn->ha_x, pbtn->ha_y, pbtn->ha_w, pbtn->ha_h, (nextid & 0x01)?VM_COLOR_BLUE:VM_COLOR_GREEN, 0x0);

		pbtn = &btn_E697_menu[nextid+1];										
		GUI_DrawRect(pbtn->ha_x, pbtn->ha_y, pbtn->ha_w, pbtn->ha_h, emf_st_set[nextid]?VM_COLOR_BLUE:VM_COLOR_GREEN, 0x0);
					
		
		E697_do_emf_exec(nextid,emf_st_set[nextid]);
		emf_st_set[nextid] =!emf_st_set[nextid];
		nextid++;	
		if(nextid>=10)
		{
			nextid =0;
		}
		Autodoemftimer =ctime+3000;  //100 ms 
	}

	//gui_show_ledshow();
	
}





int hal_e697_check(void)
{
	int ret;
	uint16_t canid;
	long ctime, ntime;
	
	canid = BRD_E697_CANID;
	
	CanRecv_Clear();
	E697_check_online(canid);
	
	ntime = arch_get_systicks() + 2000;
	while (1)
	{
		ret = CanRecv_Total();
		if (ret)
			break;
		
		ctime = arch_get_systicks();
		if (ctime > ntime)
			break;
	}
	
	//if (ret)
	{
		E697.cantxid = canid;
		E697.online = 1;
		E697.sins =32;/*输入信号32个*/
	
		strcpy(E697.name, "E697 Board Test");
		
		{
			int i;
			CAN_MSG_DEF *canmsg = NULL;

			E697.sysno = 0;
			arch_delay_ms(50);
			for (i = 0; i < ret; i++)
			{
				ret = CanRecv_pick(i, &canmsg);
				if ((canmsg->msg_dat[0] == CMDTYPE_OTHERSET)&&(canmsg->msg_dat[1] == CMD_OTHERSET_GET_VER))
				{
					E697.Version_32 = canmsg->msg_dat[2]|(canmsg->msg_dat[3]<<8)|(canmsg->msg_dat[4]<<16);
				}				
			}
		}
	}
	/*
	else
	{
		strcpy(E481.name, "E644 No find");
	}*/
	return ret;
}


void hal_E697_init(void)
{
	gui_ebrd_reg(E697.name, NULL);
}

void hal_E697_process(void)
{	
	hal_E697_init();
	gui_input_process_E697();

	
}


void gui_input_process_E697(void)
{

	int i;
	int x,y,w,h;
	int pt, chs;
	int ret;
	int evt, sts;	
	char last_evi=0;
	char isautorun=0;

	GUI_Clr(VM_COLOR_BG);
	GUI_DrawRect(  1,  1, LCD_WIDTH-2, LCD_HEIGH-2, VM_COLOR_BLUE, 0x0);

	/*  title area */
	GUI_DrawLine(  1, 30, LCD_WIDTH-2,  30, VM_COLOR_BLUE);
	GUI_DrawLine(420,  1, 420, 30, VM_COLOR_BLUE);

	GUI_DrawAscii(	5,	5, "E697 Auto Test", VM_COLOR_BLACK, VM_COLOR_BG);
	GUI_DrawAscii(430, 5, "Exit", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);

	for (i = 0; i < COUNTOF(btn_E697_menu)-1; i++)
	{
		tBtnWidget *pbtn;		
		
		w = ( i == (COUNTOF(btn_E697_menu)-2))?120:60;
		h = 40;
		x = 16 + (i%5)*(w+16);
		y = 30 + 30 + (i/5)*(h+30);

		GUI_DrawRect(x, y, w, h, ( i == (COUNTOF(btn_E697_menu)-2))?VM_COLOR_BLUE:((emf_st_set[i]==0 )?VM_COLOR_BLUE:VM_COLOR_GREEN), 0x0);
		
		//GUI_DrawRect(x, y, w, h, (emf_st_set[i]==0 )?VM_COLOR_BLUE:VM_COLOR_GREEN, 0x0);
		
		pbtn = &btn_E697_menu[i+1];
		pbtn->ha_x = x;
		pbtn->ha_y = y;
		pbtn->ha_w = w;
		pbtn->ha_h = h;

		chs = strlen(btn_E697_text[i+1]);
		pt = chs * 8;
		pt = (w - pt)/2;

		GUI_DrawAscii(x+pt, y+h/2-10, btn_E697_text[i+1], VM_COLOR_RGB(0x0F, 0x0F, 0x8F), VM_COLOR_BG);
		
	}

	//GUI_DrawRect(16, 300, 120, 40, VM_COLOR_BLUE, 0x0);
	

	touchctl_HotArea_Register(btn_E697_menu, COUNTOF(btn_E697_menu));
	//E481_step_son(0);

	

	hal_e697_input_poll(1);
	while (1)
	{
		hal_e697_input_poll(0);
		
		ret = system_event_pop(&evt, &sts);
		if (ret <= 0)
			continue;

		switch (evt)
		{
			case BTN_OUT_1:
			case BTN_OUT_2:
			case BTN_OUT_3:
			case BTN_OUT_4:
			case BTN_OUT_5:
			case BTN_OUT_6:
			case BTN_OUT_7:
			case BTN_OUT_8:
			case BTN_OUT_9:
			case BTN_OUT_10:
				if(!isautorun)
				{
					
					int ei = evt-1;	
					tBtnWidget *pbtn;	
					pbtn = &btn_E697_menu[ei+1];										
					GUI_DrawRect(pbtn->ha_x, pbtn->ha_y, pbtn->ha_w, pbtn->ha_h, emf_st_set[ei]?VM_COLOR_BLUE:VM_COLOR_GREEN, 0x0);
					
					E697_do_emf_exec(ei,emf_st_set[ei]);
					emf_st_set[ei] =!emf_st_set[ei];
					last_evi = evt;
				}
				break;
			case BTN_OUT_AUTO:
				{
					if(isautorun)
					{
						tBtnWidget *pbtn;	
						pbtn = &btn_E697_menu[BTN_OUT_AUTO];										
						GUI_DrawRect(pbtn->ha_x, pbtn->ha_y, pbtn->ha_w, pbtn->ha_h,VM_COLOR_BLUE, 0x0);
						
						auto_test_do=0;
						isautorun =0;
						
					}				
					else
					{
						auto_test_do =1;
						isautorun =1;
					}
				}
				break;
			case BTN_EXIT:{
				pageid = PAGE_ID_TEST_MENU;
				isautorun =0;
				auto_test_do=0;
				touchctl_HotArea_Dregister(btn_E697_menu);
			}return;
		}
	}
}



