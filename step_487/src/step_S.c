#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int	cur_pos;
	int	cur_dir;
	int	speed_set;
	int	max_speed;
	int	cur_speed;
	int	min_speed;
	int	max_acc;
	int	cur_acc;
	int	acc_acc;
	int	acc_stil;
	int	max_dcc;
	int	cur_dcc;
	int	dcc_acc;
	int	cur_phase;
	int	time_ticks;
	int	time_ticks_max;
	int	level;
	int	steps;
	int	running;
	int   p_pos;
	int   set_pos;
	int   moto_sts; //0-ide,1-do,2-reset
	int   alarm_flage;
	int dct_cmd_first_flag;
}STEP_MOTOR;
#define TIME_BASE	20000L
#define MAX_MOTORS	8
STEP_MOTOR Steps[MAX_MOTORS];

unsigned short t_cnt[MAX_MOTORS]={0,0,0,0};
unsigned short i_cnt[MAX_MOTORS]={0,0,0,0};
unsigned short zero_cnt[MAX_MOTORS]={0,0,0,0};
unsigned short tmp_cnt[MAX_MOTORS]={0,0,0,0};
unsigned short moto_step_reset[MAX_MOTORS]={0,0,0,0};


void arch_StepMotor_Pulse(int stepno, int onoff);
void arch_StepMotor_Half(int stepno, int dir);
void arch_StepMotor_Dir(int stepno, int dir);

void stepmotor_reset(int m_id)
{
  // volatile struct EPWM_REGS* epwm_regs_ptr;
    if(m_id<MAX_MOTORS)
    {
        if( Steps[m_id].moto_sts != 2 )
       {
            if(arch_StepMotor_Zero(m_id)==0)  //不在0位
            {
	                Steps[m_id].cur_dir=0;
             }
             else
             {	  
	                Steps[m_id].cur_dir=1;
             }

		Steps[m_id].moto_sts =2;
		Steps[m_id].running =1;
		Steps[m_id].cur_phase = 0;
		Steps[m_id].level = 0;
		Steps[m_id].steps = 1200;
	
            arch_StepMotor_Dir(m_id,Steps[m_id].cur_dir);
	     arch_StepMotor_Half(m_id,0);	//半流		
          //  stepmotor_setuppulse(m_id,step_rst_spd);
           // if(step_motor[m_id].half_current_mode==1)
            //{
	      //  step_motor[m_id].half_current_mode=0;
              // spi_reg_write(m_id,0x01,DRV8711_REG1_WORK); 
           // }
	    // step_motor[m_id].index_flag=2;
	    // step_motor[m_id].reindex_flag=0;
	     i_cnt[m_id]=0;
	     zero_cnt[m_id]=0;
	     tmp_cnt[m_id]=0;
	     moto_step_reset[m_id]=0;

		 
	   /*  epwm_regs_ptr=READ_EPWM_REGS_PTR(m_id);
	     epwm_regs_ptr->ETCLR.bit.INT = 1; 
            epwm_regs_ptr->ETSEL.bit.INTEN = 1;               				
            epwm_regs_ptr->AQCSFRC.bit.CSFA=0;	*/
		arch_StepMotor_Stop(m_id);
	   
		arch_StepMotor_Start(m_id);	

	   
        }
    }
}

int Step_Clac_MaxAcc(int stepno, int chang)
{
	STEP_MOTOR *Step = &Steps[stepno];
	int acc;
	int cspeed;
	int stil;

	acc = 0;
	cspeed = 0;
	do {
		acc += Step->acc_acc;
		if(acc > Step->max_acc) {
			acc -= Step->acc_acc;
			break;
		}
		cspeed += acc * 2;
	} while(cspeed < chang);

	stil = (chang - cspeed) / acc;

	if(stil < Step->acc_stil)
		Step->acc_stil = stil;

	if(Step->max_acc > acc)
		Step->max_acc = acc;

	return 0;
}

void Step_Start_Step(int stepno, int step, int speed)
{
	STEP_MOTOR *Step = &Steps[stepno];

	if(Step->running) return;

	Step->cur_pos = 0;
	Step->cur_speed = 0;
	Step->cur_acc = 0;

	Step->min_speed = 3000 * 1;
	Step->acc_acc = 100;
	Step->max_acc = 1000;
	Step->acc_stil = 1000;
	Step->speed_set = 5000;

	if(speed == 0) {
		Step->max_speed = Step->speed_set;
	}
	else if(speed < 0) {
		Step->max_speed = -speed * 1;
	}
	else {
		Step->max_speed = speed * 1;
	}

	if(Step->max_speed < Step->min_speed)
		Step->max_speed = Step->min_speed;
	if(Step->cur_speed < Step->min_speed)
		Step->cur_speed = Step->min_speed;

	Step_Clac_MaxAcc(stepno, Step->max_speed - Step->cur_speed);
	Step->steps = -1;
	if(step) {
		Step_Clac_MaxAcc(stepno, step);
		Step->steps = step;
	}

	Step->time_ticks = 0;
	Step->time_ticks_max = TIME_BASE / Step->min_speed;
	Step->cur_phase = 0;
	Step->level = 0;
	Step->cur_dir = 1;

	arch_StepMotor_Half(stepno, 0);
	Step->running = 1;
	////printf("max_acc = %d, acc_stil = %d\n", Step->max_acc, Step->acc_stil);
}

void Step_Stop(int stepno)
{
	STEP_MOTOR *Step = &Steps[stepno];
	Step->running = 0;
	arch_StepMotor_Half(stepno, 1);
}

int Step_is_running(int stepno)
{
	STEP_MOTOR *Step = &Steps[stepno];
	return (Step->running != 0);
}

void step_timer_isr(int stepno)
{
	STEP_MOTOR *Step = &Steps[stepno];

	if(Step->running == 0)
		goto done;

	/*if(Step->time_ticks > 0) {
		Step->time_ticks --;
		goto done;
	}*/

	if(Step->level == 0) {
		goto speed_set;
	}

	if(Step->cur_dir)
		Step->cur_pos++;
	else
		Step->cur_pos--;

	if(Step->steps > 0) {
		Step->steps--;
		if(Step->steps <= 0) {
			Step->running = 0;
		}
	}

	switch(Step->cur_phase) {
	case 0: // Acc_acc
		Step->cur_acc += Step->acc_acc;
		if(Step->cur_acc >= Step->max_acc) {
			Step->cur_acc = Step->max_acc;
			//Step->cur_acc -= Step->acc_acc;
			if(Step->acc_stil)
				Step->cur_phase = 1;
			else
				Step->cur_phase = 2;
		}
		break;
	case 1: // Acc_mode
		--Step->acc_stil;
		if(Step->acc_stil <= 0) {
			Step->cur_phase++;
		}
		break;
	case 2: // Acc_dec
		Step->cur_acc -= Step->acc_acc;
		if(Step->cur_acc <= 0) {
			Step->cur_acc = 0;
			Step->cur_phase++;
		}
		break;
	case 3: // 
		//if(Step->cur_speed >= Step->max_speed)
		//	Step->running = 0;
		break;
	case 4: // Dec_Acc
		break;
	case 5: // Dec_mode
		break;
	case 6: // Dec_dec
		break;
	}

	if(Step->cur_speed < Step->max_speed) {
		if(Step->cur_acc == 0)
			Step->cur_acc = 1;
		Step->cur_speed += Step->cur_acc;
		if(Step->cur_speed > Step->max_speed) {
			Step->cur_speed = Step->max_speed;
		}
		//printf("acc %d, speed %d\n", Step->cur_acc, Step->cur_speed);
	}

speed_set:
	Step->level = !Step->level;
	arch_StepMotor_Pulse(stepno, Step->level);
	if(Step->cur_speed) {
#if 1
		Step->time_ticks = TIME_BASE / Step->cur_speed - 1;
#endif
#if 0
		if(Step->level)
			Step->time_ticks = 1;
		else {
			Step->time_ticks = TIME_BASE / Step->cur_speed - 1;
			Step->time_ticks = Step->time_ticks * 2 - 1;
		}
#endif
	}
	//printf("speed %d, ticks = %d\n", Step->cur_speed, Step->time_ticks);
	////Step->time_ticks = 0;
	if(Step->running == 0) {
		arch_StepMotor_Half(stepno, 1);
	}
done:
	return ;
}

void step_isr(int stepno)
{
	int i;

	//if( stepno >= MAX_MOTORS) {
	//	return ;
	//}
	StepMotor_Triangle_Isr(stepno);

}

#if 0
int main()
{
	Step_Start(0);
	while(Step_is_running(0))
		step_isr(0);

	return 0;
}
#endif
