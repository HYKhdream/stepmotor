#ifndef __STEP_H__
#define __STEP_H__

#include "config.h"

#define STEP_ISR_8_IN_1


/*定义五针板压脚电机*/
#define CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS  


//#define STEP_USE_HZ_

#ifdef STEP_USE_HZ_

#define STEP_USE_HZ_START	(1000)
#define STEP_USE_HZ_MAX_RESET	(2000)
#define STEP_USE_HZ_MAX	(4500)
#define STEP_USE_HZ_A		(200)

//#define STEP_USE_HZ_START	(4500)
//#define STEP_USE_HZ_START	(4500)




#endif




#define HIGH_SPEED_SUPPORT

#define STEPSPEEHZ_3K (1)
	

#define TRIANGLE_HALF_STEP_MAX_EX  	(800)  

#define CX_5N_B_YJ_MAX_STEPS	600


#ifdef LX_	

#define USE_TO_SK

#define USE_TO_SK_NORMAL_STEP 


#ifdef STEP_NOT_ZERO_TRY_IT
	#undef STEP_NOT_ZERO_TRY_IT

#endif

#define ZERO2_SUPPORT 

#define ZERO2_DELAY_COUNT	(2)


#define USE_HZ_

//#define USE_SLOW_SPEED	

#ifdef USE_HZ_
	/*启动频率*/
#define START_HZ_					(1500)	
#ifdef  USE_TO_SK
#ifdef USE_TO_SK_NORMAL_STEP
#define RUN_MAX_HZ_				(3300)
#else
#define RUN_MAX_HZ_				(5500)
#endif

#else
#define RUN_MAX_HZ_				(5000)

#endif
#define RESET_MAX_HZ_				(2000)
#define ACC_STEPS_HZ_				(17)
#define ACC_RUN_HZ_EVERY_STEP		((RUN_MAX_HZ_-START_HZ_)/(ACC_STEPS_HZ_-1))
#define ACC_RESET_HZ_EVERY_STEP	((RESET_MAX_HZ_-START_HZ_)/(ACC_STEPS_HZ_-1))

#define SLOW_MAX_HZ_				(4000)
#define SLOW_ACC_STEPS_HZ_		(17)



#endif






//#define STEPSPEEHZ_5K (1)
#ifdef STEPSPEEHZ_5K
	#define STEP_LOW_SPD	400
	#define STEP_MAX_SPD	1 //16//32//40//43
	#define STEP_MAX_SPD_DEF	233 //16//32//40//43

#else
	#define STEP_LOW_SPD	133
	#define STEP_MAX_SPD	50 //16//32//40//43
	#define STEP_MAX_SPD_DEF	233 //16//32//40//43


#endif


#else

/*当电机零位不在的时候，是否先往正方向走走试试看，确保过了的情况下有机会能走回来*/
#define STEP_NOT_ZERO_TRY_IT	


#ifdef STEPSPEEHZ_3K

#ifdef HIGH_SPEED_SUPPORT

	#define STEP_LOW_SPD	566
	#define STEP_MAX_SPD	1 //16//32//40//43
	#define STEP_MAX_SPD_DEF	166  //233//150 //150 //233 //16//32//40//43
	#define STEP_MOVE_ZERO_LOW_SPEED		233  /*3K的速度*/
	#define STEP_ACT_SLOW_SPEED			233 /*3k的速度*/


	
#else

	#define STEP_LOW_SPD	300
	#define STEP_MAX_SPD	1 //16//32//40//43
	#define STEP_MAX_SPD_DEF	133 //16//32//40//43
#endif	

#else  // 4 k

	#define STEP_LOW_SPD	133
	#define STEP_MAX_SPD	1 //16//32//40//43
	#define STEP_MAX_SPD_DEF	50 //16//32//40//43


#endif

#endif

	#define STEP_SLOW_LOW_SPD_YARN	(820)	
	
	#define STEP_SLOW_MAX_SPD_YARN	(153)  
	



	#define STEP_LOW_SPD_YARN (219) //四细分2.5K

	#define STEP_MAX_SPD_YARN	19  // 1--5.5 k //(19)//()19--5K

	#define STEP_TIMER_CCNT_YARN	(90)

#ifdef RUN_MAX_HZ_	
 
	#define STEP_TIMER_CCNT	(55)  
#else
	#ifdef HIGH_SPEED_SUPPORT
	#define STEP_TIMER_CCNT	(50)  
	#else
	#define STEP_TIMER_CCNT	(100)  
	#endif
#endif	

	#define STEP_LEAVE_ZERO_STEPS	(20)    //离开零位之后继续走多少步




typedef struct {
	short		position;
	short		pos_2;
	unsigned int	steps;
	
	unsigned int	steps_ex;
	unsigned int	speed_ex;
	unsigned int	phase_slow;
	unsigned int 	need_slow_speed;
	
	unsigned int	step_max;
	unsigned int	speed;
	unsigned int	speed_div;
	//unsigned int	speed_base;
	unsigned int	max_speed;
	unsigned int	slow_speed;
	unsigned int	last_speed;
	
#ifdef STEP_USE_HZ_
	unsigned int 	step_use_hz_start;			//启动频率
	unsigned int 	step_use_hz_max;			//最高频率
	//unsigned int 	step_use_hz_max_reset;		//复位频率
	unsigned int 	step_use_hz_a;				//加速度
	unsigned int 	step_use_hz_now;			//当前速度
	unsigned int    step_use_hz_ac_cnt;			//加速加了多少步了
	unsigned char	 step_use_hz_phase_st;		//0--加速，1--匀速，2--减速    
	float	step_use_hz_T;							//单位毫秒
	unsigned int step_use_hz_index;
#endif
	
	
	
	unsigned int	dir_High;
	unsigned int	alarm_step;
	unsigned int	zero_count;
	unsigned int	zero_go_;
	
	unsigned int	dir;
	unsigned int	level;
	unsigned int	running;
	unsigned int	reset;	// 0 => NORMAL; 1 => Go Zero; 2 => Leave Zero
	unsigned int	phase;
	unsigned int	acc_steps;
	unsigned int	step_wait_time;
	unsigned int	step_check_pos;
#ifdef ENCODER_SUPPORT
	unsigned int	step_delay;
	unsigned int	enc_err_count;
#endif
#ifdef STEP_TEST_IN_NDL
	unsigned int	needle;
#endif
	unsigned int	step_check_interval;
	unsigned int	zero;
	unsigned int	zero_cnt;
	unsigned int	zero_cnt_ex;
	unsigned char    zero_st_bit;
	
	
#ifdef ZERO2_SUPPORT
	unsigned int	zero2_mode;
	unsigned int	zero2;	
	unsigned int	zero2_temp[2];	
	 int	zero2_count;
#endif
#if 1//def FEET_SUPPORT
	unsigned int	work_position_count;
#endif

#ifdef USE_HZ_
	unsigned int ACC_hz_;
	unsigned int slow_steps_;
#endif

#if 0
//#ifdef PLATFORM_TMS2812
	unsigned int    timer_enable;
	unsigned int 	timer_count;
	unsigned int 	dst_count;
#endif
#ifdef TRIANGLE_STEP_SUPPORT
	unsigned int done_steps;
	unsigned int change_dir_count;
	unsigned int check_sts_enable;
	unsigned int last_rest;
	unsigned int dir_steps;  
	unsigned int left_right_steps;  
	unsigned int is_dir;
	unsigned int can_reset_dir_step;
	unsigned int zero_sign_index;
	unsigned int work_sign_index;

	
	

	unsigned int Over_Zero_Count;

#endif

/*
#ifdef TRIANGLE_STEP_SUPPORT
	unsigned int done_steps;
	unsigned int change_dir_count;
	unsigned int check_sts_enable;
	unsigned int last_rest;
	unsigned int dir_steps;

*/
	unsigned int alarm_step_leave;     //剩下多少步的时候，如果还没有找到零位，就报警
	unsigned int never_run;	 
	unsigned int is_reset;					// !0 表示正在复位()	
	unsigned int need_2_pos_after_reset;    //复位之后需要走到的位置
	unsigned int last_zero_sts[3];
	unsigned int reset_width_zero_count;      //复位过程中 计算 零位的长度 
	unsigned int moto_zero_width;				//
	unsigned int moto_zero_width_self;			//自己检检测出来自己的宽度
	int moto_zero_deviation;			//电机进出传感器的位置误差 (左转从亮到灭的位置，和右转从灭到亮的位置有偏差)
	int motor_type_config;			//电机类型
	unsigned int  moto_count_out;		//	
	unsigned char is_poweroff_goon;		/*判断是否断电续织起来*/
	unsigned char is_first_lost_alarm;		/*判断是否断电续织起来第一次失步报警*/
	unsigned char motor_alarm_cnt;          /*记录报警次数*/
	unsigned char adj_cnt;
	unsigned char adj_cnt_ex;
	//unsigned char adj_cnt_steps[4];
	short		lastposition;
	unsigned short gosteps_temp;
	
	
	#ifdef LX_ACTION_STEP_SPECIAL
	unsigned int LX_ACT_zero_width;
	unsigned char check_zero_work;
	unsigned char check_zero_work_Timer_enable;
	unsigned char adj_lost_steps;
	unsigned char last_zero_act;
	
	
	#endif

}STEP_TYPE;

/*
#ifdef TRIANGLE_STEP_SUPPORT
#define TRIANGLE_UNKNOW_ZERO	(1)
#define TRIANGLE_ZERO_ZERO		(2)
#define TRIANGLE_NEGATIVE_ZERO	(3)
#define TRIANGLE_PLUS_ZERO		(4)
#define TRIANGLE_ZERO_NEGATIVE	(5)
#define TRIANGLE_ZERO_PLUS		(6)
#define TRIANGLE_NEGATIVE_PLUS 	(7)
#define TRIANGLE_PLUS_NEGATIVE 	(8)


#endif
*/

// STEP reset flag 
#define JUST_RUN	(0)
#define GOTO_ZERO	1
#define LEAVE_ZERO	2
#define LEAVE_ZERO_YJ 200
#define LEAVE_STEP	3
#define MOVETO_ZERO	4
#define MOVETO_ZERO_YJ	400

#define DETECT_ZERO2	5
#define GOTO_ZERO2	 6
#define CHECK_SIGN_WIDTH 	7
#define CHECK_SIGN_DEVIATION	(71)

#ifdef LX_ACTION_STEP_SPECIAL
#define MOVETO_ZERO_LX_ACT (41)
#endif

#define MOVETO_ZERO_LG_STEP	(42)

#ifdef TRIANGLE_STEP_SUPPORT

#define GOTO_LEFT	8		//走到01位置(-300)
#define GOTO_RIGHT	9		//走到10位置(300)
#define CHECK_IS_LEFT 10     //检查01位置是不是真的到了
#define CHECK_IS_RIGHT 11   //检查11位置是不是真的到了
#define CHECK_ISNOT_LR 12     //检查是不是真的离开了两边

#define TEST_SIGN_WIDTH  0x20   			//检测传感器宽度
#define TEST_SIGN_WIDTH_LEAVE  0x21   //检测传感器宽度离开中间
#define TEST_SIGN_WIDTH_IN 0x22   		//回到传感器中间
#define TEST_SIGN_WIDTH_LEAVE_EX 0x23  

#define TEST_SIGN_WIDTH_LEAVE_ANDGOTOZERO 0x24		/*先离开，再回零*/
/*  试着走走看 */
#define GOTO_ZERO_TRY	14		

#endif




#ifdef LX_ACTION_STEP_SPECIAL
#define STEPS_LEAVEZERO_FOR_LX	(100)  
/*连兴特殊的值*/
/*从正到负再走10个点，从负到正少走10个点*/
#define STEP_ZERO_GOON_STEPS	(3)

#endif

// STEP running flag 
#define RUNNING		1
#define RUNNING_OVER	2


void StepMotor_Init(void);
void StepMotor_Set_ZeroAdj(unsigned int adj);
void StepMotor_Set_ZeroDetect(unsigned int area);
void StepMotor_Setup_Active(unsigned int active);
void StepMotor_Setup_Direction(unsigned int dir);
void StepMotor_Setup_Resolution(unsigned int rlt);
void StepMotor_Set_Mode(unsigned int mode);
void StepMotor_Set_FastMode(unsigned int stepno);
void StepMotor_Set_Reset_Speed(unsigned int speed,unsigned int w);
void StepMotor_Set_Speed(unsigned int speed,unsigned int w);
void StepMotor_Set_SinkerSpeed(unsigned int speed);
void StepMotor_Reset(unsigned int stepno);
void StepMotor_Reset_LeftMotor(void);
void StepMotor_Reset_RightMotor(void);
void StepMotor_All_Reset(void);
void StepMotor_Start(unsigned int stepno);
void StepMotor_Set_Position(unsigned int stepno, short pos);
short StepMotor_Get_Position(unsigned int stepno);
unsigned int StepMotor_Get_Busyidx(unsigned int stepno);
unsigned int StepMotor_Get_Busy(void);
void StepMotor_Stop(unsigned int stepno);
void StepMotor_exec(unsigned int stepno, short pos, int mode);
void StepMotor_Set_Check(unsigned int step);
void StepMotor_Feet_Setup(int cmd, unsigned int arg);
void StepMotor_Feet_exec(int feet_no, int arg);
void StepMotor_Sinker_exec(int sk_no, int arg);
void StepMotor_Sinker_Reset(void);
int StepMotor_Sinker_Get_Busyidx(int sk_no);
int StepMotor_Sinker_Get_Busy();
short StepMotor_Sinker_Get_Position(unsigned int sk_no);
void StepMotor_Sinker_Set_Position(unsigned int sk_no, short pos);
void StepMotor_Set_Reset_Delay_Time(int stepno, unsigned int delay);
int StepMotor_Feet_Work_Sign(int feet_no);
void StepMotor_Setup_Para(int cmd, int para);
// 用于测试电机是否到位
void StepMotor_debug(void);
void StepMotor_Sinker_Set_Num(int num);
int check_sinker_done(void);
int check_sti_done(int leftOrRight);
int Exec_DCT_Motor_Get_pos(int stepno);
#ifdef TRIANGLE_STEP_SUPPORT

void StepMotor_Triangle_exec(unsigned int stepno, short pos, int mode);
#endif

#endif

