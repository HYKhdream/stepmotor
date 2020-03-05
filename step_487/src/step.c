//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "arch.h"
#include "command.h"
#include "alert.h"
#include "config.h"
#include "step.h"
#include "stm32f37x_gpio.h"
//#include "platform_config.h"




// Resolution
#define FULL	0
#define HALF	1
#define QUARTER	2
#define EIGHTH	3

#define RESOLUTION	2

//#define MAX_STEPMOTOR_SUPPORT	10
//#define DEBUG_STEP
#define NEW_ZERO_DETECT_MODE
//#define FEET_SUPPORT
//#define STEPACC_DEBUG
//#define STEPACC_DIV_DEBUG
//#define ZERO2_SUPPORT

#define MAX_STEPS	(800L)

#define MAX_STEPS_YARN   (900L)

#define MAX_STEPS_UP_ZERO_YARN (800L)

#define MAX_STEPS_DOWN_ZERO_YARN (80)

#define MAX_STEPS_UP_ZERO	(400L)    //正的400步

#define MAX_STEPS_DOWN_ZERO	(200L)  //   负的200步

#define MAX_STEPMOTOR_SUPPORT (8)

#ifdef ZERO_POS_MORC_UPDATE
#define DEF_YARN_STEPMOTO_ZERO_WIDTH (20)
#else

#define DEF_YARN_STEPMOTO_ZERO_WIDTH (66)

#endif
#ifdef PLATFORM_TMS2812
//#pragma DATA_SECTION(STEPMOTOR,"EXRAM");
#endif

//#define SINKER_NEW_CURL_TEST
STEP_TYPE STEPMOTOR[MAX_STEPMOTOR_SUPPORT/*10*/];



volatile unsigned int step_run_mode;
volatile unsigned int step_reset_speed[2];
volatile unsigned int step_max_speed[2];
volatile unsigned int step_timer_cnt;

#ifdef STEP_USE_HZ_
volatile unsigned int act_step_start_hz;
volatile unsigned int act_step_max_hz;
volatile unsigned int act_step_reset_hz;
volatile unsigned int act_step_a_hz;
unsigned short sys_debug_hz[600];
float sys_debug_T[600];



#endif
volatile unsigned int step_base_speed;
// by xhl 2010/05/19
volatile unsigned int sinker_max_speed;
volatile unsigned int sinker_add_speed; //hlc 20140725

volatile unsigned int sinker_max_speed_isset;   //hlc 20140905

// by xhl 2010/09/15
volatile unsigned int sinker_base_speed;
//volatile unsigned int Step_Zero_Check_Delay[10];
volatile unsigned int step_support_max_count = 8;  //默认8个电机
volatile unsigned int step_active;
volatile unsigned int step_poweron;
// by xhl 2010/06/03
volatile unsigned int step_direction;
// by xhl 2010/06/05
volatile unsigned int step_resolution;
// by xhl 2011/01/01
volatile unsigned int step_fast_mode;
// by xhl 2012/03/05
volatile unsigned int step_interval;
// 复位时，电机离开零位后等待delay_time * 2ms后再归零
volatile unsigned int step_reset_delay_time[MAX_STEPMOTOR_SUPPORT];


volatile  int Steps_lost_alarm;

volatile   unsigned short  Steps_Done_report_sts=0x0000;

volatile int feet_enable;
#ifdef LOGOUT_ENABLE
extern volatile unsigned short logout_enable;
#endif

 volatile unsigned int vary_step_enable=0;

extern volatile unsigned int vary_sk_enable;
extern volatile unsigned int vary_yj_enable;
extern volatile unsigned long step_speed[10];

extern volatile unsigned short dct_stus;

#ifdef STEP_WORK_SIGN_SUPPORT

//*******************************************************//
//*******************************************************//
volatile unsigned int step_work_sign_alarmenable;
#endif




#ifdef FEET_SUPPORT

// by xhl 2012/03/08
volatile int feet_run_max_step;
volatile int feet_alarm_enable;
volatile int feet_sts;
void check_feet_sts();
//volatile unsigned int testalarmcount = 0;

volatile int feet_used_num = FEET_DEF_NUM;
volatile int feet_first_act;
#endif

int alert_push(int alert_code, int alert_arg);

#ifdef DEBUG_STEP
volatile unsigned int Debug_Count[2][4];
#endif


#define STEP_ZERO_ADJ_DEF 2

volatile int step_zero_adj;
volatile int step_zero_detect;
volatile int step_alert_detect_setup;
// by xhl 2012/05/19
// 当生克位置小于设定值时，零位有效不报警
volatile int sinker_zero_area;
#ifdef ZERO2_SUPPORT
volatile unsigned int step_zero2_enable;
volatile int step_setup_zero2_detect_steps;
void StepMotor_Detect_Zero2(unsigned int stepno);
#endif

#ifdef TRIANGLE_STEP_SUPPORT 
volatile unsigned int max_triangle_num;
volatile unsigned int Triangle_sts[MOTOR_COUNT_MAX];					//
volatile unsigned int Triangle_sts_reg[MOTOR_COUNT_MAX];					//

volatile unsigned char stepRunOK_return;					//


volatile int triangle_err_count[MOTOR_COUNT_MAX] ;

extern volatile unsigned int Triangle_mode; 	

#endif


volatile unsigned int max_sinker_num;
int is_sinker(int stepno);
void StepMotor_Isr(unsigned int stepno);

void StepMotor_exec(unsigned int stepno, short pos, int mode);


extern void DAC_SetVoltage(long Vout);
extern void DAC2_SetVoltage(long Vout);

#define STEP_ERR_CODE(_idx)	 (Is_Yarn_Board()? (((_idx < 8) ? (YARN_STEP1_ERR + _idx) :0)):(((_idx < 8) ? (ACT_STEP_FAULT1 + _idx) :0)))

#define COUNTS(_X)	(sizeof(_X) / sizeof(_X[0]))
volatile unsigned int Step_pulse[] = {
	170, 165, 158, 150, 140,
	130, 115, 102,  87,  73,
	 65,  57,  49,  42,  37,
       	 31,  26,  21,  17,  13,
	 11,  8,    5,   3,   1,
};
#ifdef SINKER_NEW_CURL_TEST

#if 1
volatile unsigned int Step_pulse_sinker[] = {
	170, 165, 158, 150, 140,
	128, 113, 98,  83,  70,
	 57,  47,  37,  31,  26,
	 21,  17,  13,  11,  8,
	 5,  3,    1,
};
#else
volatile unsigned int Step_pulse_sinker[] = {
	270,260,250,240,230,220,
	200,180,160,140,120,100,
	80,60,50,40,30,20,10,6,
	3,2,1,
};

#endif

#endif

#ifdef TRIANGLE_STEP_SUPPORT
#if 0
volatile unsigned int Step_pulse_triangle[] = {
	170, 165, 158, 150, 140,
	128, 113, 98,  83,  70,
	 57,  47,  37,  31,  26,
	 21,  17,  13,  11,  8,
	 5,  3,    1,
};
#endif
volatile unsigned int Step_pulse_triangle[] = {
	600,580,550,510,460,
	400,330,260,190,130,
	80,48,28,16,10,
	8,7,6,5,4,
	3,2,1,
};

volatile unsigned int Step_pulse_triangle_ex[] = {
	130,	80,48,28,16,10,
	8,7,6,5,4,
	3,2,1,
};


#define ACC_STEPS_TRIANGLE_EX	COUNTS(Step_pulse_triangle_ex)


#define ACC_STEPS_TRIANGLE	COUNTS(Step_pulse_triangle)

volatile unsigned int Triangle_Step_no_map[8] = {0,2,1,3,4,5,6,7};
volatile unsigned int Triangle_work_sign_index_map[8] = {0,1,2,3,4,5,6,7};


#endif




//#define ACC_STEPS	25
#define ACC_STEPS	COUNTS(Step_pulse)
#ifdef SINKER_NEW_CURL_TEST
#define ACC_STEPS_SINKER	COUNTS(Step_pulse_sinker)
#endif

#ifdef USE_HZ_
#define ACC_STEP 	ACC_STEPS_HZ_
#else

#define ACC_STEP	Step->acc_steps
#endif
//#define ZERO_WIDTH	1
#define ZERO_DETECT	step_zero_detect

//#ifdef SINKER_MAXSPEED_ISFAST

#define SK_ADD_SPEED_ALL 	(80)
//#else
//#define SK_ADD_SPEED_ALL 	(0)
//#endif


#ifdef ENCODER_SUPPORT
#define ENCODER_CHECK_DELAY_RUN	10
#define ENCODER_CHECK_DELAY_RST	10
#define ENCODER_CHECK_INTERVAL	10






int EE_Read(int addr, unsigned short *Data, int len);
int EE_Write(int addr, unsigned short *Data, int len);

int SSI_Read(int idx, int *coder, int *status);


extern void Massage_Send_EMF_REBACK(short arg1,short arg2,short arg3,short arg4);

typedef struct {
	unsigned short encoder_init;
	unsigned short encoder_zero[8];
	unsigned short rsvd[6];
	unsigned short CRC;
}ENCODER_DEF;
volatile short encoder_status;
volatile short ssi_check_idx;
volatile short encoder_enable;
volatile short encoder_init;
volatile short encoder_zero[MAX_STEPMOTOR_SUPPORT/*10*/];
volatile short encoder_alert_detect;
volatile short encoder_alert_state;
// by xhl 2011/09/16
volatile unsigned int coder_err;






void StepMotor_Encoder_Load()
{
	short i;
	ENCODER_DEF ENC_def;
	unsigned short *p = (unsigned short *)&ENC_def;
	unsigned short CRC = 0;
	short len = sizeof(ENC_def) / sizeof(short);

	EE_Read(10, (unsigned short*)&ENC_def, len);
	// 
	for(i = 0; i < len; i ++) {
		CRC += p[i];
	}

	//myprintf("Load CRC %d\r\n", CRC);
	if(CRC) return ;

	encoder_init = ENC_def.encoder_init;
	//myprintf("Load init 0x%x\r\n", encoder_init);
	for(i = 0; i < 8; i ++) {
		encoder_zero[i] = ENC_def.encoder_zero[i];
		//myprintf("Load zero %d\r\n", encoder_zero[i]);
	}
}

void StepMotor_Encoder_Save()
{
	short i;
	ENCODER_DEF ENC_def;
	unsigned short *p = (unsigned short *)&ENC_def;
	unsigned short CRC;
	short len = sizeof(ENC_def) / sizeof(short);

	for(i = 0; i < len; i ++) {
		p[i] = 0;
	}

	ENC_def.encoder_init = encoder_init;
	for(i = 0; i < 8; i ++) {
		ENC_def.encoder_zero[i] = encoder_zero[i];
	}
	CRC = 0;
	for(i = 0; i < len; i ++) {
		CRC += p[i];
	}
	ENC_def.CRC = -CRC;

	EE_Write(10, (unsigned short*)&ENC_def, len);
}
#endif


#ifdef TRIANGLE_STEP_SUPPORT 

void StepMotor_Triangle_Isr(unsigned int stepno);
#endif

int Exec_DCT_Motor_Get_runing_sts()
{
	unsigned int ret = 0;
	int i;
	for (i=0;i<8;i++)
	{
		//(STEPMOTOR[i].running ? 1:0)
		ret |= ((STEPMOTOR[i].running ? 1:0)<<i);
	}

	Massage_Send_EMF_REBACK(ret,0,0,0);

return 0;
}

int Exec_DCT_Motor_Get_pos(int stepno)
{
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;
	//int zero_work_area;
	//int zero;
	//int fast_mode = mode;

	short pos ;//= stepspeed_Q2R(pos);


	if(stepno>=8) return;	

	Step = &STEPMOTOR[stepno];

	
	pos = Step->position;


	if( step_resolution != QUARTER) {
		if(pos < 0) {
			pos *= -1;
			pos = stepspeed_Q2R(pos);
			pos *= -1;
		}
		else {
			pos = stepspeed_Q2R(pos);
		}
	}

	#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
	Massage_Send_EMF_REBACK(0x13FD,stepno,pos,0);
	#else

	Massage_Send_EMF_REBACK(pos,stepno,0,0);
	#endif

}


void StepMotor_map_init()
{
	int i;

	if (Is_Yarn_Board())
	{
		for (i=0 ; i<8 ; i++)
		{
			Triangle_Step_no_map[i]=i;
			Triangle_work_sign_index_map[i]=8;  //8号无效
		}
	}
	else
	{
	#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
		Triangle_Step_no_map[0]=0;//主控0号电机(后床左系统动作电机) --小板子左系统1号电机
		Triangle_Step_no_map[1]=1;//主控1号电机(后床右系统动作电机) --小板子左系统2号电机
		Triangle_Step_no_map[2]=2;//主控2号电机(前床左系统动作电机) --小板子右系统1号电机
		Triangle_Step_no_map[3]=3;//主控3号电机(前床右系统动作电机) --小板子右系统2号电机
		Triangle_Step_no_map[4]=4;
		Triangle_Step_no_map[5]=5;
		Triangle_Step_no_map[6]=6;
		Triangle_Step_no_map[7]=7;	
		Triangle_work_sign_index_map[0] = 8;
		Triangle_work_sign_index_map[1] = 8;
		Triangle_work_sign_index_map[2] = 8;
		Triangle_work_sign_index_map[3] = 8;
		Triangle_work_sign_index_map[4] = 8;
		Triangle_work_sign_index_map[5] = 8;
		Triangle_work_sign_index_map[6] = 8;
		Triangle_work_sign_index_map[7] = 8;
	#else
		Triangle_Step_no_map[0]=0;//主控0号电机(后床左系统动作电机) --小板子左系统1号电机
		Triangle_Step_no_map[1]=1;//主控1号电机(后床右系统动作电机) --小板子左系统2号电机
		Triangle_Step_no_map[2]=4;//主控2号电机(前床左系统动作电机) --小板子右系统1号电机
		Triangle_Step_no_map[3]=5;//主控3号电机(前床右系统动作电机) --小板子右系统2号电机
		Triangle_Step_no_map[4]=2;
		Triangle_Step_no_map[5]=3;
		Triangle_Step_no_map[6]=6;
		Triangle_Step_no_map[7]=7;	
		Triangle_work_sign_index_map[0] = 2;
		Triangle_work_sign_index_map[1] = 3;
		Triangle_work_sign_index_map[2] = 8;
		Triangle_work_sign_index_map[3] = 8;
		Triangle_work_sign_index_map[4] = 6;
		Triangle_work_sign_index_map[5] = 7;
		Triangle_work_sign_index_map[6] = 8;
		Triangle_work_sign_index_map[7] = 8;
	#endif
			
	}
			
}


unsigned int Get_motor_ID_with_stepno(unsigned int stepno)
{
	if (stepno>=8) return 8;
	return Triangle_Step_no_map[stepno];

}

unsigned int Get_stepno_with_motorID(unsigned int MotorID)
{
	int i=0;
	if (MotorID>=8) return 8;
	for (i=0;i<8;i++)
	{	
		if (Triangle_Step_no_map[i]==MotorID)
		{
				return i;
		}
	}
	return 8;
}



void StepMotor_Init()
{
	unsigned int i;

	//STEP_TYPE *Step = STEPMOTOR;
	StepMotor_map_init();

	step_resolution = QUARTER;

	Steps_lost_alarm = 20;   //失步报警值 (默认10步)

	step_support_max_count = 8;
	step_active = 0x00FF;

	if (Is_Yarn_Board())   // 沙嘴的
	{
		if (Get_doard_type_ex())
		{
			step_support_max_count = 4;
			step_active = 0x0007;
		}
	}

	for(i = 0; i < MAX_STEPMOTOR_SUPPORT/*10*/; i ++) {
		STEP_TYPE *Step = &STEPMOTOR[i];
		//Step->position = 0;
		
		Step->running = 0;
		Step->dir_High = 1;
		if (Is_Yarn_Board())   // 沙嘴的
		{
			Step->motor_type_config =0;
		}
		else
		{
		#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
			Step->motor_type_config = 0;		
		#else
			Step->motor_type_config = (i<4)?0:1;
		#endif
		}
		#ifdef TRIANGLE_STEP_SUPPORT
		
		if (is_triangle(i))
		{	
			Step->check_sts_enable =0;
			Step->change_dir_count =0;
			Step->done_steps =0;
			Step->dir_steps =0;
			Step->position = 0;
			Step->zero_sign_index = i;
			Step->motor_alarm_cnt=0;
			
			Step->work_sign_index = Triangle_work_sign_index_map[i];

			
				{
					if (( i==TRIANGLE_BASE )||(i==TRIANGLE_BASE+2))
						{
							if (Is_Yarn_Board()) 
							{
								Step->dir_High = 1;
							}
							else
							{
								if (!Get_doard_type_ex())
								{
									Step->dir_High = 0;
								}
								else
								{
									Step->dir_High = 1;
								}
							}
						}
					else
						{
						if (Is_Yarn_Board()) 
							{
								Step->dir_High = 1;
							}
							else
								{
								
								if (!Get_doard_type_ex())
								{
									Step->dir_High = 0;
								}
								else
								{
									#ifdef LX_
										Step->dir_High = 0;

									#else

									#ifdef QL_
											Step->dir_High = 0;
									#else
										Step->dir_High = 1;
									#endif
									#endif
								}
								}
						}
				}

			#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS

			Step->dir_High = (i & 0x01)?1:0;
			
			#endif
				
			if (!Get_doard_type_ex())
			{
				Step->Over_Zero_Count = 0;
			}
				
			
		}

		#endif
		if (Is_Yarn_Board()) 
		{
			if(arch_StepMotor_Zero(i)) 
			{
				Step->position = 0;
			}
		else {
				Step->position = 100;
			}
		}
		else
			{
		if (!Get_doard_type_ex())
		{
			if(arch_StepMotor_Zero(i)) 
			{
				Step->position = 0;
			}
			else {
				Step->position = 100;
				}

		}
		else
		{
			if(arch_StepMotor_Zero_EX(Step->zero_sign_index,Step->work_sign_index)) 
			{
				Step->position = 0;
				Step->zero=1;
			}
			else 
			{
				Step->position = 100;
				Step->zero=0;
			}
		}
			}

		Step->pos_2 = Step->position;
		
		Step->alarm_step_leave = 0;

		Step->is_reset =0;
		Step->never_run =1;
		Step->need_2_pos_after_reset = 0;
		Step->reset_width_zero_count = 0;
		Step->moto_zero_width_self = 0;
		Step->moto_zero_deviation =0;
		
		if (Is_Yarn_Board()) 
			{
				Step->moto_zero_width =DEF_YARN_STEPMOTO_ZERO_WIDTH;//Step->motor_type_config? (DEF_YARN_STEPMOTO_ZERO_WIDTH>>1):DEF_YARN_STEPMOTO_ZERO_WIDTH;
				
				Step->step_max = MAX_STEPS_YARN/*1000*/;
			}
		else
			{	

			Step->moto_zero_width =DEF_YARN_STEPMOTO_ZERO_WIDTH;
			
			Step->step_max = MAX_STEPS/*1000*/;
			}

		Step->slow_speed = STEP_ACT_SLOW_SPEED;

		Step->alarm_step = 0;

		//Step_Zero_Check_Delay[i] = 0;
		Step->speed_div = 0;
	#ifdef ENCODER_SUPPORT
		Step->step_delay = 20000;
		Step->enc_err_count = 0;
	#endif
	#ifdef STEP_TEST_IN_NDL
		Step->needle = 0;
	#endif
		Step->zero_count = 0;
		Step->zero_go_ =0;
		Step->step_check_interval = 0;

	#ifdef ZERO2_SUPPORT
		Step->zero2_mode = 0;
		Step->zero2=arch_stepMotor_inout_sts(Step->work_sign_index);
		Step->zero2_temp[Step->zero2?1:0]=ZERO2_DELAY_COUNT;
	       Step->zero2_temp[Step->zero2?0:1]=0;
	
		Step->zero2_count =0;
	#endif
		Step->steps = 0;
		Step->step_wait_time = 0;
	}

#if 0
	// chang dir
	for(i = 0; i < MAX_STEPMOTOR_SUPPORT/*10*/; i += 2) {
		Step->dir_High = 0;
	}
#endif

	step_direction = 0x5555;


	step_run_mode = 0;
	step_fast_mode = 0;

if (Is_Yarn_Board()) 
{
       step_reset_speed[0] = (((STEP_LOW_SPD_YARN+STEP_TIMER_CCNT_YARN*2)<<QUARTER)>>step_resolution)-(STEP_TIMER_CCNT_YARN*2);
	step_max_speed[0] = (((STEP_MAX_SPD_YARN +STEP_TIMER_CCNT_YARN*2)<<QUARTER)>>step_resolution)-(STEP_TIMER_CCNT_YARN*2);
	step_reset_speed[1] =(((STEP_SLOW_LOW_SPD_YARN+STEP_TIMER_CCNT_YARN*2)<<QUARTER)>>step_resolution)-(STEP_TIMER_CCNT_YARN*2);
	step_max_speed[1] = (((STEP_SLOW_MAX_SPD_YARN +STEP_TIMER_CCNT_YARN*2)<<QUARTER)>>step_resolution)-(STEP_TIMER_CCNT_YARN*2);

	step_timer_cnt = STEP_TIMER_CCNT_YARN;
}
else
{
	step_reset_speed[0] = STEP_LOW_SPD;
	step_max_speed[0] = (STEP_MAX_SPD_DEF );// << step_resolution) >> QUARTER;
	
	step_reset_speed[1] = STEP_LOW_SPD;
	step_max_speed[1] = (STEP_MAX_SPD_DEF );// << step_resolution) >> QUARTER;
	
	step_timer_cnt = STEP_TIMER_CCNT;
}


#ifdef STEP_USE_HZ_
act_step_max_hz = STEP_USE_HZ_MAX;
act_step_reset_hz = STEP_USE_HZ_MAX_RESET;
act_step_a_hz = STEP_USE_HZ_A;
act_step_start_hz = STEP_USE_HZ_START;


#endif
	
	
	step_base_speed = 0/*STEP_LOW_SPD*/;

//	sinker_max_speed = (40 << step_resolution) >> QUARTER;
	

	step_zero_adj = 4;//;       检测到0位之后，再走 这么些步数
	step_zero_detect = 7/*5*/;
	step_alert_detect_setup = 50;//10;

	// by xhl 2012/05/19
	sinker_zero_area = 60;    //10/*0*/;0;

#ifdef ZERO2_SUPPORT
	step_zero2_enable = 0;
	step_setup_zero2_detect_steps = 20;
#endif

	

	

	step_poweron = 0xFFFF;


	feet_enable = 0;

	step_interval = 0;
	for(i = 0; i < MAX_STEPMOTOR_SUPPORT; i ++) {
		
		step_reset_delay_time[i] = 50;
	}
	max_sinker_num = 2;//SINKER_COUNT;//2;

#ifdef TRIANGLE_STEP_SUPPORT
	if (Is_Yarn_Board())
		{
				max_triangle_num = 8 ;	

		}
	else
		{
			max_triangle_num = MOTOR_COUNT_MAX ;	
	}
	for (i= 0;i<max_triangle_num ;i++)
	{

	
		Triangle_sts[i] = 0x00;
		Triangle_sts_reg[i]=0x00;
		triangle_err_count[i] = 0x00;
	}
#endif
#ifdef STEP_WORK_SIGN_SUPPORT
	step_work_sign_alarmenable =0;
#endif
	
}

#if 0
void StepMotor_Set_Reset_Delay_Time(int stepno, unsigned int delay)
{
	step_reset_delay_time[stepno] = delay;
}

void StepMotor_Set_Mode(unsigned int mode)
{
	step_run_mode = mode;
	if(step_run_mode) step_zero_adj = 10;
}

void StepMotor_Set_FastMode(unsigned int stepno)
{
	if(stepno >= MAX_STEPMOTOR_SUPPORT) return;
	step_fast_mode |= 0x1 << stepno;
}
#endif

void StepMotor_Setup_Active(unsigned int active)
{
	unsigned int step_mask = 0xFFFFFFFF;
	int step_no;
	step_no = (active +1)>>1;

	if (step_no >(step_support_max_count))
	{
		step_no = (step_support_max_count);
	}
	
	step_active = ~(step_mask <<step_no);
	//step_active = active;
}


void StepMotor_Setup_Active_single(unsigned int step_no,unsigned char isactive)
{
	STEP_TYPE *Step ;//= STEPMOTOR;

	

	if (step_no >(step_support_max_count-1))
	{
		step_no = (step_support_max_count-1);
	}
	Step = &STEPMOTOR[step_no];
	Step->motor_type_config=isactive ?1:0;
	
	if (isactive)
	{
		
		
		step_active |= (0x01 <<step_no);
	}
	else
		step_active &= ~(0x01 <<step_no);

	
	
	//step_active = active;
}

void StepMotor_set_HZ_ex_start_a(unsigned short start,unsigned int a)
{
#ifdef STEP_USE_HZ_
	act_step_start_hz = start;

       act_step_a_hz= a;
#endif
}

void StepMotor_set_HZ_ex_max(unsigned short max,unsigned int max_reset)
{
#ifdef STEP_USE_HZ_
	act_step_max_hz = max;
       act_step_reset_hz = max_reset ;
#endif
}

void StepMotor_set_Remap(unsigned int idx,unsigned int newid)
{
	if ((idx>8) ||(newid>8))
		return;
	Triangle_Step_no_map[idx] = newid;
}

void StepMotor_Setup_Direction(unsigned int dir)
{
	STEP_TYPE *Step = STEPMOTOR;
	unsigned int i;
	unsigned int s_id;

	step_direction = dir;
	for(i = 0; i < MAX_STEPMOTOR_SUPPORT/*10*/; i ++) {
#if 0		
		if(vary_step_enable && (i < 8))
			continue;
		if(vary_sk_enable && is_sinker(i))
			continue;
#else
		if(vary_step_enable & (0x1 << i))
			continue;
#endif

		s_id = Get_motor_ID_with_stepno(i);

		if(step_direction & (0x1 << s_id)) {
			Step[i].dir_High = 0;
		}
		else {
			Step[i].dir_High = 1;
		}
	}
}

void StepMotor_Setup_Resolution(unsigned int rlt)
{
	STEP_TYPE *Step = STEPMOTOR;
	unsigned int i;

	for(i = 0; i < 16; i ++) {
		if((0x1 << i) == rlt) {
			break;
		}
	}
	if(i == 16) {
		return ;
	}
	step_resolution = i;
	//for(i = 0; i < MAX_STEPMOTOR_SUPPORT/*10*/; i ++) {
	
		//Step[i].step_max = (MAX_STEPS_YARN << step_resolution) >> QUARTER;
	//}

if (Is_Yarn_Board()) 
{
       step_reset_speed[0] = ((STEP_LOW_SPD_YARN+STEP_TIMER_CCNT_YARN*2)>>step_resolution)-(STEP_TIMER_CCNT_YARN*2);
	step_max_speed[0] = ((STEP_MAX_SPD_YARN +STEP_TIMER_CCNT_YARN*2)>>step_resolution)-(STEP_TIMER_CCNT_YARN*2);
	step_reset_speed[1] = ((STEP_SLOW_LOW_SPD_YARN+STEP_TIMER_CCNT_YARN*2)>>step_resolution)-(STEP_TIMER_CCNT_YARN*2);
	step_max_speed[1] = ((STEP_SLOW_MAX_SPD_YARN +STEP_TIMER_CCNT_YARN*2)>>step_resolution)-(STEP_TIMER_CCNT_YARN*2);

	step_timer_cnt = STEP_TIMER_CCNT_YARN;
}
else
{
;
}

if (!arch_check_isE487())
{	
	Set_stepmotor_sub(step_resolution);  //
}
	// by xhl 2010/08/14
	//step_zero_adj = (STEP_ZERO_ADJ << step_resolution) >> QUARTER;
	//step_zero_detect = (STEP_ZERO_DETECT << step_resolution) >> QUARTER;

}

#if 0
void StepMotor_Set_Check(unsigned int step)
{
	// by xhl 2011/12/23
	if(step == 0) {
		step_alert_detect_setup = 0;
#ifdef ENCODER_SUPPORT
		encoder_alert_detect = 0;
#endif
		return ;
	}
	//step_zero_detect = step;
	step_alert_detect_setup = step;
#ifdef ENCODER_SUPPORT
	encoder_alert_detect = step * 2 + 10;
#endif
}

#endif
static int stepspeed_R2Q(int R)
{
	R <<= QUARTER;
	R >>= step_resolution;

	return R;
}

static int stepspeed_Q2R(int Q)
{
	Q <<= step_resolution;
	Q >>= QUARTER;

	return Q;
}

int stepspeed_Q2R_EX(int Q)
{
  return stepspeed_Q2R(Q);
}


int Set_LostSteps_alarm(int st_s)   
{
	
	Steps_lost_alarm = st_s ;  //如果值为0的话，表示不报警，自动修正到正中间.
 	
}

int Set_Step_done_report(short rep_sts)
{
	Steps_Done_report_sts = rep_sts;
}

#ifdef STEP_ISR_8_IN_1

void Stepmotor_timer_20us()
{
int i;
for (i=0;i<8;i++)
{
	

}




}


#endif


void StepMotor_Set_Speed_HZ_ex(unsigned int isworkspeed,unsigned int speed_hz,unsigned int whichstep_)
{
	int tmp_max_speed;
	int tmp_ccnt_;
	unsigned int speed_lt;
	if (!Is_Yarn_Board())
	{
		tmp_max_speed = STEP_MAX_SPD;
		tmp_ccnt_ =  	STEP_TIMER_CCNT;
	}
	else
	{
		tmp_max_speed = STEP_MAX_SPD_YARN;//5.5K
		tmp_ccnt_ =  	STEP_TIMER_CCNT_YARN;
	}

	if ((speed_hz<1)||(speed_hz>1000)) return;
	speed_lt =(unsigned int)(1000L*100L/(long)speed_hz);/*speed_hz--单位:10HZ*/

	if (speed_lt <=((tmp_ccnt_<<1)+tmp_max_speed))
	{
		speed_lt =tmp_max_speed;
	}
	else
		speed_lt -= (tmp_ccnt_<<1);
	if (isworkspeed)	
	{
		step_max_speed[whichstep_?1:0] = speed_lt;
	}
	else
		step_reset_speed[whichstep_?1:0] = speed_lt; 
}



void StepMotor_Set_Reset_Speed(unsigned int speed,unsigned int whichsteptype)
{
	int tmp_max_speed;
	int tmp_ccnt_;
	unsigned int whichno=whichsteptype?1:0;
	if (!Is_Yarn_Board())
	{
		tmp_max_speed = STEP_MAX_SPD;
		tmp_ccnt_ =  	STEP_TIMER_CCNT;
	}
	else
	{
		tmp_max_speed = STEP_MAX_SPD_YARN;//5.5K
		tmp_ccnt_ =  	STEP_TIMER_CCNT_YARN;
	}
	
	speed = stepspeed_R2Q(speed+(tmp_ccnt_<<1))-(tmp_ccnt_<<1);

	if(speed <= tmp_max_speed)
		step_reset_speed[whichno] = tmp_max_speed;
	else
		step_reset_speed[whichno] = speed;
}

void StepMotor_Set_Speed(unsigned int speed,unsigned int whichsteptype)
{
	int tmp_max_speed;
	int tmp_ccnt_;
	unsigned int whichno=whichsteptype?1:0;
	
	if (Is_Yarn_Board())
	{
		tmp_max_speed = 0;//5.5K
		tmp_ccnt_ =  	STEP_TIMER_CCNT_YARN;
	}
	else
	{
		tmp_max_speed = 0;
		tmp_ccnt_ =  	STEP_TIMER_CCNT;
	}
	
	speed = stepspeed_R2Q(speed+(tmp_ccnt_<<1))-(tmp_ccnt_<<1);
	if(speed < tmp_max_speed)
		step_max_speed[whichno] = tmp_max_speed;
	else
		step_max_speed[whichno] = speed;
 
	
}

#if 0
void StepMotor_Set_SinkerSpeed(unsigned int speed)
{
	//if(speed > STEP_LOW_SPD) {
	//	speed = STEP_LOW_SPD;
	//}
unsigned int sk_max_speed_hlc=60;
	sinker_max_speed_isset = 1;    //set it and record it
	
		
	speed = stepspeed_R2Q(speed);

	//if ( SK_MAX_SPEED>1)
	//	sk_max_speed_hlc = 1;
	//
	if(speed < sk_max_speed_hlc)
		sinker_max_speed = sk_max_speed_hlc;
	else
		sinker_max_speed = speed;

	sinker_add_speed = SK_ADD_SPEED_ALL;
	
}

void StepMotor_Setup_Para(int cmd, int para)
{
	//myprintf("step setup %d, %d\r\n", cmd, para);
	switch(cmd) {
	case 1:
		step_zero_detect = para;
#ifdef FEET_SUPPORT_4
		sinker_zero_area = para;
#endif
		break;
	case 2:
#ifndef FEET_SUPPORT_4
		sinker_zero_area = para;
#endif
		break;
	case 3:
		step_zero_adj = para;
		break;
#ifdef ZERO2_SUPPORT
	case 4:
		if(para >= 4)
		step_setup_zero2_detect_steps = para;
		break;
	case 5:
		StepMotor_Detect_Zero2(para);
		break;
	case 6:
		step_zero2_enable = para;
		break;
#endif
	}
}

#endif
short StepMotor_Get_Position(unsigned int stepno)
{
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;
	short pos;
	

	
	if(stepno >= MAX_STEPMOTOR_SUPPORT) return 0;

	Step = &STEPMOTOR[stepno];
	pos = Step->position;

	if(pos < 0) {
		pos *= -1;
		pos = stepspeed_R2Q(pos);
		pos *= -1;
	}
	else {
		pos = stepspeed_R2Q(pos);
	}

#ifdef DEBUG_STEP
	return Debug_Count[stepno / 4][stepno % 4];
#endif



	return pos;
}

unsigned int StepMotor_Get_Unfinished_Posidx(unsigned int stepno)
{
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;

	if(stepno >= MAX_STEPMOTOR_SUPPORT) return 0;

	Step = &STEPMOTOR[stepno];

	return Step->steps;
}


unsigned int StepMotor_Get_Busyidx(unsigned int stepno)
{
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;

	if(stepno >= MAX_STEPMOTOR_SUPPORT) return 0;

	Step = &STEPMOTOR[stepno];

	return Step->running;
}

unsigned int StepMotor_Get_Busy(void)
{
	unsigned int i;
	unsigned int ret = 0;
	for(i = 0; i < 8; i ++) {
		if(vary_step_enable & (0x1 << i))
			continue;
		if(StepMotor_Get_Busyidx(i)) {
			ret = 0xFFFF;
			break;
		}
	}
	return ret;
}

#if 0
// 用于测试电机是否到位
void StepMotor_debug(void)
{
	unsigned int i;
	int idx = 0x0;
	STEP_TYPE *Step;
	int ret = 0;

	for(i = 0; i < 10; i++) {
		Step = &STEPMOTOR[i];
		if(Step->running) {
			idx |= 0x1 << i;
			if(Step->steps > ret)
				ret = Step->steps;
		}
		// 只测试偶数电机 ?
		if(step_run_mode) i ++;
	}
	alert_push(idx, ret);
}

#endif
#ifdef STEP_TEST_IN_NDL
unsigned int StepMotor_Get_Needle(unsigned int stepno)
{
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;

	if(stepno >= MAX_STEPMOTOR_SUPPORT) return 0;

	Step = &STEPMOTOR[stepno];

	return Step->needle;
}
#endif

void StepMotor_Set_Position(unsigned int stepno, short pos)
{
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;

	if(stepno >= MAX_STEPMOTOR_SUPPORT) return;

	/*#ifdef TRIANGLE_STEP_SUPPORT
		if (is_triangle(stepno))
		{ 
			stepno = Triangle_Step_no_map[stepno-TRIANGLE_BASE]+TRIANGLE_BASE;
		}			
	#endif*/

	Step = &STEPMOTOR[stepno];
	if(pos < 0) {
		pos *= -1;
		pos = stepspeed_Q2R(pos);
		pos *= -1;
	}
	else {
		pos = stepspeed_Q2R(pos);
	}

	// by xhl 2010/07/30
	//step_poweron &= ~(0x1 << stepno);
	Step->is_poweroff_goon =1; /*表示断电续织*/
	Step->is_first_lost_alarm =1;
	Step->never_run =0;
	Step->position = pos;
	Step->pos_2 = pos;
}

unsigned int Needle_pos_Get(void);


#ifdef STEP_WORK_SIGN_SUPPORT
void GetMotorNewStateInfo_ISR()				//获取新增的位置传感器的状态(2012-11-26 HLC)
{
  	
	MailBox3_Send(Get_SecondSign_sts());
	return;
}


#endif



void StepMotor_test_speed(unsigned int stepno,unsigned now_speed)
{
	
	arch_StepMotor_SetCompare(stepno,now_speed);
}




void StepMotor_exec_justrun(unsigned int stepno, short pos)
{	
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;
	//int zero_work_area;
	//int zero;
	//int fast_mode = mode;

	//pos = stepspeed_Q2R(pos);


	if(!is_triangle(stepno)) return;

	if(!(step_active & (0x1 << stepno)))
		return ;

	Step = &STEPMOTOR[stepno];

	
	//zero = arch_StepMotor_Zero(stepno);


	if( step_resolution != QUARTER) {
		if(pos < 0) {
			pos *= -1;
			pos = stepspeed_Q2R(pos);
			pos *= -1;
		}
		else {
			pos = stepspeed_Q2R(pos);
		}
	}
	


	if(Step->running) {
		if(Step->pos_2 != pos)
		{
			Step->pos_2 = pos;
			// 
			Step->running = RUNNING_OVER;      //新动作覆盖旧动作
			return ;
		}
		else
			return;
	}

	Step->is_reset =0;
	Step->pos_2 = pos;			//记录到达的目的地
	Step->level = 0;
	Step->phase = 0;
	//Step->speed = 0;
	Step->reset = JUST_RUN;
	// by xhl 2010/05/18
	Step->alarm_step = 0;      
	Step->check_sts_enable =0;   
	Step->dir_steps =0;
	Step->alarm_step_leave = 0;
	Step->reset_width_zero_count = 0;
       Step->need_2_pos_after_reset =0;
	
	Step->zero_count =0;
	Step->zero_go_ =0;


speed_set:
	Step->max_speed = step_max_speed[Step->motor_type_config?1:0];
	Step->speed = step_base_speed;   //index

	#ifdef USE_HZ_
	Step->ACC_hz_ = ACC_RUN_HZ_EVERY_STEP;
	#endif

	
	arch_StepMotor_Stop(stepno);

	// Half On
	arch_StepMotor_Half(stepno, 1); 

	if(Step->position > pos) {		
		Step->steps = Step->position - pos;
		Step->dir = 0;
	}
	else {		
		Step->steps = pos - Step->position;
		Step->dir = 1;
	}
	arch_StepMotor_Dir(stepno, Step->dir?Step->dir_High:!Step->dir_High);

	if(Step->steps >= (ACC_STEPS<< 1)) {
			Step->acc_steps = ACC_STEPS - 1;
		}
	else {
			Step->acc_steps = Step->steps >> 1;
		}

	
	Step->step_check_interval = 50;
	Step->running = 1;
	Step->step_wait_time = 0;	
	arch_StepMotor_Start(stepno);
	return;


}

void StepMotor_exec_with_check_mask(unsigned int stepno, short pos, int mode,unsigned short checkmask)
{
	if(!is_triangle(stepno)) return;

	if(!(step_active & (0x1 << stepno)))
		return ;
	#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
		Triangle_sts[stepno-TRIANGLE_BASE]=checkmask & 0x0F;
		Triangle_sts_reg[stepno-TRIANGLE_BASE] =Triangle_sts[stepno-TRIANGLE_BASE];
	#endif
	if(checkmask & 0x1000)
	{
		stepRunOK_return |=(1<<stepno);	
	}
	else
	{
		stepRunOK_return &=~(1<<stepno);		
	}
	
	StepMotor_exec(stepno,pos,mode);

}

void StepMotor_exec(unsigned int stepno, short pos, int mode)
{
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;
	int zero_work_area;
	int zero;
	int fast_mode = mode;

	//pos = stepspeed_Q2R(pos);


	if(!is_triangle(stepno)) return;

	if(!(step_active & (0x1 << stepno)))
		return ;

	Step = &STEPMOTOR[stepno];

	//Massage_Send_4Word(0xFDFD, unsigned short Msg2, unsigned short Msg3, unsigned short Msg4)

	
	zero = arch_StepMotor_Zero(stepno);


	if( step_resolution != QUARTER) {
		if(pos < 0) {
			pos *= -1;
			pos = stepspeed_Q2R(pos);
			pos *= -1;
		}
		else {
			pos = stepspeed_Q2R(pos);
		}
	}
	
#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
if(pos == 0) 
	{
		if(Step->never_run!=0 )
			//|| (zero && Step->position > step_zero_adj) 
		//	|| (!zero && Step->position < step_zero_adj)) 
		{
			Step->never_run =0;
			StepMotor_Reset(stepno);
			//alert_push(YARN_STEP1_ERR+stepno,99);
			return ;
		}
	}

#else
	if(pos == 0) 
	{
		if((Step->never_run!=0 )
			|| (zero && Step->position > step_zero_adj) 
			|| (!zero && Step->position < step_zero_adj)) 
		{
			Step->never_run =0;
			StepMotor_Reset(stepno);
			//alert_push(YARN_STEP1_ERR+stepno,99);
			return ;
		}
	}	

#endif

	if(Step->running) {
		if(Step->pos_2 != pos)
		{
			Step->pos_2 = pos;
			// 
			Step->running = RUNNING_OVER;      //新动作覆盖旧动作
			return ;
		}
		else
			return;
	}

	Step->is_reset =0;
	Step->pos_2 = pos;			//记录到达的目的地
	Step->level = 0;
	Step->phase = 0;
	//Step->speed = 0;
	Step->reset = JUST_RUN;
	// by xhl 2010/05/18
	Step->alarm_step = 0;      
	Step->check_sts_enable =0;   
	Step->dir_steps =0;
	Step->alarm_step_leave = 0;
	Step->reset_width_zero_count = 0;
       Step->need_2_pos_after_reset =0;
	Step->moto_count_out =0;   
	zero_work_area = step_zero_adj + ZERO_DETECT;
	

	zero_work_area = stepspeed_Q2R(zero_work_area);


	Step->zero_count =0;
	Step->zero_go_ =0;

	if(Step->position == pos) {	
			Step->check_sts_enable =1;
//	alert_push(0x40 + stepno - FEET_BASE, Step->position);
		if((stepRunOK_return >>stepno) & 0x01)
		{
			stepRunOK_return &=~(0x01L<<stepno);
			Massage_Send_4Word(0x12FD,stepno,zero,Step->position);	
		}
		return ;
	}

/*不在0位走到0位的话，都是走moveto_zero*/
	if(  ((Step->position !=0 )||( !zero)) &&  (pos ==/*<*/ 0)) 
	{
	   	Step->zero_count =0;

		if (!Get_doard_type_ex())
			{
		Step->Over_Zero_Count = zero ;
			}
		if (zero)
		{
			Step->reset = JUST_RUN;	
			
		}
		else
		{
			#ifdef ZERO_POS_MORC_UPDATE
			Step->reset = MOVETO_ZERO;	
			#else
			Step->reset = GOTO_ZERO;
			#endif
			if (Step->motor_type_config)
			{
				Step->moto_count_out =0;
				Step->reset = GOTO_ZERO;
			}
		}
		
		Step->pos_2 = pos;		
	}

#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
	if(pos==0)
	{
		if(Step->position >step_zero_adj)
		{
			if(zero)
			{
				Step->gosteps_temp =0;
				Step->reset =LEAVE_ZERO_YJ;
			}
			else
				Step->reset = GOTO_ZERO;	
		}
		else
		{
			Step->reset = JUST_RUN;	
		}
		if (!Get_doard_type_ex())
		{
			Step->Over_Zero_Count = zero ;
		}
		Step->pos_2 = pos;

	#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
		Triangle_sts[stepno-TRIANGLE_BASE]=0x05;
		Triangle_sts_reg[stepno-TRIANGLE_BASE] =Triangle_sts[stepno-TRIANGLE_BASE];
	#endif

	}

#endif

	

speed_set:
	Step->max_speed = step_max_speed[Step->motor_type_config?1:0];
	Step->speed = step_base_speed;   //index

	
	#ifdef USE_HZ_
	Step->ACC_hz_ = ACC_RUN_HZ_EVERY_STEP;
	#endif

	arch_StepMotor_Stop(stepno);

	// Half On
	arch_StepMotor_Half(stepno, 1); 

	if(Step->position > pos) {		
		Step->steps = Step->position - pos;
		Step->dir = 0;
	}
	else {		
		Step->steps = pos - Step->position;
		Step->dir = 1;
	}
	arch_StepMotor_Dir(stepno, Step->dir?Step->dir_High:!Step->dir_High);



		if((fast_mode ) && (Step->steps > stepspeed_Q2R(400))) {
		if(Step->steps > stepspeed_Q2R(800)) {
			Step->steps %= stepspeed_Q2R(800);
			//Step->dir = !Step->dir;
		}
		else {
			Step->steps = stepspeed_Q2R(800) - Step->steps;
			Step->dir = !Step->dir;
		}
		//Step->reset = 0;
		//myprintf("steps = %d\r\n", Step->steps);
		if(Step->dir)
		{
			Step->position = pos - Step->steps;
		}
		else 
		{
			Step->position = pos + Step->steps;			
		}
		arch_StepMotor_Dir(stepno, Step->dir?Step->dir_High:!Step->dir_High);

	}



	

		if(Step->steps >= (ACC_STEPS<< 1)) {
			Step->acc_steps = ACC_STEPS - 1;
		}
		else {
			Step->acc_steps = Step->steps >> 1;
		}

	//Massage_Send_4Word(0xFFFF,Step->max_speed,Step->speed,Step->acc_steps);


	
	Step->step_check_interval = 50;
	Step->running = 1;

	Step->step_wait_time = 0;
	
	arch_StepMotor_Start(stepno);
	return;

}




void StepMotor_Reset(unsigned int stepno)
{
	int iszero;
	int isleft;
	int isright;
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;


	if(stepno >= MAX_STEPMOTOR_SUPPORT) return;

	if(!(step_active & (0x1 << stepno))) return ;

	Step = &STEPMOTOR[stepno];

	arch_StepMotor_Stop(stepno);

	// by xhl 2010/08/26
	Step->never_run=0;

	if (Is_Yarn_Board())
	{
		Step->is_reset =1;
	}else
	{
		Step->is_reset =0;
	}
	Step->level = 0;
	Step->phase = 0;
	Step->speed = 0;
	Step->pos_2 = 0;
	// by xhl 2010/05/18
	Step->alarm_step = 0;
	// by xhl 2012/03/08
	Step->speed_div = 0;

	Step->is_poweroff_goon = 0;
	Step->is_first_lost_alarm = 0;
	Step->steps = Step->step_max;
	Step->reset_width_zero_count = 0;

	Step->acc_steps = ACC_STEPS - 1;
	Step->left_right_steps = 0;
	Step->is_dir =0;
	Step->can_reset_dir_step = 0;
	Step->motor_alarm_cnt=0;
	Step->adj_cnt_ex =0;
	Step->gosteps_temp=0;
	Step->change_dir_count=0;
	Step->zero_go_=0;


	#ifdef STEP_USE_HZ_
	Step->step_use_hz_max=act_step_reset_hz;
	Step->step_use_hz_start=act_step_start_hz;
	Step->step_use_hz_a = act_step_a_hz;
	Step->step_use_hz_now= act_step_start_hz;	
	Step->step_use_hz_ac_cnt = 0;
	Step->step_use_hz_phase_st = 0;
	Step->step_use_hz_T =((1000L)/(float)Step->step_use_hz_now);
	Step->step_use_hz_index =0;
	
	#endif

	#ifdef USE_HZ_
	Step->ACC_hz_ = ACC_RESET_HZ_EVERY_STEP;
	#endif
	
	
	#ifdef ZERO2_SUPPORT
	Step->zero2 = arch_stepMotor_inout_sts(Step->work_sign_index);
	Step->zero2_temp[Step->zero2?1:0]=ZERO2_DELAY_COUNT;
	Step->zero2_temp[Step->zero2?0:1]=0;
	Step->zero2_count=0;
	Step->moto_count_out =0;
	#endif

	
	#ifdef LX_ACTION_STEP_SPECIAL
	Step->LX_ACT_zero_width =0;
	if (!Is_Yarn_Board())
	{
		#ifdef ZERO2_SUPPORT
		if (Step->zero2_mode)
		{
			Step->check_zero_work = 0x0F;
		}
		else
		#endif
		Step->check_zero_work = 0x05;
	}
	#endif

      
	if(step_reset_speed[Step->motor_type_config?1:0] < step_max_speed[Step->motor_type_config?1:0])
		step_reset_speed[Step->motor_type_config?1:0] = step_max_speed[Step->motor_type_config?1:0];
	
	Step->max_speed = step_reset_speed[Step->motor_type_config?1:0];
	
	arch_StepMotor_Half(stepno, 1);
	
	iszero = arch_StepMotor_Zero(stepno);
	Step->zero = iszero;
	Step->zero_st_bit= iszero?0xff:0x00;
	if (!Is_Yarn_Board())
	{
		if (Get_doard_type_ex())
		{
			iszero = arch_StepMotor_Zero_EX(Step->zero_sign_index,Step->work_sign_index);

			if (Triangle_mode)
			{
				isleft =0;
				 isright =0;
			}
			else
			{
				isleft = Triangle_is_left_Sign_ex(Step->zero_sign_index,Step->work_sign_index);
				isright = Triangle_is_right_Sign_ex(Step->zero_sign_index,Step->work_sign_index);
			}
		}
	}
	
	if(iszero) 
	{
		#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
		if(Step->position>step_zero_adj)
		{
			Step->dir = 0;		
			Step->reset = LEAVE_ZERO_YJ;
			//Step->position = 0;	

		}
		else
		{

			Step->dir = 1;		
			Step->reset = LEAVE_ZERO;
			Step->position = 0;	
			
		}

		#else
		
		Step->dir = 1;		
		Step->reset = LEAVE_ZERO;
		Step->position = 0;	
		#endif
		if (Is_Yarn_Board())
		{
			Step->alarm_step_leave = stepspeed_Q2R(MAX_STEPS_YARN -MAX_STEPS_DOWN_ZERO_YARN);
			//stepspeed_Q2R(steps);

			if (Step->motor_type_config)
			{
				Step->alarm_step_leave = Step->alarm_step_leave>>1;
			}
		}
		else
		{
			Step->alarm_step_leave =stepspeed_Q2R( MAX_STEPS - MAX_STEPS_DOWN_ZERO);
		}
		

	}
	else 
	{
		if (Is_Yarn_Board())
		{

			
			Step->dir = 0;
			if (Step->motor_type_config)
			{
				Step->reset = GOTO_ZERO;
				Step->moto_count_out =0;
			}
			else
				{			
			#ifdef ZERO_POS_MORC_UPDATE
				#ifdef SELF_CHECK			
					Step->reset = CHECK_SIGN_WIDTH;
				#else 
					Step->reset = GOTO_ZERO;
				#endif
			#else
			Step->reset = GOTO_ZERO;
			#endif
				}

		}
		else
		{
			if (Get_doard_type_ex())
			{
				if (isright) 
				{
				
					#ifdef QL_
					Step->dir = (Step->position<0) ? 1:0;
					#else
					Step->dir = 0;
					#endif
					
				}else
				if (isleft)
				{
					
					#ifdef QL_
					Step->dir = (Step->position<0) ? 1:0;
					#else
					
					Step->dir = 1;
					#endif
				}
				else				
				{
					

					#ifdef QL_

					Step->dir = (Step->position<0) ? 1:0;
					#else
					
					Step->dir = 0;
					#endif					
				}
			}
			else
			{
				#ifdef LX_ACTION_STEP_SPECIAL

				
					Step->dir = 0;
				#else

					#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
						Step->dir = 0;	
					#else					
					#ifdef STEP_NOT_ZERO_TRY_IT
						Step->dir = 1;				//往一边走
						Step->steps = 50;				//走50步
						Step->position = 50;								
						Step->reset = GOTO_ZERO_TRY;
						goto do_TRY_gogo;
					#else
						Step->dir = 0;			
					#endif
					#endif
				#endif
						
			}
			#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
			Step->zero_go_=0;		
			Step->dir_steps =0;
			#endif
			Step->reset = GOTO_ZERO;
		}		
		
		Step->position = Step->step_max;
		
#ifdef STEP_NOT_ZERO_TRY_IT
	do_TRY_gogo:
#endif

		if (Is_Yarn_Board())
			{
				Step->alarm_step_leave = stepspeed_Q2R(MAX_STEPS_YARN - MAX_STEPS_UP_ZERO_YARN);
		}
		else
			{
				Step->alarm_step_leave =stepspeed_Q2R( MAX_STEPS - MAX_STEPS_UP_ZERO);
		}
		
		
	}
	arch_StepMotor_Dir(stepno,(Step->dir) ? (Step->dir_High) : (!Step->dir_High));
	
{
	int i;
	for (i=0;i<3;i++)
	{
		Step->last_zero_sts[i] = iszero;
	}
}

	Step->step_check_interval = 50;
	Step->running = 1;
	Step->zero_count = 0;
	Step->zero_go_ =0;
	

	Step->step_wait_time = 0;

#ifdef TRIANGLE_STEP_SUPPORT 
	Step->done_steps = 0;				//已经执行的步数 针对 三角电机
	Step->last_rest= 0;	
	Step->change_dir_count = 0;
	if (!Get_doard_type_ex())
	{
		Step->Over_Zero_Count = 0;
	}
	Step->dir_steps =0;
	if (is_triangle(stepno)){

		if (Triangle_mode)
			Triangle_sts[stepno-TRIANGLE_BASE]=0x01;
		else		
			Triangle_sts[stepno-TRIANGLE_BASE]=0x03;
#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
		Triangle_sts[stepno-TRIANGLE_BASE]=0x05;
		
#endif
		Triangle_sts_reg[stepno-TRIANGLE_BASE] =Triangle_sts[stepno-TRIANGLE_BASE];
	}



	
#endif
     arch_StepMotor_Start(stepno);

}

#ifdef ZERO2_SUPPORT_old
int is_zero2_support(unsigned int stepno)
{
	return step_zero2_enable & (0x1 << stepno);
}
void StepMotor_Detect_Zero2(unsigned int stepno)
{
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;
	int i;
	int steps;

	if(stepno >= MAX_STEPMOTOR_SUPPORT) return;

	if(!(step_active & (0x1 << stepno)))
		return ;

	if(!is_sinker(stepno)) {
		return ;
	}

	//if(!arch_StepMotor_Zero(stepno + 2)) {
	//	return ;
	//}

	Step = &STEPMOTOR[stepno];

	if(Step->position <= sinker_zero_area &&
	   !arch_StepMotor_Zero(stepno)) {
		goto exec_it;
	}

	//if(arch_StepMotor_Zero(stepno)) {
	//	StepMotor_Reset(stepno);
	//	return ;
	//}

exec_it:
	arch_StepMotor_Stop(stepno);

	// by xhl 2010/08/26
	Step->never_run = 0;
	//step_poweron &= ~(0x1 << stepno);

	Step->level = 0;
	Step->phase = 0;
	Step->speed = 0;
	Step->pos_2 = 0;
	// by xhl 2010/05/18
	Step->alarm_step = 0;
	// by xhl 2012/03/08
	Step->speed_div = 0;

	steps = step_setup_zero2_detect_steps;
	Step->steps = steps;
	Step->acc_steps = (steps + 1) >> 1;

	arch_StepMotor_Half(stepno, 1);

	
	Step->dir = 1;
	arch_StepMotor_Dir(stepno, Step->dir?Step->dir_High:!Step->dir_High);

	Step->max_speed = STEP_LOW_SPD/*step_reset_speed*/;
	Step->reset = DETECT_ZERO2;

	arch_StepMotor_Stop(stepno);
	Step->step_check_interval = 200/*50*/;
	Step->running = 1;
	Step->zero_count = 0;
	//myprintf("start dir %d, step %d\r\n", Step->dir, Step->steps);
	arch_StepMotor_Start(stepno);
	//Step_Zero_Check_Delay[stepno] = 2/*4*/;
#ifdef ENCODER_SUPPORT
	encoder_init &= ~(0x1 << stepno);
	Step->step_delay = ENCODER_CHECK_DELAY_RST;
	Step->enc_err_count = 0;
#endif
	Step->step_wait_time = 0;
	Step->zero2 = arch_StepMotor_Zero(stepno + 2);
	Step->zero2_count = 0;
}
#endif

void StepMotor_All_Reset()
{
	unsigned int i;

	for(i = 0; i < 8; i ++) {
		if(vary_step_enable & (0x1 << i))
			continue;
		StepMotor_Reset(i);
	}
}

void StepMotor_Reset_LeftMotor()
{
	unsigned int i;

	for(i = 0; i < 8; i += 2) {
		StepMotor_Reset(i);
	}
}

void StepMotor_Reset_RightMotor()
{
	unsigned int i;

	for(i = 1; i < 8; i += 2) {
		StepMotor_Reset(i);
	}
}

#ifdef TRIANGLE_STEP_SUPPORT 

#ifdef ZERO2_SUPPORT
#define LX_POS_EACH_POS (97)
#define LX_POSCH (47)


short Check_zero2_count_with_pos_pos2(short pos1,short pos2)
{
	short pos1_pos2=pos1-pos2;
	unsigned int i;
if (pos1_pos2<0)
{
	pos1_pos2 *=-1;
}

if (pos1_pos2<=(LX_POS_EACH_POS-LX_POSCH))
{
	return 0;
}
else

for (i=1;i<9;i++)
{
	if (pos1_pos2<=(i*LX_POS_EACH_POS+LX_POSCH))
	{
		return i*2;
	}
	else
		continue;
}

return 0;
		

}


#endif


int is_triangle(int stepno)
{
if (Is_Yarn_Board()){
	if((stepno >= TRIANGLE_BASE) &&
	   (stepno < TRIANGLE_BASE + 8)) {
		return 1;
	}
	return 0;
}
else
{
if((stepno >= TRIANGLE_BASE) &&
	   (stepno < TRIANGLE_BASE + MOTOR_COUNT_MAX)) {
		return 1;
	}
	return 0;
}
}

int a=1,val_num =0,com_val=1,cla_val=0;
void step_test()
{
	
	val_num++;
	cla_val = 65536 -com_val;
//	if(val_num>1){
		a =!a;
	
		GPIO_WriteBit(GPIOA,GPIO_Pin_6,a);
		val_num =0;
//	}
	
}

void step_isr(int stepno)
{
	int i;

	if( stepno >= MAX_STEPMOTOR_SUPPORT) {   //MAX_STEPMOTOR_SUPPORT=8,最大8个，判断电机编号是否超过最大值
		return ;
	}
	if (Is_Yarn_Board())
	{ 
		StepMotor_Isr(stepno) ;
	}
	else{
			StepMotor_Triangle_Isr((unsigned int)stepno);
		}
	

}

void StepMotor_Triangle_exec(unsigned int stepno, short pos, int mode)
{
	
STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;
	int zero_work_area;
	int zero;

	
	/*    mode 的bit0 表示是否转换细分步数
				 bit1表示电机运行模式(0-按照位置走.1-按照po走)
	*	if (mode & 0x2 ==0) 说明 pos<0  走到最左边，pos>0走到最右边 pos=0 走到中间
	*	if (mode & 0x2 !=0) 说明按照pos走
	*/


	if(!is_triangle(stepno)) return;

	if(!(step_active & (0x1 << stepno)))
		return ;

	Step = &STEPMOTOR[stepno];


	//Massage_Send_4Word(0xBBBB,stepno,pos,mode?0xF001:0x0001);

	if (!Get_doard_type_ex())
		{
	
	zero = arch_StepMotor_Zero(stepno);

	}else
		{

	zero = arch_StepMotor_Zero_EX(Step->zero_sign_index,Step->work_sign_index);
	
		}


	if((mode & 0x1) && (step_resolution != QUARTER)) {
		if(pos < 0) {
			pos *= -1;
			pos = stepspeed_Q2R(pos);
			pos *= -1;
		}
		else {
			pos = stepspeed_Q2R(pos);
		}
	}	

	if(pos == 0) {

		#ifndef LX_

		if (!Get_doard_type_ex())  //默认走归零的流程
		{

			if((Step->never_run)
			|| (zero && Step->position >=20) 
			|| (!zero && Step->position <=0)) 
			{	
				StepMotor_Reset(stepno);
				return ;
			}				
//	alert_push(0x20 + stepno - FEET_BASE, Step->position);
			
		}

		else
		#else
		if (!Get_doard_type_ex())  //默认走归零的流程
		{
			if((Step->never_run)
			|| (zero && Step->position >=20) 
			|| (!zero && Step->position <=0)) {
			//step_poweron &= ~(0x1 << stepno);
//		reset_it:
			StepMotor_Reset(stepno);
//	alert_push(0x20 + stepno - FEET_BASE, Step->position);
			return ;
			}
		}
		else	
		
		#endif	
		{
			if((Step->never_run)
				|| (zero && Step->position != 0) 
				|| (!zero && Step->position == 0)) 
			{
				//step_poweron &= ~(0x1 << stepno);
		//		reset_it:
				StepMotor_Reset(stepno);
		//	alert_push(0x20 + stepno - FEET_BASE, Step->position);
				return ;
			}
		}
	}	

	if(Step->running) {
		if(Step->pos_2 != pos)
		{
			Step->pos_2 = pos;
			// 
			Step->running = RUNNING_OVER;      //新动作覆盖旧动作
			return ;
		}
		else
			return;
	}

	Step->pos_2 = pos;			//记录到达的目的地
	Step->level = 0;
	Step->phase = 0;
	//Step->speed = 0;
	Step->reset = 0;
	// by xhl 2010/05/18
	Step->alarm_step = 0;      
	Step->check_sts_enable =0;   

	
	Step->dir_steps =0;
	Step->left_right_steps = 0;
	Step->is_dir =0;
	Step->can_reset_dir_step = 0;
	Step->zero = zero;
	Step->zero_st_bit= zero?0xff:0x00;
	Step->zero_go_ =0;
	Step->zero_count = 0;
	 Step->last_speed = 0;

	#ifdef STEP_USE_HZ_
	Step->step_use_hz_max=act_step_max_hz;
	Step->step_use_hz_start=act_step_start_hz;
	Step->step_use_hz_a = act_step_a_hz;
	Step->step_use_hz_now= act_step_start_hz;	
	Step->step_use_hz_ac_cnt = 0;
	Step->step_use_hz_phase_st = 0;
	Step->step_use_hz_T =((1000L)/(float)Step->step_use_hz_now);
	Step->step_use_hz_index=0;
	memset(sys_debug_hz,0,sizeof(sys_debug_hz));
	memset(sys_debug_T,0,sizeof(sys_debug_T));
	
	#endif


	 
	#ifdef ZERO2_SUPPORT

	Step->zero2 = arch_stepMotor_inout_sts(Step->work_sign_index);

		
	Step->zero2_count=Check_zero2_count_with_pos_pos2(Step->position,Step->pos_2);
	
	Step->zero2_temp[Step->zero2?1:0]=ZERO2_DELAY_COUNT;
	Step->zero2_temp[Step->zero2?0:1]=0;
	

	#endif

	if (!Get_doard_type_ex())
	{
		Step->Over_Zero_Count = 0;
	}

	zero_work_area = step_zero_adj + ZERO_DETECT;
	

	zero_work_area = stepspeed_Q2R(zero_work_area);




	if(Step->position == pos) {	
		if ((Step->is_poweroff_goon)&&(Step->is_first_lost_alarm)&&(Step->motor_type_config))
		{
			return ;
		}
		Step->check_sts_enable =1;
		#ifdef LX_
		{
			extern void Massage_Send_4Word(unsigned short Msg1,unsigned short Msg2,unsigned short Msg3,unsigned short Msg4);
			//Massage_Send_4Word(0x73,Get_stepno_with_motorID(stepno),0xEEEE,0xEEEE);
		}
		#endif

		return ;
	}

	if (!Get_doard_type_ex())
	{
		#ifndef LX_
		Step->adj_cnt =0;
		Step->adj_cnt_ex =0;
		#if 0
		Step->adj_cnt_steps[0]=0;
		Step->adj_cnt_steps[1]=0;
		Step->adj_cnt_steps[2]=0;
		Step->adj_cnt_steps[3]=0;
		#endif
		Step->lastposition = Step->position;
		
		if (((Step->position>0)&&(pos<=0))||((Step->position<=0)&&(pos>0)))
		{
			Step->adj_cnt = 1;
			//Step->adj_cnt_ex =1;
		}
		
		if ((pos ==/*<*/ 0)&&((Step->position!=0)))
		{
  			Step->zero_count =0;
			Step->Over_Zero_Count = zero ;
			Step->reset = MOVETO_ZERO;
			Step->pos_2 = pos;	
			if (Step->motor_type_config)
			{
				Step->max_speed = STEP_MOVE_ZERO_LOW_SPEED;
				goto speed_set_ok;
			}
		}

		
		
		#else

		#ifdef LX_ACTION_STEP_SPECIAL

		if (pos==0)
		{
			if ((Step->position>0)&&(!zero))   //
			{
				Step->reset = MOVETO_ZERO;
				Step->pos_2 = pos;
			}
			if (((Step->position<0)&&(!zero))||((Step->position<-100)&&(zero)))
			{
				Step->reset = MOVETO_ZERO_LX_ACT;
				Step->pos_2 = pos;
			}
		}



		#else		
		if ((pos ==/*<*/ 0)&&((Step->position>0)))
		{
  			Step->zero_count =0;
			Step->Over_Zero_Count = zero ;
			Step->reset = MOVETO_ZERO;
			Step->pos_2 = pos;	
		}		
		#endif
		#endif

	}
	else
	{

/*不在0位走到0位的话，都是走moveto_zero*/
		if(  ((Step->position !=0 )||( !zero)) &&  (pos ==/*<*/ 0)) 
		{
		   	Step->zero_count =0;
			Step->reset = MOVETO_ZERO;
			Step->pos_2 = pos;		
		}

	}

#if 0
	if ( (mode & 0x0002)==0)
	{
		if (pos<0 ) {
			Step->reset = GOTO_LEFT;
		}
		if (pos>0 ){
			Step->reset = GOTO_RIGHT;
		}
	}
#endif	

speed_set:
	
	Step->max_speed = step_max_speed[Step->motor_type_config?1:0];
speed_set_ok:	
	Step->speed = step_base_speed;

	#ifdef USE_HZ_
	Step->ACC_hz_ = ACC_RUN_HZ_EVERY_STEP;
	#endif


#ifdef STEP_USE_HZ_


#endif

	

	// Half On
	arch_StepMotor_Half(stepno, 1);

	if(Step->position > pos) {		
		Step->steps = Step->position - pos;
		Step->dir = 0;
	}
	else {		
		Step->steps = pos - Step->position;
		Step->dir = 1;
	}
	arch_StepMotor_Dir(stepno, Step->dir?Step->dir_High:!Step->dir_High);


		if(Step->steps >= (ACC_STEPS_TRIANGLE<< 1)) {
			Step->acc_steps = ACC_STEPS_TRIANGLE - 1;
		}
		else {
			Step->acc_steps = Step->steps >> 1;
		}



	arch_StepMotor_Stop(stepno);
	Step->step_check_interval = 50;
	Step->running = 1;
	//Step->zero_count = 0;

	Triangle_sts_reg[stepno-TRIANGLE_BASE] = Triangle_sts[stepno-TRIANGLE_BASE];
	
	arch_StepMotor_Start(stepno);




	Step->step_wait_time = 0;

	return;

}


#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
void check_triangle_sts_CX_YJ(void)
{
	int i;
	unsigned int is_temp;
	
	

	for (i=0; i < MOTOR_COUNT_MAX; i++)
	{
		if ((STEPMOTOR[i + TRIANGLE_BASE].steps == 0) 
				&&(STEPMOTOR[i + TRIANGLE_BASE].running ==0)
					&&(STEPMOTOR[i + TRIANGLE_BASE].check_sts_enable))
		{

			if(Triangle_sts_reg[i] & 0x04)
			{
				unsigned char checkst=0;
				checkst = Triangle_sts_reg[i] & (0x01);
				if(arch_StepMotor_Zero(i)!=checkst)
				{
					triangle_err_count[i]++;
					if(triangle_err_count[i] > DEF_TRIANGLE_ERR_CHECK_TIME)
					{
						alert_push(STEP_ERR_CODE(TRIANGLE_BASE +Get_stepno_with_motorID(i)), 1);
						triangle_err_count[i] = 0;
						STEPMOTOR[i + TRIANGLE_BASE].check_sts_enable = 0;
						STEPMOTOR[i + TRIANGLE_BASE].motor_alarm_cnt++;
					}
				}
				else
				{
					triangle_err_count[i] =0;
				}				
			}							
		}
	}



}



#endif


void check_triangle_sts(void)
{
	int i;
	unsigned int is_temp;
	
	

	for (i=0; i < MOTOR_COUNT_MAX; i++)
	{
		if ((STEPMOTOR[i + TRIANGLE_BASE].steps == 0) 
				&&(STEPMOTOR[i + TRIANGLE_BASE].running ==0)
					&&(STEPMOTOR[i + TRIANGLE_BASE].check_sts_enable))
		{
				if ((STEPMOTOR[i + TRIANGLE_BASE].is_poweroff_goon)&&(STEPMOTOR[i + TRIANGLE_BASE].is_first_lost_alarm)&&(STEPMOTOR[i + TRIANGLE_BASE].motor_type_config))
				{
					continue;
				}

				if((STEPMOTOR[i + TRIANGLE_BASE].position<50)&&(STEPMOTOR[i + TRIANGLE_BASE].position>-50))
				{
					continue;
				}

			switch (Triangle_sts_reg[i] & (0x03)){
				case 01:
						{						
						if (!Get_doard_type_ex())
						{
							is_temp = !Triangle_is_left_Sign(TRIANGLE_BASE + i);
						}
						else
						{
							if (Triangle_mode)
							{
								is_temp =  !arch_stepMotor_inout_sts(STEPMOTOR[i + TRIANGLE_BASE].zero_sign_index);
							}
							else
								is_temp = !Triangle_is_left_Sign_ex(STEPMOTOR[i + TRIANGLE_BASE].zero_sign_index,STEPMOTOR[i + TRIANGLE_BASE].work_sign_index);
						}
						
						if (is_temp)
						{
							triangle_err_count[i]++;
							if(triangle_err_count[i] > DEF_TRIANGLE_ERR_CHECK_TIME)
							{
								alert_push(STEP_ERR_CODE(TRIANGLE_BASE +Get_stepno_with_motorID(i)), 1);
								triangle_err_count[i] = 0;
								STEPMOTOR[i + TRIANGLE_BASE].check_sts_enable = 0;
								STEPMOTOR[i + TRIANGLE_BASE].motor_alarm_cnt++;
							}
						}
						else
							{
							triangle_err_count[i] = 0;
							}
						}
						break;
				case 02:
						{
							if (!Get_doard_type_ex())
							{
								is_temp = !Triangle_is_right_Sign(TRIANGLE_BASE + i);
							}
							else
							{
								if (Triangle_mode)
								{
									is_temp = !arch_stepMotor_inout_sts(STEPMOTOR[i + TRIANGLE_BASE].work_sign_index);

								}
								else
								{
									is_temp = !Triangle_is_right_Sign_ex(STEPMOTOR[i + TRIANGLE_BASE].zero_sign_index,STEPMOTOR[i + TRIANGLE_BASE].work_sign_index);
								}
							}
						
						if (is_temp)						
						{
							triangle_err_count[i]++;
							if(triangle_err_count[i] > DEF_TRIANGLE_ERR_CHECK_TIME)
							{
								alert_push(STEP_ERR_CODE(TRIANGLE_BASE + Get_stepno_with_motorID(i)), 20);
								triangle_err_count[i] = 0;
								STEPMOTOR[i + TRIANGLE_BASE].check_sts_enable = 0;
								STEPMOTOR[i + TRIANGLE_BASE].motor_alarm_cnt++;
							}
						}
						else
							{
							triangle_err_count[i] = 0;
							}
						}
						break;
				case 03:
						{

							if (!Get_doard_type_ex())
						{
							is_temp = !arch_StepMotor_Zero(TRIANGLE_BASE + i);
						}
						else
							{
								if (Triangle_mode)
								{
									is_temp = !((arch_stepMotor_inout_sts(STEPMOTOR[i + TRIANGLE_BASE].zero_sign_index))&&(arch_stepMotor_inout_sts(STEPMOTOR[i + TRIANGLE_BASE].work_sign_index)));
								}
								else
							
								is_temp = !arch_StepMotor_Zero_EX(STEPMOTOR[i + TRIANGLE_BASE].zero_sign_index,STEPMOTOR[i + TRIANGLE_BASE].work_sign_index);
						}
						
						if (is_temp)						
						{
							triangle_err_count[i]++;
							if(triangle_err_count[i] > DEF_TRIANGLE_ERR_CHECK_TIME)
							{
								alert_push(STEP_ERR_CODE(TRIANGLE_BASE + Get_stepno_with_motorID(i)), 3);
								triangle_err_count[i] = 0;
								STEPMOTOR[i + TRIANGLE_BASE].check_sts_enable = 0;
								STEPMOTOR[i + TRIANGLE_BASE].motor_alarm_cnt++;
							}
						}
						else
							{
							triangle_err_count[i] = 0;
							}
						}
						break;
				default:
					break;
				}				
		}
	}



}

#ifdef LX_

void check_triangle_sts_ex(void)
{
	int i;
	unsigned int is_zero;
	unsigned int is_work;
	unsigned int is_alert;
	
	
	

	for (i=0; i < MOTOR_COUNT_MAX; i++)
	{
		if ((STEPMOTOR[i + TRIANGLE_BASE].steps == 0) 
				&&(STEPMOTOR[i + TRIANGLE_BASE].running ==0)
					&&(STEPMOTOR[i + TRIANGLE_BASE].check_sts_enable))
		{
			is_zero =  arch_stepMotor_inout_sts(STEPMOTOR[i + TRIANGLE_BASE].zero_sign_index);
			is_work = arch_stepMotor_inout_sts(STEPMOTOR[i + TRIANGLE_BASE].work_sign_index);

			if (STEPMOTOR[i + TRIANGLE_BASE].position==0)
			{
				is_alert = is_zero? 0:1;
			}
			else if (STEPMOTOR[i + TRIANGLE_BASE].position<0)
			{
				
				is_alert = (is_zero? 0:1)|((is_work?1:0)<<1)|((STEPMOTOR[i + TRIANGLE_BASE].zero2_count/*==0 ? 0:1*/)<<2);

			}
			else
			{

				is_alert = (is_zero? 1:0)|((is_work?1:0)<<1)|((STEPMOTOR[i + TRIANGLE_BASE].zero2_count/*==0 ? 0:1*/)<<2);

			}
			if (is_alert)
			{
				triangle_err_count[i]++;
				if(triangle_err_count[i] > DEF_TRIANGLE_ERR_CHECK_TIME)
				{
					alert_push(STEP_ERR_CODE(TRIANGLE_BASE +Get_stepno_with_motorID(i)), (0xf<<12)|(is_alert));
					triangle_err_count[i] = 0;
					STEPMOTOR[i + TRIANGLE_BASE].check_sts_enable = 0;
				}
			}
			else
			{
				triangle_err_count[i] = 0;
			}
					
		}
	}



}

#endif
//#ifdef TRIANGLE_STEP_SUPPORT 

#ifdef STEP_USE_HZ_


void act_step_use_hz_speed_op(STEP_TYPE *Step)
{
	
	if ((Step->steps<=Step->step_use_hz_ac_cnt) ||(Step->step_use_hz_phase_st ==2)) //开始减速
	{
		if (Step->step_use_hz_phase_st !=2)
			Step->step_use_hz_phase_st =2;//
		Step->step_use_hz_now -=(unsigned int)(Step->step_use_hz_a* Step->step_use_hz_T );
		if (Step->step_use_hz_now<Step->step_use_hz_start)
			Step->step_use_hz_now =Step->step_use_hz_start; 
		goto return_T; 
	}
	
		
	if (Step->step_use_hz_now>Step->step_use_hz_max)			//最高速降下来了
	{
		Step->step_use_hz_now -= (unsigned int)((long)Step->step_use_hz_a* Step->step_use_hz_T );		
		if (Step->step_use_hz_now<Step->step_use_hz_max)
			Step->step_use_hz_now = Step->step_use_hz_max;
		if (Step->step_use_hz_ac_cnt)
			Step->step_use_hz_ac_cnt--;
	}
	else
		if (Step->step_use_hz_now==Step->step_use_hz_max)			//加速完成
		{
			if (Step->step_use_hz_phase_st!=1)
				Step->step_use_hz_phase_st=1;	
			goto return_T ;
		}
		else															//加速
		{
			Step->step_use_hz_now += (unsigned int)((long)Step->step_use_hz_a*Step->step_use_hz_T);
			if (Step->step_use_hz_now>Step->step_use_hz_max)
			{
				Step->step_use_hz_now = Step->step_use_hz_max;
			}
			Step->step_use_hz_ac_cnt++;
				if (Step->step_use_hz_phase_st!=0)
				Step->step_use_hz_phase_st=0;	
			goto return_T;
		}

	return_T:
	Step->step_use_hz_T=((1000L)/(float)Step->step_use_hz_now);

	if (Step->step_use_hz_index<600)
	{
		sys_debug_hz[Step->step_use_hz_index]=Step->step_use_hz_now;
		sys_debug_T[Step->step_use_hz_index++]=Step->step_use_hz_T*1000;
	}
	

}

void Step_debug_get_hz_t()
{
	int i;
	for (i=0;i<600;)
	{
		Massage_Send_4Word_log(i,sys_debug_hz[i],sys_debug_hz[i+1],sys_debug_hz[i+2]);
		i=i+3;
	}
	for (i=0;i<600;)
	{
		Massage_Send_4Word_log(i,(unsigned short)sys_debug_T[i],(unsigned short)sys_debug_T[i+1],(unsigned short)sys_debug_T[i+2]);
		i=i+3;
	}

}


#endif


#define DIR_CCR_27	27
#define DIR_CR_16	16

void StepMotor_Triangle_Isr(unsigned int stepno)
{
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;
	unsigned int speed;
	unsigned char need_change_dir=0;
	unsigned short error_no=0;
	
	#ifdef USE_HZ_
	unsigned int Now_HZ_;
	#endif
	
	int zero = 0;
	int isleft = 0;
	int isright = 0;
	int needslow_speed=0;
	int zero_go_=0;
	

	//int StepMotor_Feet_Work_Sign(int feet_no);
	
	if (!is_triangle(stepno)) return;
	
	if(stepno >= MAX_STEPMOTOR_SUPPORT) return;

	Step = &STEPMOTOR[stepno];

	if(Step->running == 0 || Step->steps == 0) {          //说明走完了，该停下来了。
		Step->running = 0;
		arch_StepMotor_Stop(stepno);
		goto exit;
	}
	//Step->step_check_interval = 50;
	if(Step->step_wait_time) {
		arch_StepMotor_Set_Speed(stepno, PULSE_LOW_TRIANGLE);
		goto done;
	}

	if(Step->level == 0) {
		Step->level = 1;
		arch_StepMotor_Pulse(stepno, 1);

		arch_StepMotor_Set_Speed(stepno, PULSE_LOW_TRIANGLE);
		goto done;
	}

	#ifdef LX_ACTION_STEP_SPECIAL
	zero_go_ = (Step->moto_zero_width_self - Step->moto_zero_deviation)>>1;
	zero_go_ +=(Step->dir?(-1):(1))*STEP_ZERO_GOON_STEPS;
	
	#endif

	//step_alert_detect = step_alert_detect_setup;

	Step->level = 0;

	arch_StepMotor_Pulse(stepno, 0);
	// by xhl 2011/04/20
	if (!Get_doard_type_ex())
		{
	zero = arch_StepMotor_Zero(stepno);
	isleft = Triangle_is_left_Sign(stepno);
	isright = Triangle_is_right_Sign(stepno);
	}else
		{
	zero = arch_StepMotor_Zero_EX(Step->zero_sign_index,Step->work_sign_index);
	isleft = Triangle_is_left_Sign_ex(Step->zero_sign_index,Step->work_sign_index);
	isright = Triangle_is_right_Sign_ex(Step->zero_sign_index,Step->work_sign_index);

	#ifdef LX_
	
	isleft = 	 arch_StepMotor_Zero_EX(Step->work_sign_index,0); //work sig

	#ifdef ZERO2_SUPPORT
	if (Step->zero2_temp[isleft?1:0]<ZERO2_DELAY_COUNT)
	{
		Step->zero2_temp[isleft?1:0]++;
		Step->zero2_temp[isleft?0:1]=0;
	}
	{
		unsigned int iszero_temp=Step->zero2;
		if (Step->zero2_temp[0]>=ZERO2_DELAY_COUNT)
		{
			iszero_temp=0;
		}
		if (Step->zero2_temp[1]>=ZERO2_DELAY_COUNT)
		{
			iszero_temp=1;
		}		
		if (Step->zero2 != iszero_temp)
		{
		
			Step->zero2_count--;
			Step->zero2 = iszero_temp;
		}
	}
	#endif
	#endif
	
		}

	

	
	Step->done_steps ++;
	Step->dir_steps ++;


	//if (Step->reset != GOTO_ZERO ) {
		
	if( (Step->done_steps >= Step->step_max) ||((Step->change_dir_count >=2)))   //shuoming zou de tai yuan le .yao ting xia lai 
	{
		Step->steps = 0;
		goto steps_is_0;
	}
		//}
	#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
		if(Step->dir) 
		{
			Step->position ++;
		}
		else
		{
			Step->position --;
		}

	#else

	if(Step->dir) {
		Step->position ++;

		#ifdef CX_
		if ((!Is_Yarn_Board()) && (!Get_doard_type_ex())&&(Step->motor_type_config==0))	
		{
			if (Step->position>=135)
			{
				Step->need_slow_speed++;
				needslow_speed = 1;
				Step->phase_slow=1;
				if (Step->need_slow_speed>=ACC_STEPS_TRIANGLE_EX)
				{
					Step->need_slow_speed =ACC_STEPS_TRIANGLE_EX;
				}
				//if (Step->phase==1)
				{
				//	Step->phase=2;
				}
			}

		}


		#endif


		
		#ifdef LX_
		if ((!Is_Yarn_Board()) && (Get_doard_type_ex()) &&(Triangle_mode))
		{
		if ((Step->position>=388) &&(Step->position<=485))
		{
			needslow_speed =1;
		}
		}
		else
		if ((!Get_doard_type_ex())&&(!Is_Yarn_Board()))
		{
			if ((Step->position>=-194) &&(Step->position<=-97))
			{
				needslow_speed =1;

				#ifdef USE_HZ_
				if (Step->position==-194)
				{
					Step->slow_steps_=1;
				}
				else
				if (Step->position<(-194+SLOW_ACC_STEPS_HZ_))
				{
					Step->slow_steps_++;
				}

				if (Step->position==-97-SLOW_ACC_STEPS_HZ_)
				{
					Step->slow_steps_=SLOW_ACC_STEPS_HZ_;
				}
				else
				if (Step->position>(-97-SLOW_ACC_STEPS_HZ_))
				{
					Step->slow_steps_--;
				}
				
					
				#endif
			}
		}
		#endif
	}
	else {
		Step->position --;

		#ifdef CX_
		if ((!Is_Yarn_Board()) && (!Get_doard_type_ex())&&(Step->motor_type_config==0))	
		{
			if ((Step->position<=135) &&(Step->position>=0))
			{
				Step->need_slow_speed++;
				needslow_speed = 1;
				
				if (Step->position<=ACC_STEPS_TRIANGLE_EX)
				{
					if (Step->phase_slow==1)
					{						
						Step->need_slow_speed =0;
					}
					Step->phase_slow=2;
				}
				else
					Step->phase_slow=1;
				
				if (Step->need_slow_speed>=ACC_STEPS_TRIANGLE_EX)
				{
					Step->need_slow_speed =ACC_STEPS_TRIANGLE_EX;
				}

			#if 0
				if (Step->position=135)
				{
					if (Step->phase==1)
					{
						Step->phase=2;
					}
				}
				if (Step->position=0)
				{
					if (Step->phase==1)
					{
						Step->phase=0;
					}
				}
				#endif
			}

		}
		#endif

		
		#ifdef LX_
		if ((!Is_Yarn_Board()) && (Get_doard_type_ex()) &&(Triangle_mode))
		{
			if ((Step->position<=194) &&(Step->position>=97))
			{
				needslow_speed =1;
			}
		}
		else
			
		if ((!Get_doard_type_ex())&&(!Is_Yarn_Board()))
		{
		#if 0
			if ((Step->position>=97) &&(Step->position<=194))
			{
				needslow_speed =1;
				#ifdef USE_HZ_
				if (Step->position==194)
				{
					Step->slow_steps_=1;
				}
				else
				if (Step->position>(194-SLOW_ACC_STEPS_HZ_))
				{
					Step->slow_steps_++;
				}

				if (Step->position==97+SLOW_ACC_STEPS_HZ_)
				{
					Step->slow_steps_=SLOW_ACC_STEPS_HZ_;
				}
				else
				if (Step->position<(97+SLOW_ACC_STEPS_HZ_))
				{
					Step->slow_steps_--;
				}
				
					
				#endif
			}
		#else
		;
		#endif	
		}
		

		#endif
	}

	#endif

	Step->steps --;


	Step->zero_st_bit <<=1;
	Step->zero_st_bit |=zero?1:0;

	if (!zero)
	{
		if (Step->zero_cnt)
		{
			Step->zero_cnt++;
		}
		else
		{
			if (Step->zero)
			{
				Step->zero_cnt++;
			}
		}
		Step->zero_cnt_ex =0;
	}
	else
	{
		if (Step->zero_cnt_ex)
		{
			Step->zero_cnt_ex++;
		}
		else
		{
			if (!Step->zero)
			{
				Step->zero_cnt_ex++;
			}
		}
		Step->zero_cnt =0;
	}
	
	
	


	//#ifdef LX_
	if (!Is_Yarn_Board()) 
	{
		if ((Step->reset==0)||(Step->reset==MOVETO_ZERO))
		{
			if (Step->dir?((Step->zero_st_bit&0x1F )==0x10):((Step->zero_st_bit&0x1F )==0x0F))
			{	
				/*if ((Step->reset==0)&&(Step->adj_cnt_ex<4))
				{
					Step->adj_cnt_steps[Step->adj_cnt_ex++]=abs(Step->lastposition - Step->position);
					Step->lastposition = Step->position;
				}*/
				if (Step->adj_cnt)
				{
					int errsteps_=0;						

					if (Step->dir)
					{
						if (Step->position>DIR_CCR_27)
						{
							errsteps_ = Step->position-DIR_CCR_27;		// 2--表示本次失步了"+"
							//errsteps_ = 222;
							errsteps_ |=(2<<12);
							
						}
						else
						{
							errsteps_ = DIR_CCR_27-Step->position;		// 3 --表表示上一次失步了"-"
							//errsteps_ = 333;
							errsteps_ |=(3<<12);
							
						}

					}
					else
					{
					
						if (Step->position<DIR_CR_16)
						{
							errsteps_ = DIR_CR_16-Step->position;		// 2--表示本次失步了"+"
							//errsteps_ = 444;
							errsteps_ |=(2<<12);
							
						}
						else
						{
							errsteps_ = Step->position-DIR_CR_16;		// 3 --表表示上一次失步了"-"
							//errsteps_ = 111;
							errsteps_ |=(3<<12);
							
						}
					}
					
					
					
					if ((Step->is_poweroff_goon)&&(Step->is_first_lost_alarm)&&(Step->motor_type_config)) /*第一次把失步值放大一倍*/
					{
						int Steps_lost_alarm_temp=Steps_lost_alarm<<1;
						if (((errsteps_&0xFFF)>=Steps_lost_alarm_temp)&&(Steps_lost_alarm)&&(!Step->dir))
						{
							alert_push(STEP_ERR_CODE(Get_stepno_with_motorID(stepno)),errsteps_);	
							
						}
						Step->is_first_lost_alarm = 0;

					}
					else					
					if (((errsteps_&0xFFF)>=Steps_lost_alarm)&&(Steps_lost_alarm)&&(!Step->dir))
					{
						 
						alert_push(STEP_ERR_CODE(Get_stepno_with_motorID(stepno)),errsteps_);	
					}
					
					Step->position=(Step->dir)?(DIR_CCR_27):DIR_CR_16;
					//if 
					Step->steps =Step->position>=Step->pos_2?(Step->position-Step->pos_2):(Step->pos_2-Step->position);
					#ifdef LX_
					if ((Get_doard_type_ex()) &&(Triangle_mode))
						Step->zero2_count=Check_zero2_count_with_pos_pos2(Step->position,Step->pos_2);
					#endif
					Step->adj_cnt=0;					
				}

			
				
					
			}
		}
	}
	//#endif

	
	//if(Step->reset) {
		
		switch(Step->reset) {

		case JUST_RUN:
			{
				if(Step->running == RUNNING_OVER) 
				{
					Step->steps = 0;
					goto exit;
				}	
				break;				
			}	
		case GOTO_ZERO_TRY:
			{
				if (zero)
				{
					Step->steps = Step->step_max;
					Step->reset = LEAVE_ZERO;
				}
				else
				{
					if (Step->steps == 0)  //试着走走，结果走完了，没找到零位,那要调头了
					{
						Step->steps = Step->step_max;
						Step->speed = 0;
						Step->phase = 0;
						Step->dir_steps = 0;
						Step->change_dir_count++;
						Step->dir = !Step->dir ;
						need_change_dir =1;
						Step->step_wait_time = step_reset_delay_time[stepno];
						//arch_StepMotor_Dir(stepno, Step->dir ? Step->dir_High :!Step->dir_High);
						Step->reset = GOTO_ZERO;
					
					}
				}

			}
			break;
		case GOTO_ZERO:
			#ifdef LX_

			if ((!Is_Yarn_Board()) && (Get_doard_type_ex()) &&(Triangle_mode))
			{
				if(Step->steps == 100) {
				Step->max_speed = step_reset_speed[Step->motor_type_config?1:0];
				}
				if(zero) {
				int steps = 20;
				
				steps = stepspeed_Q2R(steps);

				Step->steps = steps/*0*/;
				Step->position = steps/*0*/;
				Step->reset = 0;
				// by xhl 2011/04/20			
			
				}
				else if(Step->alarm_step) {
						Step->alarm_step ++;
				}
				break;
			}
			#endif
			
			{
					if (!Get_doard_type_ex())
					{
						if (zero){
							Step->zero_go_++;				//记录零位的宽度
							Step->change_dir_count=0;
							Step->steps =TRIANGLE_SIGN_WIDTH+TRIANGLE_SIGN_WIDTH_GO;//20170519
							if (Step->zero_go_>=TRIANGLE_SIGN_WIDTH)        //在零位上走了20步，可以回头了。
							{
								Step->steps =TRIANGLE_SIGN_WIDTH_GO ;//继续走20步 再回来								
								Step->reset =TEST_SIGN_WIDTH_IN;// TEST_SIGN_WIDTH_LEAVE;

							}
							
						}
						else	{
							
								#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
								if (Step->dir_steps > CX_5N_B_YJ_MAX_STEPS ) //说明走了半天 啥传感器都没有
								{
									Step->steps = 0;
									error_no = 65;/*没找到零位*/
									goto steps_is_0;
								}
								#else														
								if (Step->dir_steps > TRIANGLE_HALF_STEP_MAX_EX ) //说明走了半天 啥传感器都没有
								{
									Step->steps = Step->step_max;
									Step->speed = 0;
									Step->phase = 0;
									Step->dir_steps = 0;
									Step->change_dir_count++;
									Step->dir = !Step->dir ;
									Step->step_wait_time = step_reset_delay_time[stepno];
									need_change_dir =1;
									//arch_StepMotor_Dir(stepno, Step->dir ? Step->dir_High :!Step->dir_High);
								}
								#endif
								
						}

					}
					else
						{
				
					if (zero){
						Step->zero_go_++;				//记录零位的宽度
						Step->change_dir_count=0;
						Step->is_dir =0;
					}
					else	{
							if (Step->zero_go_!=0)  //说明已经测量出了0位的宽度
							{
								Step->steps = 20;//继续走20步 再回来								
								Step->reset = TEST_SIGN_WIDTH_LEAVE;
							}
							else							
							if ((Step->dir_steps > TRIANGLE_HALF_STEP_MAX )) //说明走了半天 啥传感器都没有
							{
								Step->is_dir =1;
								Step->left_right_steps =0;
								Step->steps = Step->step_max;
								Step->speed = 0;
								Step->phase = 0;
								Step->dir_steps = 0;
								Step->change_dir_count++;
								Step->dir = !Step->dir ;
								Step->step_wait_time = step_reset_delay_time[stepno];
								need_change_dir =1;
								//arch_StepMotor_Dir(stepno, Step->dir ? Step->dir_High :!Step->dir_High);
							}
							
					}

						}
			}
			break;
			
		case TEST_SIGN_WIDTH_LEAVE:
				{
					if (Step->steps ==0)
					{
						if (!Get_doard_type_ex())
						{
							Step->steps = TRIANGLE_SIGN_WIDTH_GO+TRIANGLE_SIGN_WIDTH+(Step->zero_go_);
						}else						
						{	
							Step->steps = 20+(Step->zero_go_ >>1);//继续走20步 再回来
						}
						//Step->steps = Step->step_max;
						Step->speed = 0;
						Step->phase = 0;
						Step->dir_steps = 0;
						Step->change_dir_count++;
						Step->dir = !Step->dir ;
						need_change_dir=1;
						Step->step_wait_time = step_reset_delay_time[stepno];
						//arch_StepMotor_Dir(stepno, Step->dir ? Step->dir_High :!Step->dir_High);
						Step->reset = TEST_SIGN_WIDTH_IN;
						
					}
					
				}
				break;
		case TEST_SIGN_WIDTH_LEAVE_EX:
				{
					if (Step->steps ==0)
					{
						Step->steps = 1+(Step->zero_go_ >>1);//继续走20步 再回来
						//Step->steps = Step->step_max;
						Step->speed = 0;
						Step->phase = 0;
						Step->dir_steps = 0;
						Step->change_dir_count++;
						Step->dir = !Step->dir ;
						Step->step_wait_time = step_reset_delay_time[stepno];
						need_change_dir =1;
						//arch_StepMotor_Dir(stepno, Step->dir ? Step->dir_High :!Step->dir_High);
						Step->reset = TEST_SIGN_WIDTH_IN;
						
					}
					
				}
				break;		
		case TEST_SIGN_WIDTH_IN:
			{	
				if (Step->steps ==0)
				{
					if (!Get_doard_type_ex())
					{
						Step->position = 0;
						Step->reset =0;
					
					}
					else
					{
						if (zero)
						{
							Step->position = 0;
							Step->reset =0;
							Step->zero_go_ = 0;
						}
						else
						{
							;//报警
						}
					}						
				}
						
			}
			break;

		case TEST_SIGN_WIDTH_LEAVE_ANDGOTOZERO:
				{
					if (Step->steps ==0)
					{
						if (!Get_doard_type_ex())
						{
							Step->steps = TRIANGLE_SIGN_WIDTH_GO+TRIANGLE_SIGN_WIDTH+(Step->zero_go_);
						}else						
						{	
							Step->steps = 20+(Step->zero_go_ >>1);//继续走20步 再回来
						}
						Step->steps = Step->step_max;
						Step->speed = 0;
						Step->phase = 0;
						Step->dir_steps = 0;
						Step->change_dir_count++;
						Step->dir = !Step->dir ;
						need_change_dir=1;
						Step->step_wait_time = step_reset_delay_time[stepno];
						//arch_StepMotor_Dir(stepno, Step->dir ? Step->dir_High :!Step->dir_High);
						Step->reset = GOTO_ZERO;
						
					}
				}	
			break;

		case MOVETO_ZERO_YJ:
			{
				Step->gosteps_temp++;
				if(zero)
				{			
					if(Step->gosteps_temp>180)
					{
						
						/*调头走*/
						Step->steps =200;
						Step->gosteps_temp =0;
						Step->dir = !Step->dir;
						need_change_dir =1;
						Step->change_dir_count++;
					}
				}
				else
				{
					Step->reset =MOVETO_ZERO;
					Step->gosteps_temp =0;
					Step->zero_go_ =0;
					Step->dir_steps =0;
					Step->steps =Step->step_max;
				}
			}
			break;				
		case MOVETO_ZERO:
			#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
			{
				if(zero)
				{

				}
				else
				{

				}
				
			}
			break;	
			
			#endif
			
					if (!Get_doard_type_ex())
					{
						if (zero)
						{
							if (Step->Over_Zero_Count==zero)   //说明一直在0位
							{
								if (Step->dir_steps > TRIANGLE_HALF_STEP_MAX_EX ) //说明走了半天 啥传感器都没有
								{
									Step->steps = Step->step_max;
									Step->speed = 0;
									Step->phase = 0;
									Step->dir_steps = 0;
									Step->change_dir_count++;
									Step->dir = !Step->dir ;
									need_change_dir=1;
									Step->step_wait_time = step_reset_delay_time[stepno];
									//arch_StepMotor_Dir(stepno, Step->dir ? Step->dir_High :!Step->dir_High);

								}
								else
								{
									if ((Step->steps==0)&&(Step->motor_type_config)&&(Step->motor_alarm_cnt))  /*走完了还在零位，那就强制找一下零位*/
									{
										Step->steps = Step->step_max;
										Step->motor_alarm_cnt=0;
									}
								}
								
							}
							else								//说明本来不在0位现在到0位了。
							{							
								Step->zero_go_++;				//记录零位的宽度
								Step->change_dir_count=0;	
								Step->steps +=TRIANGLE_SIGN_WIDTH;   /*确保能走到*/

								if (Step->zero_go_>=TRIANGLE_SIGN_WIDTH)
								{
									Step->steps =TRIANGLE_SIGN_WIDTH_GO ;//继续走20步 再回来				

									//#ifdef LX_
									Step->reset = TEST_SIGN_WIDTH_IN;
									//#else									
									//Step->reset = TEST_SIGN_WIDTH_LEAVE;
									//#endif
								}
								
							}
						}
						else	
						{	
							if (Step->Over_Zero_Count==zero)							// 说明一直不在0位
							{
								if (Step->dir_steps > TRIANGLE_HALF_STEP_MAX_EX ) //说明走了半天 啥传感器都没有
								{
									Step->steps = Step->step_max;
									Step->speed = 0;
									Step->phase = 0;
									Step->dir_steps = 0;
									Step->change_dir_count++;
									Step->dir = !Step->dir ;
									need_change_dir=1;
									Step->step_wait_time = step_reset_delay_time[stepno];
								//	arch_StepMotor_Dir(stepno, Step->dir ? Step->dir_High :!Step->dir_High);

								}
								else if (Step->steps==0)
								{
									/*走完了，停下来了，还没到0位*/
									Step->steps = Step->step_max;
									Step->zero_go_=0 ;
									//Step->dir_steps = 0;
									Step->reset = GOTO_ZERO;
								}
							}
							else												// 本来在0位，一下就不在0位了
							{
								Step->zero_go_++;				//记录零位的宽度
								Step->change_dir_count=0;				

								//if(Step->steps)

								if (Step->zero_go_>=0)
								{
									Step->steps =TRIANGLE_SIGN_WIDTH_GO ;//继续走10步 再回来				
									Step->reset = TEST_SIGN_WIDTH_LEAVE_ANDGOTOZERO;
								}
							}

													
							
						}
						
					
					}
					else
						{
#ifdef LX_
if ((!Is_Yarn_Board()) && (Get_doard_type_ex()) &&(Triangle_mode))
			{
				if(zero) {
				int steps;

				//steps=20;
				steps = 20;
				steps = stepspeed_Q2R(steps);

				Step->steps = steps/*0*/;
				Step->position = steps/*0*/;
				Step->reset = 0;
				}
			else {
				if(Step->steps == 0/*ACC_STEP*/) {
					int steps = 200;
					steps = stepspeed_Q2R(steps);

					Step->steps = steps;//200;//Step->step_max;
					Step->position = steps;//200;//Step->step_max;

					//if(step_alert_detect)
					Step->alarm_step = 1/*step_alert_detect*//*ZERO_DETECT*//*5*/;
					Step->max_speed = step_reset_speed[Step->motor_type_config?1:0];
					Step->reset = GOTO_ZERO;
					if(Step->phase == 2) {
						Step->phase = 0;
					}
				}
			}
			break;
}
#endif
						
			
					if (zero){
						Step->zero_go_++;				//记录零位的宽度
						Step->change_dir_count=0;
					}
					else	{
							if (Step->zero_go_!=0)  //说明已经测量出了0位的宽度
							{
								Step->steps = 20;//继续走20步 再回来								
								Step->reset = TEST_SIGN_WIDTH_LEAVE;
							}
							else							
							if (Step->dir_steps > TRIANGLE_HALF_STEP_MAX ) //说明走了半天 啥传感器都没有
							{
								Step->steps = Step->step_max;
								Step->speed = 0;
								Step->phase = 0;
								Step->dir_steps = 0;
								Step->change_dir_count++;
								Step->dir = !Step->dir ;
								need_change_dir=1;
								Step->step_wait_time = step_reset_delay_time[stepno];
								//arch_StepMotor_Dir(stepno, Step->dir ? Step->dir_High :!Step->dir_High);

							}
					}	

						}
			break;
		case LEAVE_ZERO_YJ:
			{
				Step->gosteps_temp++;
				if(zero)
				{			
					if(Step->gosteps_temp>180)
					{
						
						/*调头走*/
						Step->steps =200;
						Step->gosteps_temp =0;
						Step->dir = !Step->dir;
						need_change_dir =1;
						Step->change_dir_count++;
					}
				}
				else
				{
					Step->change_dir_count=0;
					Step->reset =Step->dir ?LEAVE_ZERO:GOTO_ZERO;
					Step->gosteps_temp =0;
					Step->zero_go_ =0;
					Step->dir_steps =0;
					Step->steps =Step->step_max;
				}
			}
			break;
		case LEAVE_ZERO:			
			if(!zero) {

				if(!Step->change_dir_count)
				{
					#ifdef LX_ACTION_STEP_SPECIAL
					int steps = STEPS_LEAVEZERO_FOR_LX;
					#else
					int steps = 20/*50*/;
					#endif
					steps = stepspeed_Q2R(steps);

					Step->steps = steps;//50/*100*/;
					
					//Step->position = 0;
					
					Step->reset = LEAVE_STEP;
				}
				else
				{
					Step->steps = Step->step_max;//50/*100*/;
					Step->reset = GOTO_ZERO;
				}
			}
			#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
			else
			{
				Step->gosteps_temp++;
				if(Step->gosteps_temp>180)
				{
					
					/*调头走*/
					Step->steps =200;
					Step->gosteps_temp =0;
					Step->dir = !Step->dir;
					need_change_dir =1;
					Step->change_dir_count++;					
				}				
			}
			#endif
			
			break;
			
		case LEAVE_STEP:

			#ifdef LX_ACTION_STEP_SPECIAL
				if (zero)  //走了几步就回到零位了
				{
					
					
				}
				

			#else

			#endif
			
			if(Step->steps == 0) {
				int steps = 200;

				steps = stepspeed_Q2R(steps);

				Step->steps = steps;//200;
				Step->max_speed = step_reset_speed[Step->motor_type_config?1:0];
				Step->speed = 0;
				Step->phase = 0;
				Step->dir = 0;
				Step->dir_steps = 0;
				//
				need_change_dir=1;
				Step->step_wait_time = step_reset_delay_time[stepno];
				//arch_StepMotor_Dir(stepno, !Step->dir_High);
				
				Step->reset = GOTO_ZERO;
			}
			break;
		
		default:
			break;
		}
		if(need_change_dir)
		{
			arch_StepMotor_Dir(stepno, Step->dir?Step->dir_High:!Step->dir_High);
		}

steps_is_0:
	
	if(Step->steps > 0) {

		#ifdef USE_HZ_

		Now_HZ_ = (START_HZ_+Step->ACC_hz_*Step->speed);
		speed= (1000*1000/Now_HZ_)-(step_timer_cnt<<1);
		
		#else
		speed = Step_pulse_triangle[Step->speed];

		#endif
		//speed += Step->max_speed;
		//arch_StepMotor_Set_Speed(stepno, speed);

		switch(Step->phase) {
		case 0:	// Accelerate
			//Step->speed += ((0x1 << step_resolution) + 3) >> QUARTER;
			Step->speed ++;
			if(Step->speed >= ACC_STEP) {
				Step->speed = ACC_STEP;
				Step->phase ++;
			}
#if 1
			else if(Step->steps < ACC_STEP) {
				Step->phase ++;
			}
#else
			else if(Step->steps < 8) {
				Step->phase ++;
			}
#endif
			break;
		case 1: // Isokinetic
#if 1
			if(Step->steps < ACC_STEP) {
				Step->phase ++;
			}
#else
			if(Step->steps < 8) {
				Step->phase ++;
			}
#endif
			break;
		case 2: // Decelerate
#if 1
			if(Step->speed > 0)
				Step->speed --;
#else
			if(Step->speed > 2)
				Step->speed -= 3;
			else
				Step->speed = 0;
#endif
			break;
		default:
			break;
		}

		

		#ifdef LX_
		if (((!Is_Yarn_Board()) && (Get_doard_type_ex()) &&(Triangle_mode))||((!Get_doard_type_ex())&&(!Is_Yarn_Board())))
		{
		#ifdef USE_SLOW_SPEED
			
			if (needslow_speed)
			{

				#ifdef USE_HZ_
					if ((Now_HZ_>SLOW_MAX_HZ_)&&(Step->slow_steps_>0))
					{
						speed = 1000*1000/(Now_HZ_-(Now_HZ_ - SLOW_MAX_HZ_)*(Step->slow_steps_-1) /(SLOW_ACC_STEPS_HZ_-1))-(step_timer_cnt<<1);
					}
				#else
				speed =Step_pulse_triangle[0];
				#endif				
			}
			else
			#ifndef USE_HZ_			
			speed += Step->max_speed;
			#else
			;
			#endif
			

		#endif
			
		}
		
		#else
			#ifndef USE_HZ_			
			speed += Step->max_speed;
			#endif
			//speed += Step->max_speed;
			
		#endif

		

		#ifdef CX_
		if ((!Is_Yarn_Board()) && (!Get_doard_type_ex())&&(Step->motor_type_config==0))	
		{
			if (needslow_speed)
			{	
				if (Step->phase_slow ==1) /*减速*/
				{
					if (Step->need_slow_speed<=ACC_STEPS_TRIANGLE_EX)
					speed = Step_pulse_triangle_ex[ACC_STEPS_TRIANGLE_EX -Step->need_slow_speed]+Step->max_speed;
				}
				if (Step->phase_slow ==2) /*加速*/
				{
						if (Step->need_slow_speed<ACC_STEPS_TRIANGLE_EX)
					speed = Step_pulse_triangle_ex[Step->need_slow_speed]+Step->max_speed;				
				}
				
			//	if (speed<=Step->slow_speed)
				//speed = 	Step->slow_speed;
				//speed += Step->max_speed;
			}

		if (Step->phase==2)
		{
			if (speed<Step->last_speed)
			{
				speed = Step->last_speed;
			}
		}		
		Step->last_speed = speed;

			
		}
		

		
		#endif

		#ifdef STEP_USE_HZ_
		if ((!Is_Yarn_Board()) && (!Get_doard_type_ex())&&(Step->motor_type_config==0))	
		 act_step_use_hz_speed_op(Step);

		speed = (Step->step_use_hz_T*1000L) - (step_timer_cnt<<1);
		
		#endif

		
		
		
		// by xhl 2011/04/15
		
		//if(!is_sinker(stepno) && step_run_mode && (stepno & 0x1)) {
			speed = stepspeed_R2Q(speed);
	//	}

		
		arch_StepMotor_Set_Speed(stepno, speed);
	}
	else {
			//yes it's over  走完需要的步数了。下面判断失步
		if ((Step->running == RUNNING_OVER)&&(Step->reset==JUST_RUN)) 
		{
			goto exit;
		}


		Step->running = 0;
		Step->done_steps  = 0;
		Step->change_dir_count =0;
		Step->zero_count = 20;
		//Step->check_sts_enable = 1;
		arch_StepMotor_Stop(stepno);
		arch_StepMotor_Half(stepno, 0);
		/*
		if ((Step->adj_cnt_ex>2)&&(Step->motor_type_config==0))
		{
			alert_push(STEP_ERR_CODE(Get_stepno_with_motorID(stepno)),(8<<12)|Step->adj_cnt_ex);
			alert_push(0x90,(9<<12)|Step->adj_cnt_steps[0]);
			alert_push(0x91,(10<<12)|Step->adj_cnt_steps[1]);
			alert_push(0x92,(11<<12)|Step->adj_cnt_steps[2]);
			alert_push(0x93,(12<<12)|Step->adj_cnt_steps[3]);
		}
		*/
		#ifdef LX_
		{
			extern void Massage_Send_4Word(unsigned short Msg1,unsigned short Msg2,unsigned short Msg3,unsigned short Msg4);
			//Massage_Send_4Word(0x73,Get_stepno_with_motorID(stepno),0xFFFF,0xFFFF);
		}
		#endif
		goto exit;

#if 0
		if(step_alert_detect == 0)         
			Step->alarm_step = 0;             
		// by xhl 2011/04/20
		if((Step->alarm_step & 0xFFF) > step_alert_detect) {
			errno = Step->alarm_step;
			if((Step->alarm_step >> 12) == 0) {
				errno |= 0x5 << 12;
			}
			Step->position = Step->pos_2;
			if((Step->alarm_step & 0xFFF) < Step->pos_2)
			alert_push(STEP_ERR_CODE(stepno), errno);
			//goto done;
			goto exit;
		}


		zero_work_area = step_zero_adj + ZERO_DETECT;


		zero_work_area = stepspeed_Q2R(zero_work_area);

#ifndef NEW_ZERO_DETECT_MODE
		// check zero
		if(Step->position > zero_work_area /*step_zero_adj + ZERO_DETECT*//*5*/) {
			if(zero && (errno == 0)) {
				errno = 1;
			}
		}
		else if(Step->position <= 0) {
			if(!zero && (errno == 0)) {
				errno = 2;
			}
		}
#endif

#ifdef NEW_ZERO_DETECT_MODE
		if ((zero && (Step->position > zero_work_area /*step_zero_adj + ZERO_DETECT*//*5*/)) ||
		    (!zero && (Step->position <= 0))) {
			Step->zero_count = 20/*10*/;
			Step->zero_count += 15;
			errno = 0;
			// by xhl 2012/07/12
			Step->pos_2 = Step->position;
			Step->alarm_step = 0;
		}
#endif


		if(errno) {
			alert_push(STEP_ERR_CODE(stepno), errno);
			Step->position = 0;
			Step->pos_2 = 0;
		}

		goto exit;

	#endif
	}

done:

	arch_StepMotor_Active(stepno,step_speed[stepno] );
	/*
	if(step_speed[stepno] )
	{
		static unsigned short idxxx;
		Massage_Send_4Word(0xffff,step_speed[stepno] ,step_timer_cnt,idxxx++);
	}
	*/
exit:

	// by xhl 2011/04/20
	if(Step->steps == 0) {
		if(Step->reset) {
			if((Step->reset == LEAVE_ZERO) ||
			   (Step->reset == GOTO_ZERO)||
			   (Step->reset ==TEST_SIGN_WIDTH_IN)) {					//说明没有找到0位
				alert_push(STEP_ERR_CODE(Get_stepno_with_motorID(stepno)),(Step->reset<<8)|16);
				Step->position = 0;
				Step->pos_2 = 0;
				Step->motor_alarm_cnt++;
			}
			return ;
		}
		//Massage_Send_4Word(0xFFEE,stepno,Step->pos_2,Step->position);
		#if 1
		if (Step->running == RUNNING_OVER)
		{
			Step->running = 0;
			if(Step->pos_2 != Step->position) {

			unsigned int	speed_local;	
			unsigned int	phase_local;

			speed_local = Step->speed;
			phase_local = Step->phase;
			Step->done_steps  = 0;
			
			StepMotor_Triangle_exec(stepno, Step->pos_2, 0);

			Step->speed = speed_local ;
			Step->phase = phase_local ;
				
			arch_StepMotor_Active(stepno,step_speed[stepno]);
			
			// by xhl 2012/07/12
			Step->step_wait_time = 10;
			}
		}
		#endif
	/*	if(Step->pos_2 != Step->position) {
			StepMotor_exec(stepno, Step->pos_2, 0);
			arch_StepMotor_Active(stepno);
			// by xhl 2012/07/12
			Step->step_wait_time = 10;
		}
		*/
		#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
		if((stepRunOK_return>>stepno) & 0x01)
		{
			stepRunOK_return &=~(0x01L<<stepno);
			Massage_Send_4Word(0x12FD,stepno,zero,Step->position);		
		}
		#endif
	}
	return ;

}


void StepMotor_Triangle_Set_Sign(unsigned int sk_no,short pos)
{
		int stepno =0;//sk_no+TRIANGLE_BASE;
		int pos_l;

		stepno = Get_motor_ID_with_stepno(sk_no&0xf);
		if(!is_triangle(stepno)) 
		return ;

		switch (pos & 0x03){
		case 0x01:
				pos_l = TRIANGLE_LEFT_POSTION;
				break;
		case 0x02:
				pos_l = TRIANGLE_RIGHT_POSTION;
				break;
		case 0x03:
				pos_l = TRIANGLE_ZERO_POSTION;
				break;
		case 0x00:
				pos_l = TRIANGLE_ZERO_POSTION;
				break;
		default:
				return;

		}
		StepMotor_Set_Position(stepno,pos_l);
		return;
}


short StepMotor_Triangle_Get_Sign(unsigned int sk_no)
{
		STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;
		int sign_0 =0;
		int sign_1 =0;
		int stepno =0;//sk_no+TRIANGLE_BASE;
		int pos;

		stepno = Get_motor_ID_with_stepno(sk_no&0xf);
		Step = &STEPMOTOR[stepno];
		if(!is_triangle(stepno)) 
		return 0;
	
			sign_0 = !arch_input_check(Step->zero_sign_index);
if (Get_doard_type_ex())
{
			sign_1 = !arch_input_check(Step->work_sign_index);

			if ((sign_0==sign_1)&& (sign_0==1)){
				pos= 0x03;
			}
			else
			{ 
				if (sign_0!=sign_1) {
					if (sign_0){
						pos = 0x02;
					}
					else
					{
						pos =0x01;
					}
				}
				else{
					pos = 0x00;
				}					
			}
			
			return pos;
}else

			return sign_0;

	
}

void StepMotor_Triangle_Set_Position(unsigned int sk_no,short pos)
{
	unsigned int st_no;  
	st_no=Get_motor_ID_with_stepno(sk_no&0xf);
	
		
	if(!is_triangle(st_no))//SINKER_COUNT) 
		return ;
	StepMotor_Set_Position(st_no,pos);
	return ;
}


short StepMotor_Triangle_Get_Position(unsigned int sk_no)
{
	unsigned int st_no;  
	st_no=Get_motor_ID_with_stepno(sk_no&0xf);
	
		
	if(!is_triangle(st_no))//SINKER_COUNT) 
		return 0;
	return StepMotor_Get_Position( st_no);
}



#if 0

void Exec_Triangle_step(unsigned int Tno,  int Pos,unsigned int alarmenable)
{
	unsigned int stepno;
	unsigned int localno;
	short arg;
	int alarmenable_local;
	int mode= 0;

	if (Tno >=TRIANGLE_STEP_NUM) return;
	localno = Triangle_Step_no_map[Tno];
	stepno = TRIANGLE_BASE +localno ;

	if (stepno >=MAX_STEPMOTOR_SUPPORT ) return;

	alarmenable_local = alarmenable &0x0003;

	
	
	Triangle_sts[localno] =(alarmenable_local & 0x03);
	mode = Triangle_sts[localno]<<2;
	if ((alarmenable & 0x8000) ==0)
	{		
		alarmenable_local = (Pos& 0x03);
		if ((stepno==TRIANGLE_BASE+1) ||(stepno==TRIANGLE_BASE+3))   //前床
		{
				if ((Pos & 0x03) ==0x01){
					alarmenable_local = 0x02;
					}
				if ((Pos & 0x03) ==0x02){			
					alarmenable_local = 0x01;
					}			
		}

		
		Triangle_sts[localno] =(alarmenable_local& 0x03);		
		mode = Triangle_sts[localno]<<2;

		
		switch (Pos&0x03) 
		{
		case 0x01:
				arg = TRIANGLE_LEFT_POSTION;
				break;
		case 0x02:
				arg = TRIANGLE_RIGHT_POSTION;
				break;
		case 0x03:
				arg = TRIANGLE_ZERO_POSTION;
				break;
		case 0x00:
				Triangle_sts[localno] =0x03;		//归零的时候也是需要检测的
				StepMotor_Reset(stepno);
				return;
				//break;
		default:
			break;			
		}
		mode |=0x03;

		//alert_push(STEP_ERR_CODE(stepno)+mode+99, 1);
		StepMotor_Triangle_exec(stepno, arg, mode);
	}
	else
	{
		arg = (Pos);

#ifdef TRIANGLE_ONE_SENSOR
		if (arg<=20)
		{Triangle_sts[localno]  =0x02;}
		else
			if (arg>=20)
			{
			Triangle_sts[localno]  =0x01;	
			}
			else
				Triangle_sts[localno] = 0x00;
#endif		
		
		mode |=0x3;
		StepMotor_Triangle_exec(stepno, arg, mode);
	}
	
	
}

#endif


void Exec_Triangle_step(unsigned int Tno,  int Pos,unsigned int alarmenable)
{
	unsigned int stepno;
	unsigned int localno;
	short arg;
	int alarmenable_local;
	int mode= 0;

	if (Tno >=TRIANGLE_STEP_NUM) return;
	localno = Get_motor_ID_with_stepno(Tno);
	stepno = TRIANGLE_BASE +localno ;

	if (stepno >=MAX_STEPMOTOR_SUPPORT ) return;

	#ifdef LX_ACTION_STEP_SPECIAL
	STEPMOTOR[stepno].check_zero_work = alarmenable & 0x0F;
	#endif
	
	alarmenable_local = alarmenable &0x0003;	
	
	Triangle_sts[localno] =(alarmenable_local & 0x03);
	mode = Triangle_sts[localno]<<2;
	if ((alarmenable & 0x8000) ==0)
	{		
		alarmenable_local = (Pos& 0x03);
#if 0
		if ((stepno==TRIANGLE_BASE+1) ||(stepno==TRIANGLE_BASE+3))   //前床
		{
				if ((Pos & 0x03) ==0x01){
					alarmenable_local = 0x02;
					}
				if ((Pos & 0x03) ==0x02){			
					alarmenable_local = 0x01;
					}			
		}
#endif
		
		Triangle_sts[localno] =(alarmenable_local& 0x03);		
		mode = Triangle_sts[localno]<<2;

		
		switch (Pos&0x03) 
		{
		case 0x01:
				arg = TRIANGLE_LEFT_POSTION;
				break;
		case 0x02:
				arg = TRIANGLE_RIGHT_POSTION;
				break;
		case 0x03:
				arg = TRIANGLE_ZERO_POSTION;
				break;
		case 0x00:
				Triangle_sts[localno] =0x03;		//归零的时候也是需要检测的
				StepMotor_Reset(stepno);
				return;
				//break;
		default:
			break;			
		}
		mode |=0x03;

		//alert_push(STEP_ERR_CODE(stepno)+mode+99, 1);
		StepMotor_Triangle_exec(stepno, arg, mode);
	}
	else
	{
		arg = (Pos);

		if (!Get_doard_type_ex())
		{
			int steps_temp1;
			int steps_temp2;
			#ifdef USE_TO_SK
			steps_temp1 =20;
			steps_temp2 =50;	
			
			#else
			steps_temp1 =20;	
			steps_temp2 =20;	
			
			#endif
			if (arg<=steps_temp1)
			{
				Triangle_sts[localno]  =0x02;
			}
			else
			if (arg>=steps_temp2)
			{
				Triangle_sts[localno]  =0x01;	
			}
			else
				Triangle_sts[localno] = 0x00;
		}
		else
		{
			if (!Triangle_mode)				
			{
			#ifdef QL_
				if (((arg<-50)&&(arg>=-200))||(arg>200))			
				{
					alarmenable_local =0x02;
				}
				else
					if ((arg>50)||(arg<-200))
					{
						alarmenable_local =0x01;
					}
					else
					{
						alarmenable_local =0x03;
					}
			#else
				if (arg<-50)				
				{
					alarmenable_local =0x01;
				}
				else
					if (arg>50)
					{
						alarmenable_local =0x02;
					}
					else
					{
						alarmenable_local =0x03;
					}
			#endif		
			}
			Triangle_sts[localno]  =alarmenable_local;
		}
		
	
		
		mode = Triangle_sts[localno]<<2;
		mode |=0x3;
		//arch_SendMassage(0, &mode, 1);
		StepMotor_Triangle_exec(stepno, arg,mode);
	}
	
	
}


//2014-02-17 三角电机复位函数 by hlc

void StepMotor_Triangle_ResetAll(void)
{
	int i;
	for(i = 0; i < max_triangle_num; i ++) {
		StepMotor_Reset(TRIANGLE_BASE + i);
	}
}

#endif



void StepMotor_Isr(unsigned int stepno)
{
	STEP_TYPE *Step/* = &STEPMOTOR[stepno]*/;
	unsigned int speed;
	unsigned char need_chang_dir=0;
	
	int zero = 0;
	int errno = 0;
	int step_alert_detect;
	//int StepMotor_Feet_Work_Sign(int feet_no);
	int zero_work_area;

	if(stepno >= MAX_STEPMOTOR_SUPPORT) return;

	Step = &STEPMOTOR[stepno];
	Step->steps = 200;
	Step->running = 10;
	if(Step->running == 0 || Step->steps == 0) {          //说明走完了，该停下来了。
		Step->running = 0;
		arch_StepMotor_Stop(stepno);
		goto exit;
	}

	if(Step->step_wait_time) {
		arch_StepMotor_Set_Speed(stepno, PULSE_LOW);
		goto done;
	}

	if(Step->level == 0) {
		Step->level = 1;
		arch_StepMotor_Pulse(stepno, 1);

		arch_StepMotor_Set_Speed(stepno, PULSE_LOW);
		goto done;
	}


	step_alert_detect = step_alert_detect_setup;

	Step->level = 0;

	arch_StepMotor_Pulse(stepno, 0);
	// by xhl 2011/04/20
	if (!Get_doard_type_ex())
		{
		zero = arch_StepMotor_Zero(stepno);
	}else
	{	
		zero = arch_StepMotor_Zero_EX(Step->zero_sign_index,Step->work_sign_index);
		}
#if 0
	if ((zero!=Step->last_zero_sts[2]) &&(zero == Step->last_zero_sts[1])&&(zero == Step->last_zero_sts[0]))
	{
		Step->last_zero_sts[2] = zero;
		if (zero)
		{
			Step->position = step_zero_adj-2;
		}
		else
		{
			Step->position = step_zero_adj+2;
		}

		
	}else   
	{
		if ( zero != Step->last_zero_sts[2] )
		{
			Step->last_zero_sts[2]=Step->last_zero_sts[1];
			Step->last_zero_sts[1]=Step->last_zero_sts[0];
			Step->last_zero_sts[0]=zero;
		}
		else
		{
			if (Step->last_zero_sts[0]!=Step->last_zero_sts[2])
				Step->last_zero_sts[0] = Step->last_zero_sts[2];
			if (Step->last_zero_sts[1]!=Step->last_zero_sts[2])
				Step->last_zero_sts[1] = Step->last_zero_sts[2];

		}
	}


#endif	

	if(Step->dir) {
		Step->position ++;
	}
	else {
		Step->position --;
	}

	Step->steps --;
	switch(Step->reset) 
	{
		case JUST_RUN:
			{
				if(Step->running == RUNNING_OVER) 
				{
					Step->steps = 0;
					//Step->running = 0 ;
					goto exit;
				}
			
				
			}
			break;
		case CHECK_SIGN_WIDTH:
			if(Step->steps <= 10) {
				Step->max_speed = step_reset_speed[Step->motor_type_config?1:0];				
			}			
			if(zero) {
				if (Step->is_reset)
				{									
					if (Step->reset_width_zero_count ==0)	//第一次进入传感器
					{
						int steps = 100;

						steps = stepspeed_Q2R(steps);

						Step->steps = steps/*0*/;
						if (Step->dir)
						{
							Step->position = 0-steps/*0*/;
						}
						else
						{
							Step->position = steps/*0*/;

						}
						Step->alarm_step_leave = steps -stepspeed_Q2R(90);//剩下10步
					}
					if (Step->steps < Step->alarm_step_leave)
					{
						errno = 6;							//走了90步，传感器信号还在
						goto stop_moto_run;
					}					
					Step->reset_width_zero_count ++;	
					
				}
				else
					{
				#if 0
				int steps = step_zero_adj;

				if (Is_Yarn_Board())
					steps = Step->moto_zero_width>>1;
				
				steps = stepspeed_Q2R(steps);

				Step->steps = steps/*0*/;
				if (Step->dir)
					{
					Step->position = 0-steps/*0*/;
					}
				else
					{
						Step->position = steps/*0*/;

				}
				
				Step->reset = JUST_RUN;	
					
				#endif
				}
			
			}
			else
			{
				if (Step->is_reset) {
					if (Step->reset_width_zero_count) {//说明之前进入过传感器,现在出来了，那就是检测好了宽度了

						Step->moto_zero_width_self = Step->reset_width_zero_count;
						//Exec_Send_Sign_width(stepno,Step->moto_zero_width_self);
						Step->reset_width_zero_count=0;
						{											//接着走20步
							int steps = STEP_LEAVE_ZERO_STEPS;					
							steps = stepspeed_Q2R(steps);

							Step->steps = steps/*0*/;
							Step->alarm_step_leave = 0;
							if (Step->dir)
							{
								Step->position = 0-steps/*0*/;
							}
							else
							{
								Step->position = steps/*0*/;

							}
				
						}
						
					}
								
					
					if (Step->steps==0)			//说明之前的20步 走完了
					{ 
							int steps = STEP_LEAVE_ZERO_STEPS+ (Step->moto_zero_width_self - Step->moto_zero_width);					
							steps = stepspeed_Q2R(steps);

							Step->steps = steps/*0*/;
							Step->alarm_step_leave = 0;

							//Exec_Send_Sign_width(stepno,Step->steps);

							Step->dir = !Step->dir;
							need_chang_dir =1;
							Step->step_wait_time = step_reset_delay_time[stepno];
							if (Step->dir)
							{
								Step->position = 0-steps/*0*/;
								//arch_StepMotor_Dir(stepno, Step->dir_High);
							}
							else
							{
								Step->position = steps/*0*/;
								//arch_StepMotor_Dir(stepno, !Step->dir_High);

							}
							Step->reset = CHECK_SIGN_DEVIATION;	
							//arch_StepMotor_Dir(stepno, !Step->dir_High);
						
					}

			
				}

				if (Step->steps < Step->alarm_step_leave)
				{
					errno = 4;						//快走完了，还没找到0为
					goto stop_moto_run;
				}

			}
			break;
		case CHECK_SIGN_DEVIATION:
			if (Step->is_reset) 
			{
				if (zero) 
				{
					//Step->moto_zero_deviation = STEP_LEAVE_ZERO_STEPS+ (Step->moto_zero_width_self - Step->moto_zero_width)-Step->steps - STEP_LEAVE_ZERO_STEPS;
					Step->moto_zero_deviation = (Step->moto_zero_width_self - Step->moto_zero_width)-Step->steps;	//计算出了误差
					Step->reset = JUST_RUN;	
					//Exec_Send_Sign_width(stepno,Step->moto_zero_deviation);
				}
				
			}
			break;			
		case GOTO_ZERO:					
			if(Step->steps <= 10) {
				Step->max_speed = step_reset_speed[Step->motor_type_config?1:0];
				
			}
			#ifdef ZERO_POS_MORC_UPDATE
			if (Step->steps == 0)
			{
				if (Step->motor_type_config)
				{
					Step->steps =100;

					if (Step->dir)
						{
							Step->position = 0-Step->steps/*0*/;
						}
						else
						{
							Step->position = Step->steps/*0*/;

						}
					Step->moto_count_out++;	
					//Step->position =200;
				}else
			
				Step->reset = JUST_RUN;
			}
			
			#endif
			
			if(zero) {

				if (Step->is_reset)
				{
					#ifdef ZERO_POS_MORC_UPDATE

					if (Step->reset_width_zero_count ==0)
					{
						int steps = 4+Step->moto_zero_width;

						steps = stepspeed_Q2R(steps);

						Step->steps = steps/*0*/;
						if (Step->dir)
						{
							Step->position = 0-steps/*0*/;
						}
						else
						{
							Step->position = steps/*0*/;

						}

						if (Step->motor_type_config)
						{
							Step->reset = JUST_RUN;
						}
						//Step->alarm_step_leave = steps -stepspeed_Q2R(4);//剩下10步
					}
					
				/*	if (Step->steps < Step->alarm_step_leave)
					{
						errno = 6;
						goto stop_moto_run;
					}					
				*/	
					Step->reset_width_zero_count ++;

					#else
				
					if (Step->reset_width_zero_count ==0)
					{
						int steps = 100;

						steps = stepspeed_Q2R(steps);

						Step->steps = steps/*0*/;
						if (Step->dir)
						{
							Step->position = 0-steps/*0*/;
						}
						else
						{
							Step->position = steps/*0*/;

						}
						Step->alarm_step_leave = steps -stepspeed_Q2R(90);//剩下10步
					}
					if (Step->steps < Step->alarm_step_leave)
					{
						errno = 6;
						goto stop_moto_run;
					}					
					
					Step->reset_width_zero_count ++;
					#endif

				}
				else
					{
				
				int steps = step_zero_adj;

				if (Is_Yarn_Board())
					steps = Step->moto_zero_width>>1;
				
				steps = stepspeed_Q2R(steps);

				Step->steps = steps/*0*/;
				if (Step->dir)
					{
					Step->position = 0-steps/*0*/;
					}
				else
					{
						Step->position = steps/*0*/;

				}
				
				Step->reset = JUST_RUN;	
					}
			
			}
			else
			{
				if (Step->is_reset) {
					if (Step->reset_width_zero_count) {

						#ifdef ZERO_POS_MORC_UPDATE

							errno = 6;
							goto stop_moto_run;

						#else

						

						Step->moto_zero_width =stepspeed_R2Q(Step->reset_width_zero_count);

						Step->reset_width_zero_count=0;
						{
							int steps = 20;					
							steps = stepspeed_Q2R(steps);

							Step->steps = steps/*0*/;
							Step->alarm_step_leave = 0;
							if (Step->dir)
							{
								Step->position = 0-steps/*0*/;
							}
							else
							{
								Step->position = steps/*0*/;

							}
				
							//Step->reset = JUST_RUN;	
					
						}
						#endif
						
						
					}

					#ifndef ZERO_POS_MORC_UPDATE					
					
					if (Step->steps==0)
					{ 
							int steps = 20+ (Step->moto_zero_width >>1);					
							steps = stepspeed_Q2R(steps);

							Step->steps = steps/*0*/;
							Step->alarm_step_leave = 0;

							Step->dir = !Step->dir;
							need_chang_dir=1;
							Step->step_wait_time = step_reset_delay_time[stepno];
							if (Step->dir)
							{
								Step->position = 0-steps/*0*/;
							//	arch_StepMotor_Dir(stepno, Step->dir_High);
							}
							else
							{
								Step->position = steps/*0*/;
								//arch_StepMotor_Dir(stepno, !Step->dir_High);

							}
							Step->reset = JUST_RUN;	
							//arch_StepMotor_Dir(stepno, !Step->dir_High);
						
					}

					#endif
				}

				if (((Step->steps < Step->alarm_step_leave)&&(!Step->motor_type_config))||((Step->moto_count_out>1)&&(Step->motor_type_config)))
				{
					errno = 4;
					goto stop_moto_run;
				}

			}
			
			break;
		case MOVETO_ZERO:
			#ifdef ZERO_POS_MORC_UPDATE
			{
			
				if(Step->steps <= 10) 
				{
					Step->max_speed = step_reset_speed[Step->motor_type_config?1:0];
				}
			
				if(Step->steps == 0/*ACC_STEP*/) 
				{
					if (!zero)						
					{

						if (Step->dir_steps)   //说明之前来过一次了，走了最大步了，还不能能走到0位那就出问题了。
						{
							errno = 12;
							goto stop_moto_run;
						}
						else
						{
							Step->dir_steps++;
							Step->steps=stepspeed_Q2R(MAX_STEPS_YARN);	
							//Exec_Send_Sign_width(stepno,0x8888);
						}
					
						//errno = 7;
						//goto stop_moto_run;
					}
				}

				

				#ifdef SELF_CHECK

				if(zero) {
					
					int steps;
					int step_alarm_tmp=0;

					Step->max_speed = step_reset_speed[Step->motor_type_config?1:0];

#if 0
					if (Step->dir_steps)
					{
						Exec_Send_Sign_width(stepno,Step->dir_steps);
					}
					else
					{
						Exec_Send_Sign_width(stepno,Step->steps);
					}
#endif					
					
					if (Step->dir==0)
					{
						steps = Step->moto_zero_width;
						steps = stepspeed_Q2R(steps);
						Step->position = steps/*0*/;
					}
					else
					{
						
						steps = Step->moto_zero_width_self -Step->moto_zero_width - Step->moto_zero_deviation;
						if (steps<0)
							{
							steps *= -1;			
							steps = stepspeed_Q2R(steps);
							steps *= -1;
							}
						Step->position = 0-steps/*0*/;
					}
					//steps = step_zero_adj;
					//steps = stepspeed_Q2R(steps);
					if (Step->dir_steps)
					{
						if (Step->dir_steps+steps < Steps_lost_alarm)  //失步没到报警上限
						{
								errno = 0;
						}
						else						//说明多走了
						{
								errno = 7;
								step_alarm_tmp =Step->dir_steps+steps;
							
						}
					}
					else
					{
						int steps_lost_tmp;
						steps_lost_tmp = (int)(Step->steps) -steps;
							
						if (steps_lost_tmp >= Steps_lost_alarm)  // 说明失步了
						{
								errno = 8;
								step_alarm_tmp =steps_lost_tmp;
						}
						else
						{
							if (steps_lost_tmp <= 0-(int)Steps_lost_alarm)   //说明前面走多了
							{
								errno = 9;
								step_alarm_tmp =0-steps_lost_tmp;
							
							}
							else
							{
								errno=0;
							}
						}
					}

					if (errno!=0)
					{
						alert_push(STEP_ERR_CODE(stepno), errno);
						//Exec_Send_Sign_width(stepno,step_alarm_tmp);
					}
					
					if (steps<0)
					{
						steps *= -1; 
						if (steps>800)
						{
							errno = 13;
							goto stop_moto_run;
						}
						Step->steps = 800- steps/*0*/;
					}else
					Step->steps = steps/*0*/;

					//Exec_Send_Sign_width(stepno,Step->steps);
					
					Step->reset = JUST_RUN;
				}
				else
				{
					if (Step->dir_steps)
					{
						Step->dir_steps++;
					}

					if (Step->dir_steps >=Steps_lost_alarm)
					{
						errno =10;
						if (errno!=0)
						{
							alert_push(STEP_ERR_CODE(stepno), errno);
							//Exec_Send_Sign_width(stepno,Step->dir_steps);
						}
					}
				}


				#else
				if (Step->dir==0)
				{
					Step->is_reset = 1;
					Step->reset_width_zero_count = 0;
					Step->reset = GOTO_ZERO;

				}

				#endif
			}			
			#else
			
			if(zero) {
				int steps;

				if(is_sinker(stepno)) {
					steps = sinker_zero_area;
				}
				else {
					steps = step_zero_adj + ZERO_DETECT;
				}

			

				steps = step_zero_adj;
				steps = stepspeed_Q2R(steps);

				Step->steps = steps/*0*/;
				Step->position = steps/*0*/;
				Step->reset = 0;
			}
			else {
				if(Step->steps == 0/*ACC_STEP*/) {
					int steps = 200;

					steps = stepspeed_Q2R(steps);

					Step->steps = steps;//200;//Step->step_max;
					Step->position = steps;//200;//Step->step_max;

					//if(step_alert_detect)
					//Step->alarm_step = 1/*step_alert_detect*//*ZERO_DETECT*//*5*/;
					Step->max_speed = step_reset_speed[Step->motor_type_config?1:0];
					Step->reset = GOTO_ZERO;
					if(Step->phase == 2) {
						Step->phase = 0;
					}
				}
			}
			#endif
			break;
		case LEAVE_ZERO:
			if(!zero) {
				int steps = 20/*50*/;
				
				steps = stepspeed_Q2R(steps);

				Step->steps = steps;//50/*100*/;
				Step->position = 0;
				Step->reset = LEAVE_STEP;
			}
			else
				if (Step->steps <Step->alarm_step_leave)
				{
					errno = 5;							//走了半天，0位信号一直有效
					goto stop_moto_run;
				}
			break;		
		case LEAVE_STEP:
			if(Step->steps == 0) {
				int steps = 200;

				steps = stepspeed_Q2R(steps);

				Step->steps = steps;//200;
				Step->max_speed = step_reset_speed[Step->motor_type_config?1:0];
				Step->speed = 0;
				Step->phase = 0;
				Step->dir = 0;
				need_chang_dir=1;
				Step->alarm_step_leave =steps - stepspeed_Q2R(40);
				//
				Step->step_wait_time = step_reset_delay_time[stepno];
				//arch_StepMotor_Dir(stepno, !Step->dir_High);

				if (Step->motor_type_config)
				{
					Step->reset = GOTO_ZERO;
					Step->moto_count_out =0;
				}
				else
					{

				#ifdef ZERO_POS_MORC_UPDATE
				#ifdef SELF_CHECK
					Step->reset = CHECK_SIGN_WIDTH;
				#else
					Step->reset = GOTO_ZERO;
				#endif
				#else
				Step->reset = GOTO_ZERO;
				#endif
					}
			}
			break;

		default:
			break;
	}
		
	if(need_chang_dir)
	{
		arch_StepMotor_Dir(stepno, Step->dir?Step->dir_High:!Step->dir_High);
	}
		
	if(Step->steps > 0) {
		speed = Step_pulse[Step->speed];
		
		switch(Step->phase) {
		case 0:	// Accelerate
			//Step->speed += ((0x1 << step_resolution) + 3) >> QUARTER;
			Step->speed ++;
			if(Step->speed >= ACC_STEP) {
				Step->speed = ACC_STEP;
				Step->phase ++;
			}
			else 
			if(Step->steps < ACC_STEP) {
				Step->phase ++;
			}
			break;
		case 1: // Isokinetic
			if(Step->steps < ACC_STEP) {
				Step->phase ++;
			}
			break;
		case 2: // Decelerate
			if(Step->speed > 0)
				Step->speed --;
			break;
		default:
			break;
		}

		// by xhl 2011/04/15
		speed += Step->max_speed;

		arch_StepMotor_Set_Speed(stepno, speed);
	}
	else {
				
		zero_work_area = step_zero_adj + ZERO_DETECT;
	
		zero_work_area = stepspeed_Q2R(zero_work_area);


#ifdef NEW_ZERO_DETECT_MODE
		if ((zero && (Step->position > zero_work_area /*step_zero_adj + ZERO_DETECT*//*5*/)) ||
		    (!zero && (Step->position <= 0))) 
		    {
			Step->zero_count = 20/*10*/;
			Step->zero_count += 15;
			errno = 0;
			// by xhl 2012/07/12
			Step->pos_2 = Step->position;
			Step->alarm_step = 0;
		     }
#endif

	

stop_moto_run:
		Step->running = 0;
		arch_StepMotor_Stop(stepno);
		arch_StepMotor_Half(stepno, 0);	


		if ((Steps_Done_report_sts >>stepno) &0x0001)
		{
			//extern void Massage_Send_EMF_REBACK(short arg1,short arg2,short arg3,short arg4);

			Massage_Send_EMF_REBACK(0xfd|(0x0f<<8),stepno,Step->position,0xffff);
			
		}

		if(errno) {
			alert_push(STEP_ERR_CODE(stepno), errno);
			Step->position = 0;
			Step->pos_2 = 0;
		}

		

		goto exit;
	}

done:

	{	
		arch_StepMotor_Active(stepno,step_speed[stepno]);
	}
exit:

	// by xhl 2011/04/20
	if(Step->steps == 0) {		
		if(Step->reset) {
			if(Step->reset == LEAVE_ZERO ||
			   Step->reset == GOTO_ZERO) {
				alert_push(STEP_ERR_CODE(stepno), 3);
				Step->position = 0;
				Step->pos_2 = 0;
			}
			return ;
		}
		
		if (Step->running == RUNNING_OVER)
		{
			Step->running = 0;
			if(Step->pos_2 != Step->position) {
			if (Step->motor_type_config)
			{
				StepMotor_exec(stepno, Step->pos_2, 0);
			}
			else
			StepMotor_exec(stepno, Step->pos_2, Is_Yarn_Board() ? 1 : 0);
				
			arch_StepMotor_Active(stepno,step_speed[stepno]);
			
			// by xhl 2012/07/12
			Step->step_wait_time = 10;
			}
		}
		
		if (Step->is_reset) 
		{
			if (Step->need_2_pos_after_reset !=0)
			{
				StepMotor_exec(stepno, Step->need_2_pos_after_reset, Is_Yarn_Board() ? 1 : 0);
				
				arch_StepMotor_Active(stepno,step_speed[stepno]);
			
				Step->step_wait_time = 10;
			}
			else
			{
				dct_stus |=(0x03<<stepno*2);	
			}
		}
		
	}

	return ;
}

#ifdef ENCODER_SUPPORT
int StepMotor_SSI_step2encoder(int idx, int pos)
{
	long encoder;

	encoder = (long)pos * 128L / 25L;
	if ((idx < 4 && ((idx & 0x1) == 1)) ||
	    (idx >= 4 && ((idx & 0x1) == 0))) {
		//myprintf("1pos[%d] ", encoder);
		encoder = 4096 + encoder_zero[idx] - encoder;
	}
	else {
		//myprintf("2pos[%d] ", encoder);
		encoder = encoder_zero[idx] + encoder;
	}
	encoder &= 0xFFF;
	return encoder;
}

int StepMotor_SSI_ckeck_encoder(int idx, int pos, int encoder)
{
	STEP_TYPE *Step = &STEPMOTOR[idx];
	long iencoder;
	int flag;
	int errno = 0;

	if ((idx < 4 && ((idx & 0x1) == 1)) ||
	    (idx >= 4 && ((idx & 0x1) == 0))) {
		flag = 1;
	}
	else {
		flag = 0;
	}
	iencoder = (long)pos * 128L / 25L;
	if (flag) {
		iencoder = 4096 + encoder_zero[idx] - iencoder;
	}
	else {
		iencoder = encoder_zero[idx] + iencoder;
	}
	iencoder &= 0xFFF;

	if(iencoder < encoder) {
		errno = encoder - iencoder;
	}
	else {
		errno = iencoder - encoder;
	}
	if(errno > (4096 >> 1)) {
		errno = 4096 - errno;
	}
	if(encoder_alert_detect && errno/*iencoder*/ > encoder_alert_detect) {
		if(Step->enc_err_count++ < 2) {
			return 0;
		}
		Step->enc_err_count = 0;
		encoder_alert_state |= (0x1 << idx);
		//alert_push(STEP_ERR_CODE(idx), errno | (0x4 << 12));
		coder_err = 0xe1 + idx;
		alert_push(coder_err, errno | (0x4 << 12));
		//myprintf("coder_err 0x%x\r\n", coder_err);
		//myprintf("[%d] encoder [%d]->[%d] err[%d]\r\n", idx, iencoder, encoder, errno);
		return -1;
	}
	Step->enc_err_count = 0;
	return 0;
}

int SSI_Check(int idx)
{
	int ret;
	int i;
	int coder;
	int sts;

	if(idx != 0xFF) {
		ret = SSI_Read(idx, &coder, &sts);
		if(ret != -100) {
			encoder_status |= 0x1 << idx;
		}
		else {
			encoder_status &= ~(0x1 << idx);
			return 0xFFFF;
		}

		return (sts >> 1);
	}
	for(i = 0; i < 8; i ++) {
		ret = SSI_Read(i, &coder, &sts);
		if(ret == 0) {
			encoder_status |= 0x1 << i;
		}
		else {
			encoder_status &= ~(0x1 << i);
		}
	}

	//myprintf("encoder_status 0x%x\r\n", encoder_status);
	return encoder_status;
}

void StepMotor_Poll(void)
{
	int ret;
	int i;
	int encoder;
	int sts;
	int enable;
	int pos;

	enable = encoder_status & encoder_enable;
	if(enable == 0) return ;

	i = ssi_check_idx;
next:
	i ++;
	i &= 0x7;

	// by xhl 2012/08/16
	if(step_poweron & (0x1 << i)) 
		goto quit;

	if(enable & (0x1 << i)) {
		STEP_TYPE *Step = &STEPMOTOR[i];
		if((Step->running) ||
		   (Step->step_delay)) {
			//i ++;
			//i &= 0x7;
			//if(i != ssi_check_idx) goto next;
			goto quit;
		}
		Step->step_delay = ENCODER_CHECK_INTERVAL;

		ret = SSI_Read(i, &encoder, &sts);
		if((ret < 0) ||
		   (Step->running)) {
			goto quit;
		}
		//myprintf("[%d] encoder [%d]\r\n", i, encoder);
		pos = Step->position;
		if((encoder_init & (0x1 << i)) == 0) {
			if(pos == 0) {
				encoder_zero[i] = encoder;
				encoder_init |= 0x1 << i;

				StepMotor_Encoder_Save();
			}
		}
		else {
			if(encoder_alert_state & (0x1 << i)) {
				goto quit;
			}
			StepMotor_SSI_ckeck_encoder(i, pos, encoder);
		}
	}

	//i ++;
	//i &= 0x7;
quit:
	ssi_check_idx = i;
}

#else
int SSI_Check(int idx)
{
	return 0;
}

void StepMotor_Poll(void)
{
}

#endif


#if 0

void StepMotor_timer()
{
	int i;
	int check_area/* =  step_zero_adj + ZERO_DETECT*/;
	int err = 0;
	STEP_TYPE *Step;
	for (i = 0; i < MAX_STEPMOTOR_SUPPORT/*10*/; i ++) {
		Step = &STEPMOTOR[i];
#if 0		
		// todo:
		if(i == 3 && Step->step_wait_time >= 1)
		{
			printf("\n");
		}
#endif		
		if(Step->step_wait_time) {
			Step->step_wait_time --;
			if(Step->step_wait_time == 0) {
				Step->step_check_interval = 10;
			}
		}
		if(Step->running) {
			if(Step->step_wait_time) {
				continue;
			}
			if(Step->step_check_interval <= 0) {
				continue;
			}
			Step->step_check_interval--;
			if(Step->step_check_interval > 0)
				continue;
			Step->step_check_interval = 2;
			if(Step->step_check_pos == Step->steps && Step->steps != 0) {
				//alert_push(STEP_ERR_CODE(i), (10 << 12) | Step->steps);
				// stop it
				//arch_StepMotor_Stop(i);
				//Step->running = 0;
				// restart it ???
				arch_StepMotor_Start(i);
			}
			else {
				Step->step_check_pos = Step->steps;
				if(Step->steps == 0) {
					arch_StepMotor_Stop(i);
					Step->running = 0;
					//alert_push(STEP_ERR_CODE(i), (11 << 12));
				}
			}
			continue;
		}
#ifdef ENCODER_SUPPORT
		if(Step->step_delay) Step->step_delay--;
#endif

#ifdef TRIANGLE_STEP_SUPPORT
	if (is_triangle(i)){
		continue;
	}

#endif
	/*
		if(feet_enable)
		{
			
		}
	*/
	}

#ifdef FEET_SUPPORT
	//if((feet_enable) && (feet_alarm_enable != 2))
	if(feet_enable)
		check_feet_sts();
#endif

#ifdef TRIANGLE_STEP_SUPPORT
	check_triangle_sts();
#endif

}
#else
void StepMotor_timer()
{
	int i;
	STEP_TYPE *Step;
	int check_area;
	int err = 0;
	
	for (i = 0; i < MAX_STEPMOTOR_SUPPORT/*10*/; i ++) {
		Step = &STEPMOTOR[i];
	
		if(Step->step_wait_time) {
			Step->step_wait_time --;
			if(Step->step_wait_time == 0) {
				Step->step_check_interval = 10;
			}
		}

		
#ifdef NEW_ZERO_DETECT_MODE
		
			{
		if(Step->zero_count > 0) {
			Step->zero_count --;
			if(Step->zero_count > 15) continue;

			if (!Is_Yarn_Board())
			{
				if (Step->zero_count<=0)
				{
					Step->check_sts_enable = 0;
				}
				else
					Step->check_sts_enable = 1;
				continue;
			}
			
			check_area = step_zero_adj + ZERO_DETECT;			

			check_area = stepspeed_Q2R(check_area);
	
			if(Step->position > check_area) 
			{
				int zero = arch_StepMotor_Zero(i);
				if(zero) {
					Step->alarm_step ++;
					err = 11;
				}else {
					Step->alarm_step = 0;
					
				}
			}
			else if(Step->position <= 0) {
				int zero = arch_StepMotor_Zero(i);			
				if(!zero) {
					Step->alarm_step ++;
					err = 22;
				}
				else {
					Step->alarm_step = 0;					
				}
			}
			if(Step->alarm_step > 10/*5*/ && err) {
				Step->zero_count = 0;
				Step->alarm_step = 0;
				alert_push(STEP_ERR_CODE(Get_stepno_with_motorID(i)), err/*0*/);
			}
		}
			}
#endif
}

#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS

check_triangle_sts_CX_YJ();
#else

#ifdef TRIANGLE_STEP_SUPPORT

	
	
#ifdef LX_
if ((!Is_Yarn_Board()) && (Get_doard_type_ex()) &&(Triangle_mode))
{
	check_triangle_sts_ex();
}
	else
#endif
if (!Is_Yarn_Board())
	check_triangle_sts();



#endif
#endif
	
}

#endif



#ifdef FEET_SUPPORT
//volatile int feet_run_max_step = 500;
//变量声明必需放在程序头上
void StepMotor_Set_Feet_Num(int num)
{
	if((num == 2) || ((num == 4)))
	{
		feet_used_num = num;
	}
	return;
}

int StepMotor_Get_Feet_Num()
{
	return feet_used_num;
}
void StepMotor_Feet_Setup(int cmd, unsigned int arg)
{
	int i;
	int feet_count;

	feet_count = StepMotor_Get_Feet_Num();
		
	switch(cmd) {
	case 0:
		feet_run_max_step = arg;
		feet_enable = 0xff;
		break;
	case 2:
		if ((arg & 0xFF) == 0xFE) {
			//feet_enable = 0xff;
			feet_alarm_enable  = 1;

			for (i = 0; i < feet_count; i++)
			{
				int localno;
				int stepno_feet;
				
				localno = Get_motor_ID_with_stepno(i);
				stepno_feet = FEET_BASE +localno ;
				
				if (feet_sts & (0x1 << i)) {
					if (!arch_stepMotor_inout_sts(STEPMOTOR[stepno_feet].work_sign_index))
					{
						StepMotor_Feet_exec(i, 0x4000);
					}
				}
				else {
					if (!arch_stepMotor_inout_sts(STEPMOTOR[stepno_feet].zero_sign_index))
					{
						STEPMOTOR[stepno_feet].position = 20;
						StepMotor_Feet_exec(i, 0);
					}
				}				
			}
		}
		if ((arg & 0xFF) == 0xFD) {
			//feet_enable = 0;
			feet_alarm_enable  = 0;
		}
		if (((arg & 0xFF) >= 0x40) && ((arg & 0xFF) <= 0x4F))
		{
			//feet_enable = 0xff;
			feet_sts = (arg & 0xFF);
			
			for (i = 0; i < feet_count; i++)
			{
				if (feet_sts & (0x1 << i))
				{
					if (!StepMotor_Feet_Work_Sign(FEET_BASE+ i))
						alert_push(STEP_ERR_CODE(FEET_BASE + i), 4);
				}
				else
				{
					if (!arch_StepMotor_Zero(FEET_BASE + i))
					{
						alert_push(STEP_ERR_CODE(FEET_BASE + i), 3);
					}
				}
			}
		}
		break;
	case 3:
		StepMotor_Set_Feet_Num(arg);
		break;		
	default:
		break;
	}
}

void StepMotor_Feet_exec(int feet_no, int arg)
{
	if(arg & 0x8000) {
		feet_sts &= ~(0x1 << feet_no);
		StepMotor_Reset(FEET_BASE + feet_no);
	}
	else if(arg & 0x4000) {
		if(arch_StepMotor_Zero(FEET_BASE + feet_no)) 
		{
			feet_sts |= (0x1 << feet_no);
			StepMotor_exec(FEET_BASE + feet_no, feet_run_max_step, 0x1 | 0x400);
		// adjust ???
		//StepMotor_exec(FEET_BASE + feet_no, 100, 0x1);
		
		}
	}
	else {
		feet_sts &= ~(0x1 << feet_no);
		StepMotor_exec(FEET_BASE + feet_no, 0, 0x1);
	}
}
void check_feet_sts(void)
{
	int i;
	int feet_count;
	static int err_count[4] = {0};

	feet_count = StepMotor_Get_Feet_Num();

	for (i=0; i < feet_count; i++)
	{
		if (STEPMOTOR[i + FEET_BASE].steps == 0)
		{
			if (feet_sts & (0x1 << i))
			{
				if (!StepMotor_Feet_Work_Sign(FEET_BASE + i))
				{
					err_count[i]++;
					if(err_count[i] > DEF_FEET_ERR_CHECK_TIME)
					{
						alert_push(STEP_ERR_CODE(FEET_BASE + i), 2);
					}
				}else
				{
					err_count[i] = 0;
				}
			}
			else
			{
				if (!arch_StepMotor_Zero(FEET_BASE + i))
				{
					err_count[i]++;
					if(err_count[i] > DEF_FEET_ERR_CHECK_TIME)
					{				
						alert_push(STEP_ERR_CODE(FEET_BASE + i), 1);
					}
				}else
				{
					err_count[i] = 0;					
				}
			}
		}
	}
}
//unsigned int arch_Get_DirStatus(void);
//#ifndef PLATFORM_TMS2812

int StepMotor_Feet_Work_Sign(int feet_no)
{
	switch(feet_no) {
	case 8:
		if((arch_Get_DirStatus() & 0x1) == 0) {
			return 1;
		}
		break;
	case 9:
		if((arch_Get_DirStatus() & 0x2) == 0) {
			return 1;
		}
		break;
#ifdef FEET_SUPPORT_4   //这里要确定
#ifdef SUPPORT_2S
	case 10:
		if(arch_get_3dirstatus() == 0) {
			return 1;
		}
		break;
	case 11:
		if(arch_get_4dirstatus() == 0) {
			return 1;
		}
		break;
#endif
#endif
	}
	return 0;
}

short StepMotor_Feet_Get_Position(unsigned int sk_no)
{
	if(sk_no >= StepMotor_Get_Feet_Num())//SINKER_COUNT) 
		return 0;
	return StepMotor_Get_Position(FEET_BASE +  sk_no);
}

#else
void StepMotor_Feet_Setup(int cmd, unsigned int arg){}
void StepMotor_Feet_exec(int feet_no, int arg){}
int StepMotor_Feet_Work_Sign(int feet_no){ return 0;}
#endif

void StepMotor_Sinker_Set_Num(int sinker_num)
{
	if((sinker_num == 2) || (sinker_num == 4))
		max_sinker_num = sinker_num;
}






#if 0

void StepMotor_Sinker_exec(int sk_no, int arg)
{
	if(sk_no >= max_sinker_num) return;

	StepMotor_exec(sk_no + SINKER_BASE, arg, 1);
}

void StepMotor_Sinker_Reset(void)
{
	int i;
	for(i = 0; i < max_sinker_num; i ++) {
		if(vary_step_enable & (0x1 << (i + 8)))
			continue;
		StepMotor_Reset(SINKER_BASE + i);
	}
}




int StepMotor_Sinker_Get_Busyidx(int sk_no)
{
	if(sk_no >= max_sinker_num) return 0;
	return StepMotor_Get_Busyidx(SINKER_BASE + sk_no);
}

int StepMotor_Sinker_Get_Busy()
{
	int i;
	int ret = 0;
	for(i = 0; i < max_sinker_num; i ++) {
		if(vary_step_enable & (0x1 << (i + 8)))
			continue;
		if(StepMotor_Get_Busyidx(SINKER_BASE + i)) {
			ret = 0xFFFF;
			break;
		}
	}
	return ret;
}

short StepMotor_Sinker_Get_PositionEX(unsigned int sk_no)
{
	
	if(sk_no >= max_sinker_num) 
		return 0;
	//return StepMotor_Get_Position(SINKER_BASE + SINKER_COUNT - sk_no - 1);
	return StepMotor_Get_Position(SINKER_BASE+sk_no);
}


short StepMotor_Sinker_Get_Position(unsigned int sk_no)
{
	int no;
	if(sk_no >= max_sinker_num) 
		return 0;

	switch(sk_no)
	{
		case 0:
			no = SINKER_BASE + 1;
			break;
		case 1:
			no = SINKER_BASE + 0;
			break;
		case 2:
		case 3:
			no = SINKER_BASE + sk_no;
			break;
	}

	//return StepMotor_Get_Position(SINKER_BASE + SINKER_COUNT - sk_no - 1);
	return StepMotor_Get_Position(no);
}
void StepMotor_Sinker_Set_Position(unsigned int sk_no, short pos)
{
	int no;
	if(sk_no >= max_sinker_num) 
		return;
	
	switch(sk_no)
	{
		case 0:
			no = SINKER_BASE + 1;
			break;
		case 1:
			no = SINKER_BASE + 0;
			break;
		case 2:
		case 3:
			no = SINKER_BASE + sk_no;
			break;
	}
	//StepMotor_Set_Position(SINKER_BASE + SINKER_COUNT - sk_no - 1, pos);
	StepMotor_Set_Position(no, pos);
}

void StepMotor_Sinker_Set_PositionEX(unsigned int sk_no, short pos)
{
	int no;
	if(sk_no >= max_sinker_num) 
		return;
	
	//StepMotor_Set_Position(SINKER_BASE + SINKER_COUNT - sk_no - 1, pos);
	StepMotor_Set_Position(SINKER_BASE+sk_no, pos);
}

#endif


int is_sinker(int stepno)
{
	#if 0
#if 0//def FEET_SUPPORT_4
#ifdef PLATFORM_TMS2812
	if((stepno >= FEET_BASE) &&
	   (stepno < FEET_BASE + FEET_DEF_NUM)) {
		return 1;
	}
#else
	if((stepno >= SINKER_BASE) &&
	   (stepno < SINKER_BASE + 4)) {
		return 1;
	}
#endif
#else
	if((stepno >= SINKER_BASE) &&
	   (stepno < SINKER_BASE + max_sinker_num)) {
		return 1;
	}
#endif
	#endif
	
	return 0;
}

#if 0
int check_sinker_done(void)
{
	int i;
	int step_count;
	STEP_TYPE *Step;

	for(i = 0; i < max_sinker_num; i++)
	{
		Step = &STEPMOTOR[SINKER_BASE + i];
		if(Step->running)
		{
			if(Step->pos_2 > Step->position)
				step_count = Step->pos_2 - Step->position;
			else
				step_count = Step->position - Step->pos_2;
			if(step_count > 4)
				alert_push(SINKER_DONE_ERR, (i << 12) | step_count);
		}
	}
	return 0;
}


/*
* left_right: 0 left, 0-2-4-6; 1 right, 1-3-5-7
*/
int check_sti_done(int left_right)
{
	int i;
	int sts = 0;
	
	if(left_right)
	{
		for(i = 1; i < 8; i+=2)
			if(StepMotor_Get_Busyidx(i))
				sts |= (0x1 << i);
	}
	else
	{
		for(i = 0; i < 8; i+=2)
			if(StepMotor_Get_Busyidx(i))
				sts |= (0x1 << i);
	}
	if(sts)
		alert_push(STI_DONE_ERR, sts);

	return 0;
}
#endif

#define USE_2000MA	320
#define TRIANGLE_1700MA	(1)  /*1360mv*/

void StepMotor_init_output()
{
	arch_StepMotor_Init();		
	if (!arch_check_isE487())
	{		
		Set_stepmotor_sub(step_resolution); 
	}
	/*电流输出*/	
	if (arch_check_isE487())
	{
		if (Is_Yarn_Board()) {
			DAC2_SetVoltage(640);//647
			DAC_SetVoltage(1600);// 2.0A
		}
		else
		{
			#ifdef LX_
				#ifdef USE_TO_SK			
					#ifdef USE_TO_SK_NORMAL_STEP
						DAC2_SetVoltage(640);//647	
						DAC_SetVoltage(640);//647	
					#else
						DAC2_SetVoltage(1440);//647	
						DAC_SetVoltage(1440);//647	
					#endif
				#else
					DAC2_SetVoltage(1600);//647
					DAC_SetVoltage(1600);//647
				#endif
				
			#else
				#ifdef TRIANGLE_1700MA
					DAC2_SetVoltage(USE_2000MA);//647	//1800ma=1440
					DAC_SetVoltage(USE_2000MA);//647	//1800ma=1440
				#else		
					DAC2_SetVoltage(960);//647
					DAC_SetVoltage(960);//647	
				#endif
			#endif
		}
	
	}
	else
	{
		if (Is_Yarn_Board()) {
			DAC_SetVoltage(1600);// 2.0A
		}
		else
		{
		#ifdef LX_
			#ifdef USE_TO_SK		
				#ifdef USE_TO_SK_NORMAL_STEP
					DAC_SetVoltage(640);//647	
				
				#else
					DAC_SetVoltage(1440);//647	
				#endif
			#else
				DAC_SetVoltage(1600);//647
			#endif
		#else
			#ifdef TRIANGLE_1700MA
				DAC_SetVoltage(USE_2000MA);//647	//1800ma=1440

			#else
		
				DAC_SetVoltage(960);//647	
			#endif
		#endif
		}
	}

	
}

void StepMotor_Set_Motor_Current(unsigned int Cur_mA )
{
	int V_mv;
	V_mv=(Cur_mA *8) / 10;
	DAC_SetVoltage(V_mv);
	if (arch_check_isE487())
	{
		DAC2_SetVoltage(V_mv);
	}	

}

void StepMotor_Set_Motor_Current_EX(unsigned int Cur_mA,unsigned int whichDA )
{
	int V_mv;
	V_mv=(Cur_mA *8) / 10;

	if (whichDA==0)
	{
		DAC_SetVoltage(V_mv);
	}
	if (whichDA==1)
	{
		if (arch_check_isE487())
		{
			DAC2_SetVoltage(V_mv);
		}
	}
}


unsigned char Step_is_CX_5N_board()
{
	#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
	return 1;
	#else
	return 0;
	#endif	
}


