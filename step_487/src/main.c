    /**
  ******************************************************************************
  * @file    GPIO/GPIO_Toggle/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

  /*   Flash 规划如下
*	0x0800 0000 --0x0800 5FFF   :boot
*	0x0800 6000 --0x0800 6FFF   :参数存储区
*     0x0800 7000 --0x0800 FFFF   :APP 
  */




/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f37x.h"
#include "Command.h"
#include "Config.h"
#include "massage.h"
#include "step.h"

static __IO uint32_t TimingDelay;


//#define Test_debug (1)

volatile unsigned short dct_stus=0xFFFF;

volatile unsigned int DC24=0;
volatile unsigned int DC12=0;

extern volatile unsigned int Board_isE487;


volatile unsigned int Main_sysisok =0;

#ifdef TEST_DEBUG_CODE_ENABLE

volatile unsigned int EMF_Operate_firstcmd=0; //通过两个命令来实现
volatile unsigned int EMF_Operate_firstcmd_timer=0; //通过两个命令来实现
volatile unsigned int EMF_Operate_Reback=0;//设置该值为1 表示，动作完了要应答	

#endif

volatile unsigned int POS_Yarn_DO = 200 ;


#ifdef TRIANGLE_STEP_SUPPORT
volatile unsigned int Triangle_mode=0; 	
#endif

extern volatile int can_receive_count;
extern volatile unsigned int step_resolution;

//unsigned short moto_zero_width[8]={66,66,66,66,66,66,66,66};			//0位传感器宽度

extern STEP_TYPE STEPMOTOR[];
void Delay(__IO uint32_t nTime);

CanTxMsg TxMessage = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int arch_input_boardid(void);
int arch_get_zero_status(void);

int x_atoi(char *str);
void arch_StepMotor_Start(unsigned int stepno);
void Massage_Send_EMF_REBACK(short arg1,short arg2,short arg3,short arg4);

  extern void StepMotor_timer();
  extern void StepMotor_Setup_Active(unsigned int active);
  extern void Alert_Poll();
  extern void StepMotor_Set_Motor_Current(unsigned int Cur_mA );
  extern void StepMotor_Set_Motor_Current_EX(unsigned int Cur_mA,unsigned int whichDA );


char Shell_Step(char **argv, int argc)
{
#if 0
	int i;
	if(argc == 2) {
		if(!strcmp((char*)argv[1], "on")) {
			printf("step power on\n");
			arch_step_motro_en(1);
		}
		else if(!strcmp((char*)argv[1], "off")) {
			printf("step power off\n");
			arch_step_motro_en(0);
		}
	}
	else if(argc > 2) {
		int no, pos;
		int speed;
		if(!strcmp((char*)argv[1], "all")) {
			if(!strcmp((char*)argv[2], "r")) {
				//StepMotor_All_Reset();
				for(i = 0; i < 8; i++)
					//StepMotor_Reset(i);
					Step_Stop(i);
			}
			else {
				pos = x_atoi(argv[2]);
				speed = x_atoi(argv[3]);
				for(i = 0; i < 8; i++)
					Step_Start_Step(i, pos, speed);
			}
		}
		else {
			no = x_atoi(argv[1]);
			if(!strcmp((char*)argv[2], "r")) {
				//StepMotor_Reset(no);
				Step_Stop(no);
				//arch_StepMotor_Stop(no);
			}
			else {
				extern __IO uint16_t CCR2_Val;
				pos = x_atoi(argv[2]);
				speed = x_atoi(argv[3]);
				if(speed == 0) {
				Step_Stop(no);
				//arch_StepMotor_Stop(no);
				}
				else {
				//StepMotor_exec(no, pos, 1);
				Step_Start_Step(no, pos, speed);
				//arch_StepMotor_Start(no);
				//arch_StepMotor_Dir(no, pos);
				CCR2_Val = 150;
				}
			}
		}
	}
#endif
	return 0;
}

#ifdef USE_SHELL
int Shell_AddCMD(char *cmd, char *help, char (*func)(char **, int));
void Shell_command_init()
{
	Shell_AddCMD("step", "Step Motor [on | off | {(all | no.) (r | pos)}]", &Shell_Step);
}

#endif




void Exec_DCT_MOtor_JustRun(int stepno,int pos)
{
	if(stepno >= 8) return;
	
	StepMotor_exec_justrun(stepno,pos);    
	
}



int Exec_DCT_isr(int arg1,int arg2)
{
	int tmp = arg1;
	int t_id;
	unsigned short tmp_stus;

	unsigned short last_stus;

	int moto_pos;
	
	 if((tmp>=24)&&(tmp<40))
	    {
	        tmp-=24;
		 if((tmp>>4)==Get_board_id())
		 {
		     tmp&=0x0F;
		     t_id=(tmp>>1);
		    // if(STEPMOTOR[t_id].alarm_flage==0)
		     {
		         tmp_stus=dct_stus;
			  last_stus =dct_stus;
		         if(arg2&0xFFFF)
		         {
		               tmp_stus|=(1<<tmp);
		         }
		         else
		         {
		               tmp_stus&=(~(1<<tmp));
		         }
		         if(tmp_stus!=dct_stus)
		        {
		        	int twoyarnsts;
		           dct_stus=tmp_stus;
			    twoyarnsts=(tmp_stus>>(2*t_id))&0x03;
			    switch(twoyarnsts)
			    {
			        case 0x0:
					{
						int justselfyarn = (last_stus>>(2*t_id))&0x03;
						switch(justselfyarn)
						{
							case 0x01:
									{
										if(t_id>3)
				     						{
				        						moto_pos=POS_Yarn_DO;
				     						}
				     						else
				     						{
				        						moto_pos=800-POS_Yarn_DO;
				     						}
										   dct_stus |=(1<<(tmp+1));	
									}
								break;
							case 0x02:
								{
									 if(t_id>3)
				     					{
				        				 	moto_pos=800-POS_Yarn_DO;
				     					}
				     					else
				     					{
				        					moto_pos=POS_Yarn_DO;
				    					 }
									dct_stus |=(1<<(tmp-1));	
								}
								break;
							default:
								 moto_pos=400;								
								break;
						}
						
				    
			        	}
				     break;
			        case 0x1:

						#ifdef Test_debug
							 moto_pos=POS_Yarn_DO;
						#else
				     if(t_id>3)
				     {
				         moto_pos=800-POS_Yarn_DO;
				     }
				     else
				     {
				        moto_pos=POS_Yarn_DO;
				     }			
					 #endif
				     break;	
			        case 0x2:
						#ifdef Test_debug
							 moto_pos=800-POS_Yarn_DO;
						#else
				      if(t_id>3)
				     {
				        moto_pos=POS_Yarn_DO;
				     }
				     else
				     {
				        moto_pos=800-POS_Yarn_DO;
				     }

					 #endif
				     break;
			        case 0x3:
				     moto_pos=0;
				     break;					 
			         default:
				     break;
			     }
			     if(STEPMOTOR[t_id].never_run ==1)
			     {
			     
			         StepMotor_Reset(t_id);
				  STEPMOTOR[t_id].need_2_pos_after_reset = moto_pos;
			     }
			     else
			     {
			         StepMotor_exec(t_id,moto_pos,Is_Yarn_Board() ? 1 : 0);    
				      					   
			     }
		         }
		     }
		 }
	    }
}

void Exec_GET_DCT_sts_old()
{
	unsigned int tx_data[4]={0,0,0,0};
 						
              	tx_data[0]=dct_stus;
               	arch_SendMassage(1,&tx_data[0],1);	   				

}

void Exec_GET_DCT_sts(int arg2)
{
	unsigned int tx_data[4]={0,0,0,0};
 						if((arg2 & 0x00ff)==(Get_board_id()+0x10))
	    					{
              					tx_data[0]=dct_stus;
               					tx_data[1]=(Get_board_id()+0x10)<<8;
               					tx_data[2]=0;
               					tx_data[3]=0;
               					arch_SendMassage(1,&tx_data[0],2);
	   					 }

}

void Exec_SET_DCT_sts(int arg2,int arg3)
{
extern  int stepspeed_Q2R_EX(int Q);
	if((arg2 & 0x00ff)==(Get_board_id()+0x10))
	{
								unsigned short i;
								unsigned short st_id;
								dct_stus = arg3 & 0xFFFF;
               						//根据dct_stus 设置电机pos值
               						for (i = 0;i<8;i++)
               						{
               							st_id = (dct_stus>>i*2) & 0x03;
									switch(st_id)
									{	
										case 0:
											STEPMOTOR[i].position= stepspeed_Q2R_EX(800>>1);
											STEPMOTOR[i].pos_2 = stepspeed_Q2R_EX(800>>1);											
											break;
										case 1:
											#ifdef Test_debug
							 					STEPMOTOR[i].position= stepspeed_Q2R_EX(POS_Yarn_DO);
												STEPMOTOR[i].pos_2 = stepspeed_Q2R_EX(POS_Yarn_DO);	
											#else
											 if(i>3)
				     							{
				         							STEPMOTOR[i].position= stepspeed_Q2R_EX(800-POS_Yarn_DO);
												STEPMOTOR[i].pos_2 = stepspeed_Q2R_EX(800-POS_Yarn_DO);	
				     							}
				     							else
				     							{
				          							STEPMOTOR[i].position= stepspeed_Q2R_EX(POS_Yarn_DO);
											STEPMOTOR[i].pos_2 = stepspeed_Q2R_EX(POS_Yarn_DO);	
				     							}
												#endif
				     							break;
										case 2:
											#ifdef Test_debug
							 					STEPMOTOR[i].position= stepspeed_Q2R_EX(800-POS_Yarn_DO);
												STEPMOTOR[i].pos_2 = stepspeed_Q2R_EX(800-POS_Yarn_DO);	
											#else
											if(i>3)
				     							{
				         							STEPMOTOR[i].position= stepspeed_Q2R_EX(POS_Yarn_DO);
											STEPMOTOR[i].pos_2 = stepspeed_Q2R_EX(POS_Yarn_DO);		
				     							}
				     							else
				     							{
				          						STEPMOTOR[i].position= stepspeed_Q2R_EX(800-POS_Yarn_DO);
											STEPMOTOR[i].pos_2 = stepspeed_Q2R_EX(800-POS_Yarn_DO);		
				     							}
											#endif
											break;
										case 3:	
											 STEPMOTOR[i].position= 0;
											STEPMOTOR[i].pos_2 = 0;	
											 
											break;
									}
									STEPMOTOR[i].never_run =0;
								}									
	   }
}

void Exec_GET_Moto_Zero_Width(int arg2)
{
	unsigned int tx_data[4]={0,0,0,0};
	if((arg2 & 0x00ff)==(Get_board_id()+0x10))
	{
		 unsigned short whichyarn =((arg2>>8) & 0xFF);
		
		 if (whichyarn>15) return;
               tx_data[0]=STEPMOTOR[whichyarn>>1].moto_zero_width;
               tx_data[1]=((Get_board_id()+0x10)<<8) | whichyarn;

		arch_SendMassage(1,&tx_data[0],2); 					
           //can_fifo_Push(&can_tx_fifo, &tx_data);
	}
}

void Exec_SET_Moto_Zero_Width(int arg2,int arg3)
{
	if((arg2 & 0x00ff)==(Get_board_id()+0x10))
	{
		 unsigned short whichyarn =((arg2>>8) & 0xff);
		 
		if (whichyarn>15) return;
              STEPMOTOR[whichyarn>>1].moto_zero_width = arg3;
               					
	}
}

void Exec_Reset_step_moto_one(int arg2)
{

	
	if((arg2 & 0x00ff)==(Get_board_id()+0x10))
	{
		unsigned short whichyarn =((arg2>>8) & 0xff);
		
		if (whichyarn>15) return;
              StepMotor_Reset(whichyarn>>1);               					
	}

}


void Exec_Reset_step_moto_all(int arg2)
{

	unsigned short i;

	for (i=0;i<8;i++)
	{
		StepMotor_Reset(i);
	}       

}

void Exec_GET_version_EX(int arg2)
{
	unsigned int tx_data[4]={0,0,0,0};
	unsigned int bootver=arch_get_board_bootver();
	
	if((arg2 & 0x00ff)==(Get_board_id()+0x10))
	{
		tx_data[0]=VER  | ((bootver?0x02:0x01)<<8) | (Get_board_id()<<12);
              arch_SendMassage(1,&tx_data[0],1); 
         //can_fifo_Push(&can_tx_fifo, &tx_data);
	 }
}

void Exec_Set_POS_Yarn(int arg2)
{
if ((arg2>0) &&(arg2<400)) {
	POS_Yarn_DO = arg2;
}
}

void Exec_Get_step_sign(int arg2)
{
	unsigned int tx_data[4]={0,0,0,0};

	#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
	if(1)
	#else
	if((arg2 & 0x00ff)==(Get_board_id()+0x10))
	#endif
	{	
		if(Step_is_CX_5N_board())
			{
		tx_data[0] =0x0AFD;
		tx_data[1]=arch_get_step_sign_();
               arch_SendMassage(1,&tx_data[0],2); 
		}else
			{
		tx_data[0]=arch_get_step_sign_();
               arch_SendMassage(1,&tx_data[0],1); 
			}
         //can_fifo_Push(&can_tx_fifo, &tx_data);
	 }

}


void Exec_Send_Sign_width(int stepno,int width)
{
	unsigned int tx_data[4]={0,0,0,0};
	
	
	{
		tx_data[0]=stepno;
		tx_data[1]=width;
              arch_SendMassage(1,&tx_data[0],2); 
         //can_fifo_Push(&can_tx_fifo, &tx_data);
	 }
}

//#endif



void Massage_Send_EMF_REBACK(short arg1,short arg2,short arg3,short arg4)
{
	unsigned int tx_data[4]={0,0,0,0};	
	
	{
		tx_data[0]=arg1;
		tx_data[1]=arg2;
		tx_data[2]=arg3;
		tx_data[3]=arg4;
              arch_SendMassage(1,&tx_data[0],4); 
         //can_fifo_Push(&can_tx_fifo, &tx_data);
	 }
}


#ifdef TEST_DEBUG_CODE_ENABLE
void Exec_Set_Test_mode_enable(char arg1,short arg2,short arg3)
{

	
	switch (arg1)
	{
		case 0xFE:
			{
				if (EMF_Operate_firstcmd_timer)
				{
				
					if (arg3==0x4851)
					{
						EMF_Operate_firstcmd =arg2?1:0;
						
						if (EMF_Operate_firstcmd)
						{
							EMF_Operate_firstcmd_timer = 1000;
						}
						else
						EMF_Operate_firstcmd_timer =0;
						
						Massage_Send_EMF_REBACK(0x6c,0xFE,EMF_Operate_firstcmd_timer,arg2);
					}
				}
				else
				{
					EMF_Operate_firstcmd_timer =0;
					EMF_Operate_firstcmd=0;
				}
				
				break;
			}

		break;
		case 0xFF:
			{
				if (arg3==0x4851)
				{
				
					if (arg2)
					{
						if (EMF_Operate_firstcmd_timer && EMF_Operate_firstcmd)
						{
							EMF_Operate_Reback =1;
							EMF_Operate_firstcmd_timer=0;
						}
					}
					else
						{
							;//EMF_Operate_Reback =0;
						}
					Massage_Send_EMF_REBACK(0x6c,0xFF,EMF_Operate_Reback,arg2);

				}
			}
		break;
		default:

		break;
	}
	return;
	
}


#endif


void Exec_GET_DC_V(char whichdc)
{
		extern int arch_dip_input();
		unsigned int tx_data[4]={0,0,0,0};
		unsigned int dc_v;
		unsigned int dc_v2=0;
		
		switch (whichdc)
		{
			case 1:
					dc_v = DC24 ;	
					dc_v2 =DC24 *31; 
					break;
			case 2:	
					dc_v = DC12;
					dc_v2 =DC12 * 215 /15; 
				       break;
			case 3:
				       dc_v = arch_dip_input();
					break;
			default:
				return;
		}

		tx_data[0]=dc_v & 0xffff;
		tx_data[1]=(dc_v >>16) & 0xffff;
		tx_data[2]=dc_v2 & 0xffff;
		tx_data[3]=(dc_v2 >>16) & 0xffff;
		
		
              arch_SendMassage(1,&tx_data[0],4); 
       
}

void Exec_GET_version()
{
	unsigned int tx_data[4]={0,0,0,0};
	unsigned int bootver=arch_get_board_bootver();

		tx_data[0]=VER  | ((bootver?0x02:0x01)<<8) | (Get_board_id()<<12) ;
              arch_SendMassage(1,&tx_data[0],1); 
       
}

void Exec_Get_Board_Type_ID()
{
	unsigned int tx_data[4]={0,0,0,0};

		if (Is_Yarn_Board())
			{tx_data[0]=YARN_STEP_BOARD_ID;}
		else
			{tx_data[0]=TRIANGLE_BOARD_ID;}
		
              arch_SendMassage(1,&tx_data[0],1); 
}



extern int com_val;
void Command_loop()
{
	MASSAGE_TYPE *cmd;
	unsigned int tmp;
	unsigned int t_id;
	unsigned int i;
	unsigned int tmp_stus;	
	//Uint16 cmd ;//= t_data->data[0]&0x00ff;


	 {
		cmd = Massage_Pop();
		if(cmd == NULL)
			goto comm_exit;
	
		switch(cmd->cmd) {
			case STEP_ON:
				arch_StepMotor_Enable();
				Main_sysisok =1;
				break;
			case STEP_OFF:
				arch_StepMotor_Disable();
				Main_sysisok =0;
				break;

			

			case TRIANGLE_STEP_ISR:
				#ifdef TRIANGLE_STEP_SUPPORT
						if (Is_Yarn_Board()) break;
				
						Exec_Triangle_step(cmd->arg1, cmd->arg2,cmd->arg3 );
				
				#endif
				break;
			case TRIANGLE_STEP_GET:
				#ifdef TRIANGLE_STEP_SUPPORT
				       if (Is_Yarn_Board()) break;
					if (cmd->arg2>0)			//参数2>0 表示获取编码值。否则获取传感器状态。
					{
						Massage_Send(StepMotor_Triangle_Get_Position(cmd->arg1));
					}
					else
					{
						Massage_Send(StepMotor_Triangle_Get_Sign(cmd->arg1));
					}
				#endif
				break;
			case TRIANGLE_STEP_SET:
				#ifdef TRIANGLE_STEP_SUPPORT
				       if (Is_Yarn_Board()) break;
					if (cmd->arg3>0)			//参数3>0 表示设置位置值。否则设置传感器状态。
					{
						(StepMotor_Triangle_Set_Position(cmd->arg1,cmd->arg2));
					}
					else
					{
						(StepMotor_Triangle_Set_Sign(cmd->arg1,cmd->arg2 & 0x3));
					}
				#endif
				break;	
			case TRIANGLE_STEP_MODE_SET:
				#ifdef TRIANGLE_STEP_SUPPORT
				
				       if (Is_Yarn_Board()) break;
					if (!Get_doard_type_ex())  break;
					   Triangle_mode =cmd->arg1 ? 1:0;
					
				#endif
				break;
			#ifdef FEET_SUPPORT	
			case CMD_FEET_SETUP:
				StepMotor_Feet_Setup(cmd->arg1, cmd->arg2);
				break;
			case CMD_FEET_RUN:
				StepMotor_Feet_exec(cmd->arg1, cmd->arg2);
				break; 	
			#endif
	  case DCT_ISR:
	    if (Is_Yarn_Board() ==0) break;
	    tmp=cmd->arg1;
	    Exec_DCT_isr(cmd->arg1,cmd->arg2);
	    break;


		
	  case GET_DCT: 
			{
				//if ( Is_Yarn_Board() ==0) break;
				switch(cmd->arg1)
				{
				case 0:
						{
							Exec_GET_DCT_sts(cmd->arg2);
	    						break;
						}
						
				case 1:
						{
						 	Exec_SET_DCT_sts(cmd->arg2,cmd->arg3);
	    						break;
						}
					
				case 2:
						{ 
							Exec_GET_Moto_Zero_Width(cmd->arg2);
	    						break;
						}
						
				case 3:
						{
							Exec_SET_Moto_Zero_Width(cmd->arg2,cmd->arg3);
	    						break;						

						}
						
				case 4:
						{
							Exec_Reset_step_moto_one(cmd->arg2);
							#ifdef TEST_DEBUG_CODE_ENABLE
							if (EMF_Operate_Reback)
							{	
								Massage_Send_EMF_REBACK(cmd->cmd,cmd->arg1,cmd->arg2,0);
							}
							#endif
	    						break;	

						}
				case 5:
						{
							       					
	   					 	Exec_Reset_step_moto_all(cmd->arg2);
							#ifdef TEST_DEBUG_CODE_ENABLE
							if (EMF_Operate_Reback)
							{	
								Massage_Send_EMF_REBACK(cmd->cmd,cmd->arg1,cmd->arg2,0);
							}
							#endif
	    						break;	

						}
				case 6:
						{	
							Exec_GET_version_EX(cmd->arg2);
							break;
						}
				case 7:{

							StepMotor_Setup_Active(cmd->arg2);
							#ifdef TEST_DEBUG_CODE_ENABLE
							if (EMF_Operate_Reback)
							{	
								Massage_Send_EMF_REBACK(cmd->cmd,cmd->arg1,cmd->arg2,0);
							}
							#endif
					}
					break;
				case 8:
					{
							StepMotor_Setup_Resolution(cmd->arg2);
					}
				
				break;
				case 9:
					{
						      Exec_Set_POS_Yarn(cmd->arg2);
					}
					break;
				case 0x0A:
					{
							Exec_Get_step_sign(cmd->arg2);
					}
					break;

				case 0x0B:
					{
						#ifndef E_CURR_DEF						
							StepMotor_Set_Motor_Current(cmd->arg2);
						#endif
					}
					break;

				case 0x0C:
					{
							StepMotor_Set_Reset_Speed(cmd->arg2,cmd->arg3);
					}
					break;
				case 0x0D:
					{
							StepMotor_Set_Speed(cmd->arg2,cmd->arg3);
					}
					break;
				case 0x0E:
					{
							Set_LostSteps_alarm(cmd->arg2);
					}
					break;

				case 0x0f:
					{
							Set_Step_done_report(cmd->arg2);
					}
					break;

				case 0x10:
					{
							extern void StepMotor_Setup_Active_single(unsigned int step_no,unsigned char isactive);
							StepMotor_Setup_Active_single(cmd->arg2,cmd->arg3);
					}
					break;
				case 0x11:
						{
							int reset_speed = cmd->arg3;
							
							 StepMotor_Reset(cmd->arg2);
						
	    						break;	

						}	
				case 0x12:
					{
							extern void  Exec_DCT_MOtor_JustRun(int stepno,int pos);
							//Exec_DCT_MOtor_JustRun(arg2,arg3);
							#ifdef CONFIG_CX_YJ_FOR_5_NEEDLEBOARDS
							StepMotor_exec_with_check_mask(cmd->arg2,cmd->arg3,0,cmd->arg4);
							#else
							StepMotor_exec(cmd->arg2,cmd->arg3,0);
							#endif
					}
					break;	
				case 0x13:
					{
						Exec_DCT_Motor_Get_pos(cmd->arg2);
					}
					break;
				case 0x14:
					{
						//extern void StepMotor_Set_Position(unsigned int stepno, short pos);	
						StepMotor_Set_Position(cmd->arg2,cmd->arg3);
					}
					break;
				
				case 0x15:
					{
						#ifndef E_CURR_DEF	
						StepMotor_Set_Motor_Current_EX(cmd->arg2,cmd->arg3);
						#endif
					}
					break;	
				
				case 0x16:
					{
						StepMotor_Set_Speed_HZ_ex(cmd->arg2&0xFF,cmd->arg3,(cmd->arg2>>8)&0xff);
					}
					break;

				case 0x17:
					{
						StepMotor_Setup_Direction(cmd->arg2);
						//arch_SendMassage(1,&arg2,2); 		
					}
					break;
				case 0x18:
					{
						StepMotor_set_Remap(cmd->arg2 & 0xff,(cmd->arg2>>8)&0xff);
					}
				break;

				case 0x19:
					{
						StepMotor_set_HZ_ex_start_a(cmd->arg2,cmd->arg3);
					}
					break;
				case 0x1A:
					{
						StepMotor_set_HZ_ex_max(cmd->arg2,cmd->arg3);
					}
					break;	
					
				case 0x1B:    //测速开始
				{
					
					arch_step_motro_en(0x1);
					StepMotor_test_speed(1,2000);
					com_val = cmd->arg2;
					TIM3->ARR = 500000/com_val-1;
					GPIO_WriteBit(GPIOB,GPIO_Pin_1,cmd->arg3);
					GPIO_WriteBit(GPIOB,GPIO_Pin_0,1);
					Massage_Send_EMF_REBACK(cmd->cmd,cmd->arg1,cmd->arg2,cmd->arg3);
				}break;
				default :break;						
					
				}

	  		}
	  		break;
		
	   		
	  case CLR_ERR: 
	    for(i=0;i<4;i++)
	    {
	    #if 0
	       if(step_motor[i].alarm_flag==1)
	       {
	          if(step_motor[i].err_flag&0x0C0)
	          {
		      stepmotor_reset(i);
		   }
		   step_motor[i].err_flag&=0x3F;
		   if(step_motor[i].err_flag>0)
		   {
		      spi_reg_write(i,0x07,0);
		    }
		    step_motor[i].alarm_flag=0;
	       	}
		   #endif
	     } 
	     break;
	  case UPGRADE_REQUEST:
	   // if((arg1)==(Get_board_id()+0x30))
	    {	
	    #if 1
	       Upgrade_Start();
		#endif   
	    }
	    break;
	
	case GET_DCT_OLD:	// yarn emf
	
				Exec_GET_DCT_sts_old();
		
		break;

	case GET_BOARD_TYPE_ID:
		{
			Exec_Get_Board_Type_ID();
		}
			break;
	case GET_VERSION:
		{
			Exec_GET_version();
		}
		break;

	case GET_DC_V:
		{
			Exec_GET_DC_V(cmd->arg1);
		}

	#ifdef TEST_DEBUG_CODE_ENABLE
	case CMD_TEST_MODE_START:
		{
			Exec_Set_Test_mode_enable(cmd->arg1,cmd->arg2,cmd->arg3);
		}
		break;

	case DCT_TEST_JUSTRUN:
			{
				Exec_DCT_MOtor_JustRun(cmd->arg1,cmd->arg2);
//				if (EMF_Operate_Reback)
//				{	
					Massage_Send_EMF_REBACK(cmd->cmd,cmd->arg1,cmd->arg2,0);
//				}
							
			}
		break;

	case DCT_TEST_GET_POS:
			{
				Exec_DCT_Motor_Get_pos(cmd->arg1);
			}
		break;	
	case DCT_TEST_GET_RUNINGSTS:
			{
				Exec_DCT_Motor_Get_runing_sts();
			}
		break;
	case GET_STEP_HZ_T_DEBUG:
		{
			#ifdef STEP_USE_HZ_
			Step_debug_get_hz_t();
			#endif
		}
	break;
	
	#endif
	
	  default:

				break;
		}
		
		Alert_Clear();
		
comm_exit:
	
	}
	 return;
}






int ADCVal = 0;
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void reg_init()
{
	unsigned int i;

	alert_init();
	Massage_Init();
	

}



void Test_Debug()
{
	#ifdef TEST_DEBUG

	Delay(500);
	
	arch_StepMotor_Enable();
	Delay(200);
	tmp = 0x1234;
	arch_SendMassage(0, &tmp, 1);
	Exec_Triangle_step(0,0x80b8,0);

		while(1)
			{;}
	#endif;
	return;

}

void Init_Shell()
{
	#ifdef USE_SHELL
	Shell_Init();
	Shell_command_init();
	#endif
}

void Init_other()
{
	Delay(200);
	EMF_Operate_firstcmd_timer	=2000;// 1ms*2000
	//Massage_Send(arch_check_isE487());
}

void Other_loop()
{
	static int tmp;
	if((tmp ^ arch_get_zero_status()) != 0) 
	{
		//	printf("single input:0x%x => 0x%x, id %d\n\r", tmp, arch_get_zero_status(), arch_input_boardid());
		tmp = arch_get_zero_status();
			//DAC_SetVoltage(ADCVal);
	}
	arch_adc_test();
}

void Shell_loop()
{
	#ifdef USE_SHELL
	Shell_Poll();
	#endif
}

void Alert_loop()
{
	Alert_Poll();
}

int main(void)
{
	
	
/*
1. init_core();
2.init_hardware_first();//主要是获取ID号
3.init_hardware();
4.init_parameter();
5.init_output();
6.init_other();
7.Test_Debug();
8.while()
{
	loop_command();
	loop_Shell();
	loop_alert();
	loop_other();
}

*/
	Init_core();
	Init_hardware_first();
	Init_hardware();
	Init_parameter();
	Init_output();
	Init_Shell();
	Init_other();	

	Test_Debug();
	
	//arch_step_motro_en(1);	
	
	while (1)
	{
		Command_loop(); 		
		Shell_loop();		
		Alert_loop();	
		Other_loop();		
	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

int run_led_ticks = 0;
/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */

void TimingDelay_Decrement(void)
{
	int tmp =500;
	#ifdef USE_SHELL
	shell_timer_1ms();
	#endif
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}

	if (EMF_Operate_firstcmd_timer) EMF_Operate_firstcmd_timer--;

	if (can_receive_count>0) 
		{tmp = 500;}
	else
		{tmp = 1000;}
	
	if(++run_led_ticks > tmp) {
		run_led_ticks = 0;
		arch_run_led_toggle();
	}


	StepMotor_timer();
	
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
