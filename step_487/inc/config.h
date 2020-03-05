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


	#define 	TEST_DEBUG_CODE_ENABLE	(1)// ��ϲ��Թ����õĴ�����Ч


//#define CX_   /*���Ƕ��������ʵ�ֱ�����*/



/*����ǿ¡*/
//#define QL_	

#ifndef QL_

//#define LX_

#endif



#ifdef LX_

#define LX_ACTION_STEP_SPECIAL    /*���˵Ķ�������Ƚ�����*/

#endif

//#define E_CURR_DEF	


 //#define USE_SHELL (1) 

	#define ZERO_POS_MORC_UPDATE (1)  //��λ����

	#define SELF_CHECK (1)
	//�Լ������

	#define TRIANGLE_STEP_SUPPORT (1)

	#define MOTOR_COUNT_MAX (8) //���������֧�ֵ����

	#ifdef TRIANGLE_STEP_SUPPORT
				#define PULSE_LOW_TRIANGLE   (0)  //��ֵ��5 λ֮��������

				#define TRIANGLE_GONO_STEP_LR (15)
				#define TRIANGLE_GONO_STEP_ZERO (30)
				#define TRIANGLE_HALF_STEP_MAX  	(400)  
				
				#define TRIANGLE_STEP_4
				#define TRIANGLE_STEP
				#define TRIANGLE_STEP_NUM	(8)			//Ӧ�õ�ʱ��ʹ������
				#define TRIANGLE_BASE 	(0)
				#define DEF_TRIANGLE_ERR_CHECK_TIME (5)
				#define TRIANGLE_ONE_SENSOR	(1)			//һ��������(postion<0��ʱ�򴫸���һֱ��Ч)
			
				#ifdef TRIANGLE_ONE_SENSOR
				#define TRIANGLE_LEFT_POSTION 	(0-200)
				#define TRIANGLE_RIGHT_POSTION 	(460)
				#else
					#define TRIANGLE_LEFT_POSTION 	(0-240)
					#define TRIANGLE_RIGHT_POSTION 	(240)
				#endif
				#define TRIANGLE_ZERO_POSTION 	(0)

				//#define TRIANGLE_ONE_SENSOR	(1)			//һ��������(postion<0��ʱ�򴫸���һֱ��Ч)
				#ifndef TRIANGLE_ONE_SENSOR
				#define TRIANGLE_TWO_SENSOR    (1)  			//����������
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
 * ԭ8ѡ������ÿ��ѡ����8������Ϊ16ѡ���������������3��4.
 */
//#define JAQ_WORKMODE_8_16

#define YARN_STEP_PLUGS	(1)   //���ɳ�� ���ʽ


	#ifdef STM32_BOARD
		//#define test_xzq_1_1 (1)
#define JQD_INIT_DELAY_ENABLE 	(1)


#define CIXING

		//#define MAIN_PROGRAM_9000_13_11   (1)   //���س�����9000��13��11�·ݵ�

		#define MAIN_KNIT_3G		(1)			//����Ϊ����3���

		//#define SINKER_MAXSPEED_ISFAST (1)       //��������������Ƿ�������˵������ٶ�

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
		#define FEET_SUPPORT_4     //����������
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

//2014-02-17 ֧�����ǵ��4��by hlc.
		#define TRIANGLE_STEP_SUPPORT 	(1)     
//2014-03-05  ֧�ֶ�Ŀ����ĵڶ������������
		#define STEP_WORK_SIGN_SUPPORT (1)  


		#ifndef SINKER_BASE
			#define SINKER_BASE		8
		#endif



		#ifdef PLATFORM_TMS2812_OLD
			#define CHANGE_SINKER2TRIANGLE 	(1) //12�������8����Ŀ��4������
		
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
		//2014-02-17 ֧�����ǵ��4��by hlc.
		#ifdef TRIANGLE_STEP_SUPPORT
				#define PULSE_LOW_TRIANGLE   (31)  //��ֵ��5 λ֮��������

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
				#define TRIANGLE_BASE 	(SINKER_BASE )		//�����õľ������˵��
		#endif



		#define DEF_FEET_ERR_CHECK_TIME	(2)

	#else
		#define MAX_STEPMOTOR_SUPPORT	(SINKER_BASE + SINKER_COUNT)
	#endif //PLATFORM_TMS2812
	//#define ENCODER_SUPPORT 1
	//#define LOGOUT_ENABLE 1
#endif	
#endif

