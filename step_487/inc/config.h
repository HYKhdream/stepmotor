#ifndef __CONFIG_H__
	#define __CONFIG_H__



#define CONFIG_POWER_36V

#ifdef CONFIG_POWER_36V

	#define DC24_MAX			(1290)	//(40000/31)  //40V
	#define DC24_MIN				(1032)	//(32000/31)  //32v


#else
	#define DC24_MAX			(1032)	//(32000/31)  //32V
	#define DC24_MIN				(516)	//(16000/31)  //16v

#endif	
	
	#define DC12_MAX			(1046)	//(15000*15/215)  //15V
	#define DC12_MIN				(628)	//(9000*15/215)  //9v


	#define 	TEST_DEBUG_CODE_ENABLE	(1)// 配合测试工作用的代码生效


//#define CX_   /*慈星动作电机，实现变速跑*/



/*定义强隆*/
//#define QL_	

#ifndef QL_

//#define LX_

#endif



#ifdef LX_

#define LX_ACTION_STEP_SPECIAL    /*连兴的动作电机比较特殊*/

#endif

//#define E_CURR_DEF	


 //#define USE_SHELL (1) 

	#define ZERO_POS_MORC_UPDATE (1)  //零位修正

	#define SELF_CHECK (1)
	//自己检测宽度

	#define TRIANGLE_STEP_SUPPORT (1)

	#define MOTOR_COUNT_MAX (8) //物理上最大支持电机数

	#ifdef TRIANGLE_STEP_SUPPORT
				#define PULSE_LOW_TRIANGLE   (0)  //该值移5 位之后起作用

				#define TRIANGLE_GONO_STEP_LR (15)
				#define TRIANGLE_GONO_STEP_ZERO (30)
				#define TRIANGLE_HALF_STEP_MAX  	(400)  
				
				#define TRIANGLE_STEP_4
				#define TRIANGLE_STEP
				#define TRIANGLE_STEP_NUM	(8)			//应用的时候使用数量
				#define TRIANGLE_BASE 	(0)
				#define DEF_TRIANGLE_ERR_CHECK_TIME (5)
				#define TRIANGLE_ONE_SENSOR	(1)			//一个传感器(postion<0的时候传感器一直有效)
			
				#ifdef TRIANGLE_ONE_SENSOR
				#define TRIANGLE_LEFT_POSTION 	(0-200)
				#define TRIANGLE_RIGHT_POSTION 	(460)
				#else
					#define TRIANGLE_LEFT_POSTION 	(0-240)
					#define TRIANGLE_RIGHT_POSTION 	(240)
				#endif
				#define TRIANGLE_ZERO_POSTION 	(0)

				//#define TRIANGLE_ONE_SENSOR	(1)			//一个传感器(postion<0的时候传感器一直有效)
				#ifndef TRIANGLE_ONE_SENSOR
				#define TRIANGLE_TWO_SENSOR    (1)  			//二个传感器
				#endif
				#define TRIANGLE_SIGN_WIDTH	(10)
				
				#define TRIANGLE_SIGN_WIDTH_GO	(10)
				
	#endif
	#define PULSE_LOW	0	// 100us

//#define FEET_SUPPORT


#ifdef FEET_SUPPORT
#define FEET_DEF_NUM 4
#define FEET_BASE	0

#endif
	
	
#if 0
	#ifndef CLR_JQD_DELAY_TIME
	#define CLR_JQD_DELAY_TIME	50	//500
	#endif

	#ifndef CLR_EMF_DELAY_TIME
	#define CLR_EMF_DELAY_TIME	150	//500
	#endif

	#ifndef DCT_TIME
	#define DCT_TIME	15	// 30ms
	#endif
	
	#ifndef XZQ_TIME
	#define XZQ_TIME	10	// 5ms
	#endif
	
	#define PULSE_LOW	32	// 100us
	#define PULSE_START	32	// 100US    
	#ifndef XZQ_BL
	#define XZQ_BL		12000
	#endif
	
	#define ADD_DEC_STEP	15    
	#define STEP_LOW_SPD	156
	#define STEP_MAX_SPD	16//32//40//43
	#define SK_MAX		2000
	#define SK_MAX_SPEED	 60/*32*/
	#ifndef MAX_STEPS
	#define MAX_STEPS	2000L
	#endif
	#ifndef MAX_BLADE
	#define MAX_BLADE	8
	#endif
	#ifndef MAX_JACQUARD
	#define MAX_JACQUARD	8
	#endif
	#ifndef MAX_YARN
	#define MAX_YARN	16
	#endif
	#ifndef MAX_ACTEMF
	#define MAX_ACTEMF	24
	#endif

	#ifndef SINKER_BASE
	#define SINKER_BASE	8
	#endif
/*
 * 原8选针器，每个选针器8刀，改为16选针器，最大刀数可设3、4.
 */
//#define JAQ_WORKMODE_8_16

#define YARN_STEP_PLUGS	(1)   //电机沙嘴 外挂式


	#ifdef STM32_BOARD
		//#define test_xzq_1_1 (1)
#define JQD_INIT_DELAY_ENABLE 	(1)


#define CIXING

		//#define MAIN_PROGRAM_9000_13_11   (1)   //主控程序是9000的13年11月份的

		#define MAIN_KNIT_3G		(1)			//主机为慈星3针机

		//#define SINKER_MAXSPEED_ISFAST (1)       //定义这个，用于是否提高生克的运行速度

		//#ifdef SINKER_MAXSPEED_ISFAST
			//#ifdef SK_MAX_SPEED 
				//#undef SK_MAX_SPEED
		//	#endif
		//	#define SK_MAX_SPEED 1
		//	#endif
		

		#ifndef SINKER_COUNT
		#define SINKER_COUNT	4
		#endif

		#ifndef	 CIXING
		#define FEET_SUPPORT_4     //有两处定义
		#endif

		#ifdef FEET_SUPPORT_4
		//#define MAX_STEPMOTOR_SUPPORT	12
		#define FEET_SUPPORT
		#define FEET_BASE		8
		#define FEET_DEF_NUM	4
		#define DEF_FEET_ERR_CHECK_TIME	2
		#endif


		#ifndef ALERT_STM32
		#define ALERT_STM32	1
		#endif

		//#define ZERO2_SUPPORT

	#else	//not stm32

		#ifndef SINKER_COUNT
		#define SINKER_COUNT	2
		#endif
	#endif


//#define PLATFORM_TMS2812_OLD 1

	#ifdef PLATFORM_TMS2812

//2014-02-17 支持三角电机4个by hlc.
		#define TRIANGLE_STEP_SUPPORT 	(1)     
//2014-03-05  支持度目电机的第二个传感器检测
		#define STEP_WORK_SIGN_SUPPORT (1)  


		#ifndef SINKER_BASE
			#define SINKER_BASE		8
		#endif



		#ifdef PLATFORM_TMS2812_OLD
			#define CHANGE_SINKER2TRIANGLE 	(1) //12个电机，8个度目，4个三角
		
			#ifndef SINKER_COUNT
				#define SINKER_COUNT	4
			#endif
			#define MAX_STEPMOTOR_SUPPORT	 12
		#else

			#ifndef SINKER_COUNT
				#define SINKER_COUNT	2
			#endif

			#if 0
				#define FEET_SUPPORT_4
				#define FEET_SUPPORT
				#define FEET_DEF_NUM	4

				#define FEET_BASE		(SINKER_BASE + SINKER_COUNT)
			#endif



			#define MAX_STEPMOTOR_SUPPORT	 14//(FEET_BASE + FEET_DEF_NUM)
		#endif
		//2014-02-17 支持三角电机4个by hlc.
		#ifdef TRIANGLE_STEP_SUPPORT
				#define PULSE_LOW_TRIANGLE   (31)  //该值移5 位之后起作用

				#define TRIANGLE_GONO_STEP_LR (15)
				#define TRIANGLE_GONO_STEP_ZERO (30)
				#define TRIANGLE_HALF_STEP_MAX  	(400)  
				
				#define TRIANGLE_STEP_4
				#define TRIANGLE_STEP
				#define TRIANGLE_STEP_NUM	(4)
				#define TRIANGLE_BASE 	(SINKER_BASE + SINKER_COUNT)
				#define DEF_TRIANGLE_ERR_CHECK_TIME (2)
				#define TRIANGLE_LEFT_POSTION 	(0-240)
				#define TRIANGLE_RIGHT_POSTION 	(240)
				#define TRIANGLE_ZERO_POSTION 	(0)
				
				
		#endif

		#ifdef CHANGE_SINKER2TRIANGLE
				#define TRIANGLE_BASE 	(SINKER_BASE )		//三角用的就是生克电机
		#endif



		#define DEF_FEET_ERR_CHECK_TIME	(2)

	#else
		#define MAX_STEPMOTOR_SUPPORT	(SINKER_BASE + SINKER_COUNT)
	#endif //PLATFORM_TMS2812
	//#define ENCODER_SUPPORT 1
	//#define LOGOUT_ENABLE 1
#endif	
#endif

