/**
  *****************************************************************************
  * @file    hal_E534.c
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2016-08-16
  * @brief   单板双系统:8个度目，两个生克，64个选针，24个三角，16个沙嘴
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/
  
/* Includes ------------------------------------------------------------------*/
#include "hal_E534.h"
#include "E534_cmd.h"
#include "hal_can.h"
#include "hal_ebrd.h"
#include "canfifo.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {
	uint8_t  cmd;
	uint8_t  arg1;
	uint16_t arg2;
	uint16_t arg3;
	uint16_t arg4;
}E534_CMD_t, E459_CMD_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static E534_CMD_t *pe534_cmd = (E534_CMD_t *)&ebrd_cmd;

static char auto_test_mode;

EBRD_INFO_t E534;
EBRD_INFO_t E459;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void E534_power_ctrl(int onoff)
{
	pe534_cmd->cmd = onoff ? POWER_ON : POWER_OFF;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 1);
	E534.power = onoff;
}

void E534_error_clean(void)
{
	pe534_cmd->cmd = CLR_ERR;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 1);
}

void E534_step_son(int son)
{
	pe534_cmd->cmd = son ? STEP_ON : STEP_OFF;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 1);
	E534.son = son;
}

void E534_step_reset(void)
{
	pe534_cmd->cmd = STEP_ALL_RST;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 1);
}

void E534_get_step_zero(int idx)
{
	pe534_cmd->cmd = GET_STEPMOTOR_ZERO_SIG;
	pe534_cmd->arg1 = idx;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 2);
}

void E534_get_step_pos(int idx)
{
	pe534_cmd->cmd = GET_MOT;
	pe534_cmd->arg1 = idx;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 2);
}

void E534_get_step_busy(int idx)
{
	pe534_cmd->cmd = GET_STEPMOTOR_BUSY_STATUS;
	pe534_cmd->arg1 = idx;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 2);
}

void E534_exec_step(int idx, unsigned short pos)
{
	pe534_cmd->cmd = STEP_ISR;
	pe534_cmd->arg1 = idx;
	pe534_cmd->arg2 = pos;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 4);
}

void E534_exec_xzq(int idx, unsigned short mask)
{
	pe534_cmd->cmd = XZQ_ISR;
	pe534_cmd->arg1 = idx;
	pe534_cmd->arg2 = mask;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 4);
}

void E534_get_xzq_state(int idx)
{
	pe534_cmd->cmd = GET_XZQ;
	pe534_cmd->arg1 = idx;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 2);
}

void E534_exec_emf(int idx, int onoff)
{
	pe534_cmd->cmd = DCT_ISR;
	pe534_cmd->arg1 = idx;
	pe534_cmd->arg2 = onoff;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 4);
}

void E534_get_emf_state(int grp)
{
	pe534_cmd->cmd = GET_ACT;
	pe534_cmd->arg1 = grp;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 2);
}

void E534_get_error(void)
{
	pe534_cmd->cmd = GET_ERR;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 1);
}

void E534_verison(int msgid)
{
	pe534_cmd->cmd = GET_VER_ISR;
	hal_can_send(msgid, (uint8_t *)pe534_cmd, 1);
}

void E534_board_type(void)
{
	pe534_cmd->cmd = GET_BOARD_TYPE;
	hal_can_send(E534.cantxid, (uint8_t *)pe534_cmd, 1);
}


/*----------------------------------------------------------------------------*/

void E459_verison(void)
{
	pe534_cmd->cmd = GET_VER_ISR;
	hal_can_send(E459.cantxid, (uint8_t *)pe534_cmd, 1);
}

void E459_step_son(int son)
{
	pe534_cmd->cmd = son ? STEP_ON : STEP_OFF;
	hal_can_send(E459.cantxid, (uint8_t *)pe534_cmd, 1);
	E459.son = son;
}

void E459_exec_trig(int idx, int pos)
{
	pe534_cmd->cmd = TRIANGLE_STEP_ISR;
	pe534_cmd->arg1 = idx;
	pe534_cmd->arg2 = pos;
	hal_can_send(E459.cantxid, (uint8_t *)pe534_cmd, 4);
}

/*----------------------------------------------------------------------------*/

void hal_E534_emf_poll(int mode)
{
	static unsigned long ntime;
	static short val;
	static int i = 0;
	unsigned long ctime;

	if (E534.emfs == 0)
		return;
	
	ctime = arch_get_systicks();
	if (ntime > ctime)
		return;
	
	ntime = ctime + E534.emf_tick;
	E534_exec_emf(i, val);

	i++;
	if (i >= E534.emfs)
	{
		i = 0;
		val = val ? 0 : 0xFFFF;
	}
}

void hal_E534_xzq_poll(int mode)
{
	static unsigned long ntime;
	static short mask;
	static short val;
	static short dao = 0;
	unsigned long ctime;
	int i;

	if (E534.xzqs == 0)
		return;

	ctime = arch_get_systicks();
	if (ntime > ctime)
		return;
	
	if (E534.xzq_tick == 0)
		E534.xzq_tick = 200;
	
	ntime = ctime + E534.xzq_tick;
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
	
	for (i = 0; i < E534.xzqs; i++)
	{
		E534_exec_xzq(i, mask);
	}
}

void hal_E534_mot_poll(int mode)
{
	static unsigned long ntime;
	static short pos;
	unsigned long ctime;
	int i;

	if (E534.mots == 0)
		return;
	
	ctime = arch_get_systicks();
	if (ntime > ctime)
		return;

	if (E534.mot_tick < 500)
		E534.mot_tick = 500;
	
	ntime = ctime + E534.mot_tick;
	pos = pos ? 0 : 700;
	
	for (i = 0; i < E534.mots; i++)
	{
		E534_exec_step(i, pos);
	}
}

void hal_E459_trig_poll(int mode)
{
	static unsigned long ntime;
	static short pos;
	unsigned long ctime;
	int i;

	ctime = arch_get_systicks();
	if (ntime > ctime)
		return;
	
	ntime = ctime + 10000;
	pos++;
	pos &= 0x7;
	for (i = 0; i < 8; i++)
	{
		E459_exec_trig(i, pos);
	}
}

void hal_E534_poll(void)
{
	hal_E534_mot_poll(E534.mot_mode);
	hal_E534_emf_poll(E534.emf_mode);
	hal_E534_xzq_poll(E534.xzq_mode);
	hal_E459_trig_poll(0);
}

int hal_e534_check(void)
{
	int ret, times;
	uint16_t canid;
	long ctime,ntime;
	
	times = 0;
	canid = BRD_E534_CANDID;

RETRY:
	
	CanRecv_Clear();
	E534_verison(canid);
	
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
	
	if (ret)
	{
		E534.cantxid = canid;
		E534.online = 1;
		E534.emfs = 40;
		E534.xzqs = 8;
		E534.mots = 10;

		E534.emf_mode = 0;
		E534.xzq_mode = 1;
		E534.mot_mode = 0;
		E534.power = 0;
		E534.son = 0;
		E534.mot_ma = 800;
		E534.cancomid = 0;
		
		E534.xzq_tick = 500;
		E534.emf_tick = 2000;
		E534.mot_tick = 5000;
	}
	else if (times == 0)
	{
		times++;
		canid = BRD_E534_CANID2;
		goto RETRY;
	}
	return ret;
}


void hal_E534_init(void)
{
	
	E534_power_ctrl(1);
	E534_step_son(1);
	E459_step_son(1);
	E534_error_clean();

	gui_ebrd_reg("E534 Aged Test", hal_E534_poll);
}

void hal_E534_process(void)
{
	hal_E534_init();
	gui_ebrd_process();
}

/*----------------------------------------------------------------------------*/

int hal_e459_check(void)
{
	int ret;

	E459.cantxid = BRD_E459_CANDID;
	E459.online = 0;
	
	// E459
	CanRecv_Clear();
	E459_verison();
	arch_delay_ms(200);
	ret = CanRecv_Total();
	if (ret)
	{
		E459.online = 1;
		strcpy(E459.name, "E459");
		E459.emfs = 0;
		E459.power = 0;
		E459.son = 0;
	}
}

