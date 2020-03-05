/**
  *****************************************************************************
  * @file    hal_E481.c
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
#include "hal_E481.h"
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

static EXX_CMD_t *pe481_cmd = &ebrd_cmd;
EBRD_INFO_t E481;

static char xzq_mask;
static char xzq_dao;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void E481_read_input()
{
	pe481_cmd->type = CMDTYPE_PERIPHERAL;
	pe481_cmd->data = CMD_GET_SIG_PH;
	pe481_cmd->arg1.word = 0;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe481_cmd, 4);
}

void E481_power_ctrl(int onoff)
{
	pe481_cmd->type = CMDTYPE_PERIPHERAL;
	pe481_cmd->data = CMD_POWER;
	pe481_cmd->arg1.word = onoff ? 1 : 0;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe481_cmd, 4);
	E481.power = onoff;
}

void E481_step_son(int son)
{
	pe481_cmd->type = CMDTYPE_PERIPHERAL;
	pe481_cmd->data = CMD_STEP_ENABLE;
	pe481_cmd->arg1.word = son;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe481_cmd, 4);
	E481.son = son;
}

void E481_step_current(int step_mA)
{
	E481.mot_ma = step_mA;
	
	pe481_cmd->type = CMDTYPE_PERIPHERAL;
	pe481_cmd->data = CMD_STEP_WORK_CURRENT;
	pe481_cmd->arg1.word = 0;		//DENSTITY
	pe481_cmd->arg2.word = step_mA;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe481_cmd, 6);
	pe481_cmd->arg1.word = 1;		//
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe481_cmd, 6);
	
	///SINKER STEP
	pe481_cmd->type = CMDTYPE_PERIPHERAL;
	pe481_cmd->data = CMD_SET_PWM_DA_OR_DA;
	pe481_cmd->arg1.word = 0x0001;
	pe481_cmd->arg2.word = step_mA;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe481_cmd, 6);
}

void E481_exec_step(int idx, unsigned short pos)
{
	pe481_cmd->type = CMDTYPE_MOTOR_DENSITY;
	pe481_cmd->data = CMD_DENSITY_STEP_ISR;
	pe481_cmd->arg1.byte.l = idx;
	pe481_cmd->arg1.byte.h = 1;
	pe481_cmd->arg2.word = pos;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe481_cmd, 6);
}

void E481_exec_step_other(int type, int idx, unsigned short pos)
{
	pe481_cmd->type = CMDTYPE_MOTOR_OTHER;
	pe481_cmd->data = CMD_OTHER_ISR;
	pe481_cmd->arg1.byte.l = type;
	pe481_cmd->arg1.byte.h = idx;
	pe481_cmd->arg2.word = pos;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe481_cmd, 6);
}

/* 
默认机头二三系统关闭生克电机，所以需要开启
*/
void E481_step_active(void)
{
	pe481_cmd->type = CMDTYPE_MOTOR_DENSITY;
	pe481_cmd->data = CMD_DENSITY_SET_ATTRIBUTE;
	pe481_cmd->arg1.word = 0;
	pe481_cmd->arg2.word = 0xFFFF;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe481_cmd, 6);
}

/*----------------------------------------------------------------------------*/

void E481_exec_xzq(int idx, unsigned short mask)
{
	pe481_cmd->type = CMDTYPE_XZQ;
	pe481_cmd->data = CMD_XZQ_TEST_ISR;
	pe481_cmd->arg1.word = idx;
	pe481_cmd->arg2.word = mask;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe481_cmd, 6);
}

void E481_exec_emf(int idx, int onoff)
{
	pe481_cmd->type = CMDTYPE_DCT;
	pe481_cmd->data = CMD_DCT_ISR;
	pe481_cmd->arg1.byte.l = 1;
	pe481_cmd->arg1.byte.h = idx;
	pe481_cmd->arg2.word = onoff;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe481_cmd, 6);
}

void E481_check_online(uint32_t msgid)
{
	pe481_cmd->type = GET_VER_ISR;
	hal_can_send(msgid, (uint8_t *)pe481_cmd, 1);
}

/*----------------------------------------------------------------------------*/

void hal_E481_emf_poll(int mode)
{
	static unsigned long ntime;
	static short val;
	static short i = 0;
	unsigned long ctime;
	
	ctime = arch_get_systicks();
	if (ntime > ctime)
		return;
	
	ntime = ctime + E481.emf_tick;
	for (i = 0; i < E481.emfs; i++)
	{
		E481_exec_emf(i, val);
	}
	
		val = val ? 0 : 0xFFFF;
	/*
	i++;
	if (i >= E481.emfs)
	{
		i = 0;
		val = val ? 0 : 0xFFFF;
	}
	*/
}

void hal_E481_xzq_poll(int mode)
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
	}
	else if (prevmode == 0)	//全上全下
	{
		xzq_mask = xzq_mask ? 0 : 0xFF;
	}
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
		xzq_mask = xzq_mask != 0x55 ? 0x55 : 0xaa;
	}
	
	if (xzq_dao >= 8)
	{
		xzq_dao = 0;
	}
	
	for (i = 0; i < E481.xzqs; i++)
	{
		E481_exec_xzq(i, xzq_mask);
		arch_delay_ms(10);
	}
}

void hal_E481_xzq_hand(int mode)
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
}

void hal_e481_input_poll(unsigned char isfirst)
{
	static uint8_t init = 0;
	unsigned long tMsgID;
	unsigned char data[8];
	static unsigned long ntime;
	unsigned long ctime;

	if (isfirst)
	{
		init =0;
		ntime =0;
	}

	
	if (init < 2)
	{
		if (init == 0)
			gui_senser_draw(0xFFFF);
		else
			gui_senser_draw(0);
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
		if (data[0] == CMDTYPE_PERIPHERAL && data[1] == CMD_GET_SIG_PH)
		{
			gui_senser_draw(data[3] << 8 | data[2]);
		}
	}
	else
	{
		E481_read_input();
	}
}

void hal_E481_mot_poll(int mode)
{
	static unsigned long ntime;
	static short pos;
	unsigned long ctime;
	int i;

	ctime = arch_get_systicks();
	if (ntime > ctime)
		return;

	if (E481.mot_tick < 500)
		E481.mot_tick = 500;
	
	ntime = ctime + E481.mot_tick;
	pos = pos ? 0 : 700;
	
	for (i = 0; i < E481.mots; i++)
	{
		E481_exec_step(i, pos);
	}
	/*
	for (i = 0; i < 4; i++)
	{
		E481_exec_step_other(0, i, pos);	//sinker
		E481_exec_step_other(1, i, pos);	//action
		E481_exec_step_other(2, i, pos);	//feet
	}
	*/
}

void hal_E481_poll(void)
{
	hal_E481_mot_poll(0);
	hal_E481_emf_poll(0);
	hal_E481_xzq_poll(0);
	hal_E459_trig_poll(0);
}


int hal_e481_check(void)
{
	int ret;
	uint16_t canid;
	long ctime, ntime;
	
	canid = BRD_EBRD_CANID;
	
	CanRecv_Clear();
	E481_check_online(canid);
	
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
		E481.cantxid = canid;
		E481.online = 1;
		E481.emfs = 40;
		E481.xzqs = 8;
		E481.mots = 6;

		E481.emf_mode = 0;
		E481.xzq_mode = 1;
		E481.mot_mode = 0;
		E481.power = 0;
		E481.son = 0;
		E481.mot_ma = 800;
		E481.cancomid = 0;
		
		E481.xzq_tick = 1000;
		E481.emf_tick = 1000;
		E481.mot_tick = 5000;
		strcpy(E481.name, "E481 Aged Test");
		
		{
			int i;
			CAN_MSG_DEF *canmsg = NULL;

			E481.sysno = 0;
			arch_delay_ms(200);
			for (i = 0; i < ret; i++)
			{
				ret = CanRecv_pick(i, &canmsg);
				if (canmsg->msg_dat[0] == CMDTYPE_CHECKONLINE)
				{
					E481.sysno |= (1 << canmsg->msg_dat[1]);
				}
			}
		}
	}
	/*
	else
	{
		strcpy(E481.name, "E481 No find");
	}*/
	return ret;
}


void hal_E481_init(void)
{
	E481_power_ctrl(1);
	
	E481_step_current(800);
	
	E481_step_son(1);
	E459_step_son(1);
	E481_step_active();
	
	gui_ebrd_reg(E481.name, hal_E481_poll);
}

void hal_E481_process(void)
{
	hal_E481_init();
	gui_ebrd_process();
}


