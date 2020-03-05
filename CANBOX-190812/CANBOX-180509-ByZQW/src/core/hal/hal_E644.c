/**
  *****************************************************************************
  * @file    hal_E644.c
  * @author  HongLC
  * @version V1.0.0
  * @date    2018-06-16
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/
  
/* Includes ------------------------------------------------------------------*/
#include "hal_E644.h"
#include "hal_can.h"
#include "hal_ebrd.h"
#include "canfifo.h"


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

static EXX_CMD_t *pe644_cmd = &ebrd_cmd;
EBRD_INFO_t E644;

static char xzq_mask;
static char xzq_dao;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/



void E644_read_12V()
{
	pe644_cmd->type = CMDTYPE_PERIPHERAL;
	pe644_cmd->data = CMD_GET_AD_CUR;
	pe644_cmd->arg1.word = 2;
	hal_can_send(BRD_E644_CANID, (uint8_t *)pe644_cmd, 4);
}



void E644_Set_Workmode()
{
	pe644_cmd->type = CMDTYPE_DCT;
	pe644_cmd->data = CMD_DCT_E644MODE;
	pe644_cmd->arg1.word = 0;		//0-1，1-0 都要回复
	pe644_cmd->arg2.word = 1;		// 1-0 的时候不需要上报时间间隔
	hal_can_send(BRD_E644_CANID, (uint8_t *)pe644_cmd, 6);
}

void E644_check_online(uint32_t msgid)
{
	pe644_cmd->type = CMDTYPE_OTHERSET;
	pe644_cmd->data = CMD_OTHERSET_GET_VER;
	hal_can_send(msgid, (uint8_t *)pe644_cmd, 2);
}

void E644_read_Vision()
{
	E644_check_online(BRD_E644_CANID);
}


/*----------------------------------------------------------------------------*/



void hal_e644_input_poll(unsigned char isfirst)
{
	static uint8_t init = 0;
	unsigned long tMsgID;
	unsigned char data[8];
	static unsigned long ntime;
	static unsigned long v12time=0;
	static unsigned long v12timeshow=0;
	static unsigned long HaveData=0;
	static unsigned long GetVercnt=0;	
	unsigned long ctime;
	unsigned char needsetworkmode=0;
	static unsigned short V12=0x00;
	static unsigned int Vs=0x0000;
	

	if (isfirst)
	{
		init =0;
		ntime =0;
		v12time =0;
		HaveData =0;
		V12 =0;
		GetVercnt =0;
		Vs = E644.Version_32;
	}
	
	if (init < 2)
	{
		if (init == 0)
		{	
			gui_senser_draw_ex(0xFFFF,0,1);
			gui_senser_draw_ex(0xFFFF,1,1);	
			gui_show_12V_draw(0xffff);
			gui_show_Vs_draw(0xffffffff);
		}
		else
		{
			gui_senser_draw_ex(0,0,0);
			gui_senser_draw_ex(0,1,0);
			gui_show_12V_draw(0);
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
		if(tMsgID == BRD_E644_CANID_RX )
		{
			switch (data[0])
			{
				case CMDTYPE_PERIPHERAL:
					if (data[1] == CMD_GET_AD_CUR)
					{
						V12 = (unsigned short)data[2]|((unsigned short)data[3]<<8);
						v12timeshow = ctime+20000;// 2S钟不刷新视为无数据 显示--.-V
					}
					break;
				case CMDTYPE_OTHERSET:
					if (data[1] == CMD_OTHERSET_RETURN_YARN_ZERO_ST)
					{
						char w;
						unsigned short ic;
						needsetworkmode = data[6]?1:0;
						w = data[7] & 0x01;
						ic = w?(data[5] << 8 | data[4]):(data[3] << 8 | data[2]);
						gui_senser_draw_ex(ic,w,0);
					}
					else
					if(data[1] == CMD_OTHERSET_GET_VER)
					{
						Vs = data[2] |(data[3]<<8)|(data[4]<<16);
					}						
					break;
					

			}		

			HaveData = ctime+20000;
			
		}	
	}

	if (v12timeshow<ctime)
	{
		V12 =0;
	}
	gui_show_12V_draw(V12);
	
	if(needsetworkmode)
	{
		E644_Set_Workmode();
	}
	
	if (v12time<ctime)
	{
		E644_read_12V();
		v12time=ctime+10000;//间隔1S之后获取一次12V电压值
	}
	if ((HaveData<ctime)||(Vs==0))
	{
		Vs=0;
		if(GetVercnt<ctime)
		{
			E644_read_Vision();
			GetVercnt = ctime+13000;//如果没数据，那就每隔1.3秒联系一次。
		}		
	}
	gui_show_Vs_draw(Vs);

	//gui_show_ledshow();
	
}





int hal_e644_check(void)
{
	int ret;
	uint16_t canid;
	long ctime, ntime;
	
	canid = BRD_E644_CANID;
	
	CanRecv_Clear();
	E644_check_online(canid);
	
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
		E644.cantxid = canid;
		E644.online = 1;
		E644.sins =32;/*输入信号32个*/
	
		strcpy(E644.name, "E644 Board Test");
		
		{
			int i;
			CAN_MSG_DEF *canmsg = NULL;

			E644.sysno = 0;
			arch_delay_ms(50);
			for (i = 0; i < ret; i++)
			{
				ret = CanRecv_pick(i, &canmsg);
				if ((canmsg->msg_dat[0] == CMDTYPE_OTHERSET)&&(canmsg->msg_dat[1] == CMD_OTHERSET_GET_VER))
				{
					E644.Version_32 = canmsg->msg_dat[2]|(canmsg->msg_dat[3]<<8)|(canmsg->msg_dat[4]<<16);
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


void hal_E644_init(void)
{
	gui_ebrd_reg(E644.name, NULL);
}

void hal_E644_process(void)
{	
	hal_E644_init();
	gui_input_process_E644();

	
}


