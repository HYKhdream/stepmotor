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
#include "hal_E438.h"
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

static EXX_CMD_t *pe438_cmd = &ebrd_cmd;
EBRD_INFO_t E438;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void E438_power_ctrl(int onoff)
{
	pe438_cmd->type = CMDTYPE_PERIPHERAL;
	pe438_cmd->data = CMD_POWER;
	pe438_cmd->arg1.word = onoff ? 1 : 0;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe438_cmd, 4);
	E438.power = onoff;
}

void E438_step_son(int son)
{
	pe438_cmd->type = CMDTYPE_PERIPHERAL;
	pe438_cmd->data = CMD_STEP_ENABLE;
	pe438_cmd->arg1.word = son;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe438_cmd, 4);
	E438.son = son;
}

void E438_step_current(int step_mA)
{
	E438.mot_ma = step_mA;
	
	pe438_cmd->type = CMDTYPE_PERIPHERAL;
	pe438_cmd->data = CMD_STEP_WORK_CURRENT;
	pe438_cmd->arg1.word = 0;
	pe438_cmd->arg2.word = step_mA;	//800ma
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe438_cmd, 6);

	arch_delay_ms(2);

	pe438_cmd->data = CMD_SET_PWM_DA_OR_DA;
	pe438_cmd->arg1.word = 1;	//ACT
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe438_cmd, 6);
	
	arch_delay_ms(2);

	pe438_cmd->data = CMD_SET_PWM_DA_OR_DA;
	pe438_cmd->arg1.word = 2;	//LEFT
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe438_cmd, 6);
}

void E438_dync_step(int idx, unsigned short pos, unsigned short time, unsigned char maxspd)
{
	pe438_cmd->type = CMDTYPE_MOTOR_DENSITY;
	pe438_cmd->data = CMD_DENSITY_STEP_ISR_DD;
	pe438_cmd->arg1.byte.l = idx;
	pe438_cmd->arg1.byte.h = maxspd & 0x7F;
	pe438_cmd->arg2.word = pos;
	pe438_cmd->arg3.word = time;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe438_cmd, 8);
}

void E438_exec_step(int idx, unsigned short pos)
{
	pe438_cmd->type = CMDTYPE_MOTOR_DENSITY;
	pe438_cmd->data = CMD_DENSITY_STEP_ISR;
	pe438_cmd->arg1.byte.l = idx;
	pe438_cmd->arg1.byte.h = 1;
	pe438_cmd->arg2.word = pos;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe438_cmd, 6);
}

void E438_exec_step_other(int type, int idx, unsigned short pos)
{
	pe438_cmd->type = CMDTYPE_MOTOR_OTHER;
	pe438_cmd->data = CMD_OTHER_ISR;
	pe438_cmd->arg1.byte.l = type;
	pe438_cmd->arg1.byte.h = idx;
	pe438_cmd->arg2.word = pos;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe438_cmd, 6);
}

/* 
默认机头二三系统关闭生克电机，所以需要开启
*/
void E438_step_active(void)
{
	pe438_cmd->type = CMDTYPE_MOTOR_DENSITY;
	pe438_cmd->data = CMD_DENSITY_SET_ATTRIBUTE;
	pe438_cmd->arg1.word = 0;
	pe438_cmd->arg2.word = 0xFFFF;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe438_cmd, 6);
}

/*----------------------------------------------------------------------------*/

void E438_exec_xzq(int idx, unsigned short mask)
{
	pe438_cmd->type = CMDTYPE_XZQ;
	pe438_cmd->data = CMD_XZQ_TEST_ISR;
	pe438_cmd->arg1.word = idx;
	pe438_cmd->arg2.word = mask;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe438_cmd, 6);
}

void E438_exec_emf(int idx, int onoff)
{
	pe438_cmd->type = CMDTYPE_DCT;
	pe438_cmd->data = CMD_DCT_ISR;
	pe438_cmd->arg1.byte.l = 1;
	pe438_cmd->arg1.byte.h = idx;
	pe438_cmd->arg2.word = onoff;
	hal_can_send(BRD_EBRD_CANID, (uint8_t *)pe438_cmd, 6);
}

void E438_check_online(uint32_t msgid)
{
	pe438_cmd->type = GET_VER_ISR;
	hal_can_send(msgid, (uint8_t *)pe438_cmd, 1);
}

/*----------------------------------------------------------------------------*/

void hal_E438_emf_poll(int mode)
{
	static unsigned long ntime;
	static short val;
	static short i = 0;
	unsigned long ctime;
	
	ctime = arch_get_systicks();
	if (ntime > ctime)
		return;
	
	ntime = ctime + E438.emf_tick;
	E438_exec_emf(i, val);
	i++;
	if (i >= E438.emfs)
	{
		i = 0;
		val = val ? 0 : 0xFFFF;
	}
}

void hal_E438_xzq_poll(int mode)
{
	static unsigned long ntime;
	static short mask;
	static short val;
	static short dao = 0;
	unsigned long ctime;
	int i;

	ctime = arch_get_systicks();
	if (ntime > ctime)
		return;
	
	ntime = ctime + E438.xzq_tick;
	if (mode == 0)	//单刀循环
	{
		if (dao == 0)
			val = val ? 0 : 0xFFFF;

		mask = val & (1 << dao);
		if (++dao >= 8)
		{
			dao = 0;
		}
	}
	else if (mode == 1)//全上全下
	{
		mask = mask ? 0 : 0xFFFF;
	}
	else	// 一隔一
	{
		mask = mask != 0x5555 ? 0x5555 : 0xaaaa;
	}
	
	for (i = 0; i < E438.xzqs; i++)
	{
		E438_exec_xzq(i, mask);
	}
}

void hal_E438_mot_poll(int mode)
{
	static unsigned long ntime;
	static short pos;
	unsigned long ctime;
	int i, mtype;

	ctime = arch_get_systicks();
	if (ntime > ctime)
		return;
	
	if (E438.mot_tick < 500)
		E438.mot_tick = 500;
	
	ntime = ctime + E438.mot_tick;
	pos = pos ? 0 : 700;
	
	//for (i = 0; i < E438.mots; i++)
	for (i = 0; i < 4; i++)	
	{
		E438_exec_step(i, pos);
	}
	
	for (mtype = 0; mtype < 4; mtype++)
	{
		for (i = 0; i < 4; i++)
		{
			arch_delay_ms(2);
			E438_exec_step_other(mtype, i, pos);
		}
	}
}
/*----------------------------------------------------------------------------*/


typedef struct {
	uint8_t type;
	uint32_t isrcnt;
	int16_t dpos;
	int16_t cpos;
	uint16_t time;
}TEST_t;

TEST_t tab[] = 
{
/* 0  */ {0, 05106, 286, 0  , 0,},
/* 1  */ {1, 11106, 216, 286, 274,},
/* 2  */ {1, 11140, 216, 283, 254,},
/* 3  */ {1, 11184, 216, 279, 235,},
/* 4  */ {1, 11218, 216, 272, 215,},
/* 5  */ {1, 11263, 216, 260, 195,},
/* 6  */ {1, 11297, 216, 249, 177,},
/* 7  */ {1, 11342, 216, 231, 156,},
/* 8  */ {1, 11377, 216, 228, 137,},
/* 9  */ {1, 11422, 216, 223, 118,},
/* 10 */ {1, 11456, 216, 219,  97,},
/* 11 */ {1, 11501, 216, 216,  78,},
/* 12 */ {1, 11535, 216, 216,  58,},
/* 13 */ {1, 11580, 216, 216,  39,},

/* 14 */ {1, 11698, 356, 216, 157,},
/* 15 */ {1, 11733, 356, 220, 138,},
/* 16 */ {1, 11777, 356, 228, 118,},
/* 17 */ {1, 11811, 356, 240,  98,},
/* 18 */ {1, 11856, 356, 257,  79,},
/* 19 */ {1, 11890, 356, 269,  59,},
/* 20 */ {1, 11935, 356, 284,  39,},

/* 21 */ {1, 12192, 216, 356, 158,},
/* 22 */ {1, 12228, 216, 353, 138,},
/* 23 */ {1, 12272, 216, 346, 118,},
/* 24 */ {1, 12306, 216, 337,  98,},
/* 25 */ {1, 12352, 216, 318,  79,},
/* 26 */ {1, 12388, 216, 306,  59,},
/* 27 */ {1, 12432, 216, 290,  39,},

/* 28 */ {1, 12650, 356, 221, 158,},
/* 29 */ {1, 12685, 356, 227, 138,},
/* 30 */ {1, 12730, 356, 240, 119,},
/* 31 */ {1, 12764, 356, 255,  98,},
/* 32 */ {1, 12809, 356, 270,  79,},
/* 33 */ {1, 12844, 356, 282,  59,},
/* 34 */ {1, 12888, 356, 297,  39,},

/* 35 */ {1, 13147, 216, 356, 159,},
/* 36 */ {1, 13181, 216, 353, 138,},
/* 37 */ {1, 13226, 216, 348, 119,},
/* 38 */ {1, 13261, 216, 342,  99,},
/* 39 */ {1, 13308, 216, 325,  79,},
/* 40 */ {1, 13342, 216, 312,  59,},
/* 41 */ {1, 13388, 216, 296,  39,},

/* 42 */ {1, 13607, 356, 223, 159,},
/* 43 */ {1, 13643, 356, 230, 138,},
/* 44 */ {1, 13686, 356, 246, 119,},
/* 45 */ {1, 13721, 356, 259,  99,},
/* 46 */ {1, 13768, 356, 276,  79,},
/* 47 */ {1, 13800, 356, 286,  59,},
/* 48 */ {1, 13845, 356, 301,  39,},

/* 49 */ {1, 14104, 216, 356, 159,},
/* 50 */ {1, 14139, 216, 353, 138,},
/* 51 */ {1, 14184, 216, 348, 119,},
/* 52 */ {1, 14219, 216, 341,  99,},
/* 53 */ {1, 14265, 216, 325,  79,},
/* 54 */ {1, 14300, 216, 312,  59,},
/* 55 */ {1, 14345, 216, 296,  39,},

/* 56 */ {1, 14565, 356, 223, 159,},
/* 57 */ {1, 14599, 356, 230, 139,},
/* 58 */ {1, 14644, 356, 246, 119,},
/* 59 */ {1, 14679, 356, 260,  99,},
/* 60 */ {1, 14724, 356, 275,  79,},
/* 61 */ {1, 14759, 356, 287,  59,},
/* 62 */ {1, 14804, 356, 303,  39,},
};
#define COUNTOF(a)	(sizeof(a) / sizeof(a[0]))


void mot_dync_test()
{
	static uint32_t i = 0;
	static uint32_t prevtime, isrcount;
	uint32_t cur_time = arch_get_systicks();
	
	if (prevtime == cur_time)
	{
		return;
	}
	
	prevtime = cur_time;
	isrcount += 2;
	
	if (i >= COUNTOF(tab))
	{
		isrcount = 11936;
		i = 21;
		return;
	}

	if (tab[i].isrcnt & 0x1)
		tab[i].isrcnt++;
	
	if (tab[i].isrcnt == isrcount)
	{
		if (tab[i].type)
		{
			E438_dync_step(0, tab[i].dpos, tab[i].time, 90);
			i++;
		}
		else if (tab[i].type == 0)
		{
			E438_exec_step(0, tab[i].dpos);
			i++;
		}
	}
}

void hal_E438_poll(void)
{
	hal_E438_mot_poll(0);
	//hal_E438_emf_poll(0);
	//hal_E438_xzq_poll(1);
	//mot_dync_test();
}


int hal_e438_check(void)
{
	int ret;
	uint16_t canid;
	long ctime, ntime;
	
	canid = BRD_EBRD_CANID;
	
	CanRecv_Clear();
	E438_check_online(canid);
	
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
		E438.cantxid = canid;
		E438.online = 1;
		E438.emfs = 40;
		E438.xzqs = 8;
		E438.mots = 7;

		E438.emf_mode = 0;
		E438.xzq_mode = 1;
		E438.mot_mode = 0;
		E438.power = 0;
		E438.son = 0;
		E438.mot_ma = 800;
		E438.cancomid = 0;
		
		E438.xzq_tick = 500;
		E438.emf_tick = 2000;
		E438.mot_tick = 5000;
		strcpy(E438.name, "E438 Aged Test");
		
		{
			int i;
			CAN_MSG_DEF *canmsg = NULL;

			E438.sysno = 0;
			arch_delay_ms(200);
			for (i = 0; i < ret; i++)
			{
				ret = CanRecv_pick(i, &canmsg);
				if (canmsg->msg_dat[0] == CMDTYPE_CHECKONLINE)
				{
					E438.sysno |= (1 << canmsg->msg_dat[1]);
				}
			}
		}
	}
	/*
	else
	{
		strcpy(E438.name, "E438 No find");
	}*/
	return ret;
}


void hal_E438_init(void)
{
	hal_e438_check();
	
	E438_power_ctrl(1);
	
	E438_step_current(800);
	
	E438_step_son(1);
	E438_step_active();
	
	gui_ebrd_reg(E438.name, hal_E438_poll);
}

void hal_E438_process(void)
{
	hal_E438_init();
	gui_ebrd_process();
}


