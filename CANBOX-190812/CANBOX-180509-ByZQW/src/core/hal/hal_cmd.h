


/*�����ʽ�ĳ�����:*/
/*
ͨ�������ֽ�����ʾ����(��ǰ��һ���ֽ�)
���е�һ���ֽڱ�ʾ������࣬�ڶ����ֽڱ�ʾС����
��������ֽڱ�ʾ������
���һ���ֽڱ�ʾindex.(ͨѶ���)

CAN ������8���ֽ�
Data0	Data1	data2	data3	Data4	data5	Data6	Data7
CMDtype	cmd		arg1L	arg1H	arg2L	arg2H	arg3L	index
cmdtype ��Ҫ�ּ�����
1:����
2:ϵͳ����
3:��Ŀ���
4:�������(��ˣ�ѹ�ţ������ȵ�)
5:�����(���Ǻ�ɴ��)
6:ѡ����
7:�������
8:��������

*/

#define CMDTYPE_UNDEFINE			(0x00)
#define CMDTYPE_PERIPHERAL			(0x01)
#define CMDTYPE_SYSSET				(0x02)
#define CMDTYPE_MOTOR_DENSITY		(0x03)
#define CMDTYPE_MOTOR_OTHER		(0x04)
#define CMDTYPE_DCT				(0x05)
#define CMDTYPE_XZQ				(0x06)
#define CMDTYPE_ALARM				(0x07)
#define CMDTYPE_OTHERSET			(0x08)
#define CMDTYPE_CHECKONLINE		(0x09)		/*������Ƚ����⣬�������ж��д���*/
#define CMDTYPE_E697				(0X10)



/*CMDTYPE_PERIPHERAL*/


//new cmd
#define CMD_UNDEFINE				(0x00) //�޶���
#define CMD_POWER					(0x01)// ����ӵ� �ϵ�arg1(0- �ϵ�!0 �ӵ�)
#define CMD_STEP_ENABLE			(0x02)// ��������ϵ�ϵ�,arg1(0- �ϵ�!0 �ӵ�)
#define CMD_GET_DIR					(0x03)// ȡ��ͷ���򴫸���״̬
#define CMD_DIR_ISR					(0x04)// ���û�ͷ��������
#define CMD_GET_STEP_INPUT			(0x05)//ȡ��ͷ�������λ״̬arg1l��ʾ�������(0-,1 ��Ŀ,2 ����,3 ���� ,4 ѹ��)arg1h(0- ȡ0λ1-ȡ����λ)
#define CMD_KEYDOG_READ			(0x06)	//		
#define CMD_KEYDOG_CHECK			(0x07)	//
#define CMD_ENCODER_SETUP			(0x08)	//	�����������������,ARG1=0 ���ã�=1����
#define CMD_ENCODER_CHECK			(0x09)	//	 ���ظ���������״̬
#define CMD_STEP_ENCODER_CHECK	(0x0A) /* ����ָ�������������״̬arg1=�����(0-15) */
#define CMD_STEP_WORK_CURRENT	(0x0B)  /*����*/
#define CMD_SET_SYS_ENABLE		(0x0C)  /*���õ�ǰϵͳʹ�ܻ��ǽ���*/
#define CMD_GET_SIG_PH				(0x0D)  /*ȡ����ϵͳ�ĸ�����������״̬����������λ�÷���*/
#define CMD_GET_AD_CUR				(0x0E) /*��ȡ����ֵ*/
#define CMD_GET_SYS_TEMP			(0x0F)  /*ȡ�¶�*/
#define CMD_GET_RES_DATA			(0x10)  /*��ȡ��ǰ���迹ֵ */	
#define CMD_SET_TEST_MODE_PWM		(0x11)  /*���ò���ģʽѡ��PWM��ʽ*/
#define CMD_SET_PWM_DA_OR_DA		(0x12)  /*���õ���ĵ���ֵ*/
#define CMD_POWER_CTRL_EX			(0x13)	/*�ֱ����+-24V*/
#define CMD_SET_STEP_CHECK_TYPE		(0x14)	/*���õ����λ��������(0--Ĭ�ϣ�1--��������2--��������3--������+������)*/



//Old cmd

#define KEYDOG_READ		0x33
#define KEYDOG_CHECK		0x35

#define ENCODER_SETUP		0x45 /*
				      * �����������������
				      * 0: ����
				      * 1: ����
				      */
#define ENCODER_CHECK		0x46 /*
				      * ���ظ���������״̬
				      */
					
#define STEP_ENCODER_CHECK	0x55 /*
				      * ����ָ�������������״̬
				      */


#define POWER_ON		0x00	
#define POWER_OFF		0x01	
#define STEP_ON			0x02	
#define STEP_OFF		0x03	
#define GET_DIR			0x08	
#define DIR_ISR			0x10	
#define GET_ZERO_OTHER 		0x5B	// ȡ��ͷ�����������λ״̬�����˶�Ŀ������
										/*arg1 ��ʾ�������
										*/


/*CMDTYPE_SYSSET*/
//new cmd
#define CMD_SYSSET_MODE_ISR				(0x01)	// ��ϵͳģʽ????
#define CMD_SYSSET_MOTOR_CONFIG_IDTYPE	(0x02)	/* ����������� 1-,����id&type��
							* ����1L: ��8λ��ʾ������NO�ţ�									(0-5)
							* ����1H: ��8λ��ʾtype													(0-4)
							* ����2L: ��8λ��ʾ����ID�ţ�����ڶ�Ŀ����ӿ�				(0-15)
							* ����2H: ��8λ��ʾ����ID�ţ�������������͵ĵ���ӿ�	(0-15)
							*/
#define CMD_SYSSET_MOTOR_CONFIG_INPUTID	(0x03)	/* ����������� 2-����inputid
							* ����1L: ��8λ��ʾ������NO�ţ�	(0-5)
							* ����1H: ��8λbit0��bit1 ���ڱ�ʾ0λ���͹���λ������ID�Ƿ���Ч(1��Ч��0��Ч)
							* ����2L: ��8λ��ʾ0λ������ID		(0-15)
							* ����2H: ��8λ��ʾ����λ������ID	(0-15)
							*/	
#define CMD_SYSSET_HEAD_MODE_SET			(0x04) /*  ��ͷ������������� */
								/*arg1_l=0 ��ʾ���ã�=1 ��ʾ��ȡ*/
								/* arg1_h  ��ֵ
								*0��Ĭ�����á�4��Ŀ+2����+2����+4ɴ�죨����ǵ��ɳ��Ļ�����		--��֧��	
								*1�����׵���4��Ŀ+2����+2�հ�+4ɴ�죨����ǵ��ɳ��Ļ�����			
								*2����ͨ�4��Ŀ+2����+2�հ�+4ɴ�죨����ǵ��ɳ��Ļ�����			
								*3�����˿4��Ŀ+4����+4ɴ�죨����ǵ��ɳ��Ļ�����					--��֧��
								*/
//old cmd

#define SET_MODE_ISR		0x2E	// ��ϵͳģʽ????
#define NEW_STEP_TYPE_SET		0x80 



#define SET_STEPS_TYPE	(0x76)
						/*	��������Զ���
						*  	����1: ������ܺ�(0-�޶���,1-��Ŀ���(���8��),2-��˵��(���4��), 3-�������(���4��),4-ѹ�ŵ��(���4��))
						*	����2:�����ֽڣ�bit0-bit3 ��������0 map����ͷ�ĵ����,
						*							bit4-bit7 ��������1 map����ͷ�ĵ����,
						*							bit8-bit11 ��������2 map����ͷ�ĵ����,
						*							bit12-bit15 ��������3 map����ͷ�ĵ����,
						*							
						*     ����3: �����ֽڣ�bit0-bit3 ��������4 map����ͷ�ĵ����,
						*							bit4-bit7 ��������5 map����ͷ�ĵ����,
						*							bit8-bit11 ��������6 map����ͷ�ĵ����,
						*							bit12-bit15 ��������7 map����ͷ�ĵ���š�
						*	
						*    ע��1: Ŀǰ��ͷ���������Ϊ14�������ԣ�����Щϵͳ��(��ϵͳ)
						*							û�ж�Ӧ��ϵ�ĵ����Ӧ�����ó�0XF��
						*		   2:���õ��������ظ��ˣ���ô��������յ�������ָ��ִ�С�
						*		   3:����Ŵ�0��ʼ��
						*		   4:��ͷ�����˳������:0,1,��ϵͳ�󴲶�Ŀ���
						*										 2,3,��ϵͳǰ����Ŀ���
						*										 4,5,��ϵͳ��/ǰ�����˵��
						*										 6,7,��ϵͳ�󴲶�Ŀ���
						*										 8,9,��ϵͳǰ����Ŀ���
						*										 10,11,��ϵͳ��/ǰ�����˵��
						*/

#define GET_STEPS_TYPE (0x77)
						/* ��ȡ��������Զ���*/
						/*	����1 ���ܺ�: (0-�޶���,1-��Ŀ���(���8��),2-��˵��(���4��), 3-�������(���4��),4-ѹ�ŵ��(���4��))*/
						/* 	*/




/*CMDTYPE_MOTOR_DENSITY*/
//new cmd

#define CMD_DENSITY_MOTOR_RST		(0x01)	// ���ж�Ŀ�������arg1=(0--all,1--left,2--right,3--sig) arg2 =(0-15)
#define CMD_DENSITY_GET_ZERO_WORK_BUSY		(0x02)	// ȡ��ͷ��Ŀ״̬arg1l(0-zero,1-work,2-busy,3-pos) arg1h(0-all,1-sig),arg2(0-15)
#define CMD_DENSITY_STEP_ISR		(0x03)	//ִ�ж�Ŀ����	
											/*		��Ŀ����					
												*		arg1_l	-��Ӧ��Ŀ�����
												*		arg2 -�������
												*		arg1_h - �Ƿ��鴫����2��״̬( 0--����飬!0--Ҫ��� )
											*/
#define CMD_DENSITY_SET_MOT_POS	(0x04)	// ���û�ͷָ����Ŀλ��arg1_l(0--����pos��1-����inputerr) arg1_h ��ʾ��Ŀ��(0-15) arg2 ��ʾ�����ֵ
#define CMD_DENSITY_SET_SPEED		(0x05)	//�����ٶ�arg1(0-��λ�ٶ�,1-����ٶ�,2,other) ,arg2 �ٶ�ֵ
#define CMD_DENSITY_SET_ATTRIBUTE 	(0x06)	// ���ö�Ŀ����arg1(0--enable,1--dir,2--fastmode,3--verystep,4--��⾫��,5--ϸ��6,--��λ����֮�����ߵĲ���) .arg2(attr_mask)
#define CMD_DENSITY_SET_DELAYTIME	(0x07)	// ���ö�Ŀ��λ����ʱʱ��arg1 �����(0-15)��arg2(ʱ��ֵms)
#define CMD_DENSITY_STEP_ISR_DD		(0x08)		// ִ�ж�̬��Ŀ����

#define CMD_DENSITY_SET_SPEED_EX		(0x0F)	//�����ٶ�,�µĽӿڣ��趨ֵΪƵ��ֵ()



//old cmd

	
#define STEP_ALL_RST	0x04	// ���ж�Ŀ�������
#define GET_ZERO 		0x0B	// ȡ��ͷ��Ŀ��λ״̬
#define GET_MOT 		0x0E	// ȡ��ͷ��Ŀλ��
#define GET_STEP_BUSY		0x22	// ����Ŀ���æ״̬
#define STEP_ISR			0x1C	// ��Ŀ����					
				/*		��Ŀ����					
				*		arg1	-��Ӧ��Ŀ�����
				*		arg2 -�������
				*		arg3 - �Ƿ��鴫����2��״̬( 0--����飬!0--Ҫ��� )
				*/
#define SET_STEP_ISR		0x28	// ���ö�Ŀλ��/*�������Ҳ����ͨ������ӿ�����*/
#define LEFT_STEP_RST_ISR	0x29	// ����Ŀ����
#define RIGHT_STEP_RST_ISR	0x2A 	// �Ҳ��Ŀ����
#define STEP_RST_SPD_ISR	0x2B	// ��Ŀ�����ٶ�
#define STEP_MAX_SPD_ISR	0x2C	// ��Ŀ�����ٶ�
#define SET_STEP_ENABLE		0x2F	// ���ö�Ŀ��������(Ĭ��ֵ0xFF)
#define SIG_STEP_RST			0x34	// ������Ŀ�������
#define SET_STEP_MODE		0x36	// ���ò����������ģʽ			??
#define SET_SETP_DIR		0x38	/* ���ò����������(Ĭ��ֵ0x0155)
					* arg1:ûʹ��
					*arg2 ����ֵdir_mask:
					 * ֧�ְ汾(0x2012)
					 */
#define SET_STEP_RESOLUTION	0x39	/* ���ò��������ϸ����
					 * (1, 2, 4, 8 ...)
					 * ֧�ְ汾(0x2013)
					 */
#define SET_STEP_CHECK		0x43 /*
				      * by xhl 2010/11/15
				      * ���õ��λ�ü�⾫��
				      * �����е����Ч
				      */

#define SET_STEP_FAST_MODE	0x44 /*	
				      * ���������г̵���Ŀ��λ��
				      * arg1 :����ţ���Ŀ���(0-15)
				      */

#define CMD_STEP_RESET_DELAY_TIME	0x4B /*
				      * ���������λʱ��ʱ����
				      *	����1: �����
				      *	����2: ��ʱʱ�䣨2ms��
				      */


#define CMD_SETUP_STEP		0x4C /*
				      * ������������������ýӿ�
				      * ����1: ��������
				      * ����2: ����ֵ
				      *
				      * ��������=
				      *		1: ��Ŀ��λ��������
				      *		2: ������λ��������
				      *		3: ��Ŀ��λ����
				      */
#define SET_STEPMOTOR_MODE_FAST		0x53 	/* by wf 2013/11/25
					* �ɱ��Ŀ����������������ú�ʹ��������������Ŀ,���ˣ�ѹ�ŵ��
					* bit0 = 1; sti���������� 0 ����ʹ��ԭ���ķ�ʽ
					* bit1 = 1; sk
					* bit2 = 1; yj
					* 2014/4 ��Ϊÿ��bit��Ӧ1�����
					*/
					
#define GET_STEPMOTOR_ZERO_SIG		0x54	/* by wf 2013/11/25
					* ��ȡ������Ŀ�������λ������arg1: ������1 - 8
					*/

#define GET_STEPMOTOR_BUSY_STATUS	0x56 /* by wf 2013/11/26
					  * ��Ŀ ���æ״̬��飬
					  * ����arg1: ������
					  */

//2014-03-05 
#define	  SET_MOTOR_ENABLE		(0x68)			//����ʹ�ܵ��
#define	  GET_MOTOR_NEWSTATE	(0x69)			//ȡ�����ӵĴ�������״̬




/*CMDTYPE_MOTOR_OTHER*/

//new cmd
#define CMD_OTHER_ISR				(0x01)	//�������arg1L(0-���,1--����,2--ѹ��3--other,) arg1H(������ID��(0-15))��arg2 posֵ
#define CMD_OTHER_RST				(0x02)	//�����λarg1L(0-���,1--����,2--ѹ��3--other,) arg1H(������ID��(0-15))    arg2(0-��λ���� ,!0-��λarg1h��ʾ�ĵ��)
#define CMD_OTHER_GET_ZERO_WORK_BUSY		(0x03)	// ��ȡ�����λ״̬(arg1L(0-���,1--����,2--ѹ��3--other,)arg1H(0-zero,1-����,2-busy��3-pos)),arg2l(0-15,�����) arg2h(0-���У�!0-����)
#define CMD_OTHER_GET_ZEROWORK	(0x04)	// ��ȡָ�����������״̬(arg1L(0-���,1--����,2--ѹ��3--other,)arg1H(������ID��(0-15)))      (����ֵ��bit0Ϊ0λ��bit1Ϊ����λ)
#define CMD_OTHER_SET_POS			(0x05) 	//���õ��������λ��ֵ(arg1L(0-���,1--����,2--ѹ��3--other,)arg1H(������ID��(0-15))),arg2(posֵ)
#define CMD_OTHER_SET_SPEED		(0x06)	 //���õ���������ٶ�(arg1L(0-���,1--����,2--ѹ��3--other,)arg1H(0- ��λ�ٶ�1-�����ٶ�,2-other)),arg2(�ٶ�ֵ)
#define CMD_OTHER_SET_STEPS		(0x07)	//���õ�������в���(arg1L(0-���,1--����,2--ѹ��3--other,)arg2(����))
#define CMD_OTHER_SET_OTHER		(0x08)    /*�������������һЩ�������������磬ʧ����ⷶΧ�����ϸ����*/
#define CMD_OTHER_SET_ACTION_STEP (0x09)   /*���� ���ǵ������*/

#define CMD_OTHER_SET_ACTION_HP_STEP (0x0B)   /*HP�� ���ǵ������ض�������*/

#define CMD_OTHER_SET_SPEED_EX	(0x0F)	/*��������������ٶ�ֵ,�µĽӿڣ�����ΪƵ��ֵ*/

//oldcmd

#define SK_ISR			0x1F	// ���˵������
#define SK_RST_ISR		0x20	// ���˵������
#define GET_SK_BUSY		0x21	// ������˵��æ״̬
#define GET_SK_ISR		0x26	// ȡ����λ��
								/*
								*	arg1 bit7=1�Ļ�������ȡѹ�ŵ�λ��
								*		����Ļ�������ȡ���˵�λ��
								* 	arg2 ��Ϊ0�Ļ����Ǿ���arg2���������˵ı��+1	(1-16);
								*		����Ļ�������arg1���������˵ı��(0-15)
								*/
#define SET_SK_ISR		0x27	// ��������λ��

#define SINKMOTOR_MAX_SPEED	0x48 /*
				      * ���˵�������ٶ�
				      */


#define CMD_FEET_SETUP		0x49 /*
				      * ѹ�ŵ����������
				      * ����1: ���÷���0 -- ���ò���,2 --ִ�ж���,3--����ѹ������
				      * ����2: ����
				      */
#define CMD_FEET_RUN		0x4A /*
				      * ѹ�ŵ������֧��
				      *	����1: �����
				      *	����2: 16bitָ��
				      *	  bit0~bit5:ѹ������
				      *	  bit14: 1->ѹ������ 0->ѹ��̧��
				      *	  bit15: 1->ѹ�Ÿ�λ
				      */
#define SET_SINKER_NUM			0x51 /* �������˵������ */


#define TRIANGLE_STEP_ISR		0x73 /* ���ǵ���������� 
							* ����1: ���ǵ����(0-3 Ȼ�󱾵�����ת����10-13).
							* ����2: 	arg2   bit15=0,		��ʾλ��ֵ(bit0,bit1)(01--left,11--zero,10--right,0--reset)
							*				  bit15 =1��	bit0-14��ʾʵ����Ҫ�ߵ���λ�á�		
							*����3:   	arg3   bit0,bit1��ʾ��ʱ�Ƿ���Ҫ�����Ӧ��������״̬				
							
							*/
#define TRIANGLE_STEP_GET 	0x74
							/* ���ǵ�״̬��ȡ
							* ����1: ���ǵ����(0-3 Ȼ�󱾵�����ת����10-13).
							* ����2:arg2>0  ��ʾ��ȡ���ǵ����ʵ��postion ֵ��
							*			���򣬻�ȡ���ǵ����ǰλ�õĴ������ź�.							
							*/

#define TRIANGLE_STEP_SET 	0x75
							/* ����λ������
							* ����1: ���ǵ����(0-3 Ȼ�󱾵�����ת����10-13).
							* ����2:����ֵ
							* ����3:arg3>0  ��ʾ�������ǵ����ʵ��postion ֵ��
							*			�����������ǵ����ǰλ�õĴ������ź�.							
							*/

/*CMDTYPE_DCT*/

//new cmd
#define CMD_DCT_ISR			(0x01)	//���������arg1L(0-ɴ��,1--������,2,--ǰ������) arg1H(0-7�������),arg2(0,!0)
#define CMD_DCT_GET_STS	(0x02) 	//ȡ״̬arg1(0-ɴ��״̬,1-������״̬,2-ǰ������״̬)
#define CMD_DCT_SET_HV		(0x03)	//���õ������ѹ(arg1,0- ��ѹ1��1--��ѹ2,)	arg2 ��ѹֵ
#define CMD_DCT_WORKMODE	(0x04)	//���õ��������ģʽ arg1L(0-ɴ��,1--������,2,--ǰ������) arg1H(0-7�������),arg2(0,!0--��ͨ��)
#define CMD_DCT_E644MODE	(0x09)	//����



//old cmd

#define GET_DCT			0x09	// ȡɴ��״̬
#define GET_ACT 			0x0C		// ȡ��ͷ����״̬
#define DCT_HV_ISR 		0x1A	// ���õ������ѹ
#define DCT_ISR			0x1E	// ���������
#define DCT_HV1_ISR		0x35	// ���õ������ѹ2
#define SET_EMF_WORK_MODE	0x41 /*
				      *	by xhl 2010/06/17 
				      * ֧�ְ汾(0x2016)
				      * �����������ǵ�����Ƿ����ڳ�ͨ��
				      * ģʽ��
				      * ������ + ��������(arg1:0-11) + ����ģʽ(arg2:0,!0)
				      * ��arg2 Ϊ����ʱ���õ���������ڳ�ͨ��ģʽ��(��ͨIO)
				      */

/*CMDTYPE_XZQ*/

//new cmd


#define CMD_XZQ_INIT			(0x01)	//����ѡ������ʼ��
#define CMD_XZQ_ZHEN_ISR 		(0x02)	// ����ѡ������.�������ɵ���canID ���͸�ϵͳ���н���arg1l- ������,arg1h ѡ����mask ,arg2,arg3 ����ϵͳ
#define  CMD_XZQ_TEST_ISR		(0x03)	//ѡ��������(��ͷ���Խ������) arg1(0-7 ѡ���) arg2 (ÿ��״̬sts(bit0-7))
#define CMD_XZQ_GET_STS 		(0x04)	// ȡ��ͷѡ����״̬(arg1(0-3)��ʾ)
#define CMD_XZQ_GET_DAO_ISR	 (0x05)	// ȡ��ǰ��
#define CMD_XZQ_GET_READY_BLADE 	(0x06) // ȡ��ͷѡ������ǰ�����Ĺ�����ͷ arg1(0-7 ѡ���)


#define CMD_XZQ_SET_START		(0x07)  //����ѡ��������arg1(0-7) arg2(��id��0-7),
#define CMD_XZQ_SET_HLV		(0x08) //����ѡ�����ĸ�ѹ���ѹʱ��arg1(0-��ѹ��1-��ѹ) arg2(����ʱ��ֵ)
#define CMD_XZQ_SET_DAO_ISR	(0x09) // ���õ�ǰ�� arg1(��ǰ��)
#define CMD_XZQ_SET_DAO_MAX	(0x0A) // �����ܵ���arg1(0-15) ��ʾ1-16��
#define CMD_XZQ_SET_WORKMODE	(0x0B) //����ѡ��������ģʽarg1(0-����ģʽ��!0--һ��ѡ�������������)
#define CMD_XZQ_SET_NEEDLE_STEP (0x0C)// ѡ�������ж� ���� �ӼӼ�1��Ϊ�Ӽ�2��arg1(��ʾ���岽��)




#define HP3G_NEEDLE_2STEP		0x72 /* �������������������
							* ѡ�������ж� ���� �ӼӼ�1��Ϊ�Ӽ�2��
							*/
#define ST_INIT			0x05	// ����ѡ������ʼ��
#define GET_XZQ 			0x0D		// ȡ��ͷѡ����״̬
#define ZHEN_ISR		0x0F	// ����ѡ������
#define ST1_ISR			0x11	// ����1��ѡ������
#define ST2_ISR			0x12	// ����2��ѡ������
#define ST3_ISR			0x13	// ����3��ѡ������
#define ST4_ISR			0x14	// ����4��ѡ������
#define ST5_ISR			0x15	// ����5��ѡ������
#define ST6_ISR			0x16	// ����6��ѡ������
#define ST7_ISR			0x17	// ����7��ѡ������
#define ST8_ISR			0x18	// ����8��ѡ������
#define XZQ_HV_ISR 		0x19	// ����ѡ������ѹ
#define XZQ_LV_ISR		0x1B	// ����ѡ������ѹ
#define XZQ_ISR			0x1D	// ѡ��������, test
#define GET_DAO_ISR		0x23	// ȡ��ǰ��
#define DAO_ISR			0x24	// ���õ�ǰ��
#define DAO_MAX_ISR		0x25	// �����ܵ���
									/* arg1 ��ʾ�ܵ���-1.	(0-15) ��ʵ���ܵ���(1-16)
									*
									*/	
#define SET_XZQ_MODE_F		0x31	// ����ǰ��ѡ������
#define SET_XZQ_MODE_B		0x32	// ���ͺ�ѡ������
#define SET_XZQ_MODE_TYPE	0x33	/*
					 * ����ǰ�󴲹���ѡ����
					 * ������ + ǰ�� + ��
					 */
#define GET_JQD_BLADE		0x42 /*
				      * by xhl 2010/08/09
				      * ȡ��ͷѡ������ǰ�����Ĺ�����ͷ
				      * ������ + ѡ�������(0 -- 7)
				      */
#define CMD_PARA_SETUP		0x4D /*
				      * �����������ýӿ�
				      * ����1: ��������
				      * 0x10: ѡ��������ӳ����ʼ��
				      * 0x18: ɴ�쿪����ʱ��
				      * 0x19:ɴ��ر���ʱ��
				      */

#define JAQ_WORKMODE_8_16_SET		0x4E /*
				      * ѡ���������������ýӿ�
				      * ����1: ��������
				      * 1: ѡ����������8_16ģʽ��
				      * 0: ѡ�����ظ���8·ģʽ
				      */

#define ST9_ISR			0x60	// ����9��ѡ������
#define ST10_ISR		0x61	// ����10��ѡ������
#define ST11_ISR		0x62	// ����11��ѡ������
#define ST12_ISR		0x63	// ����12��ѡ������
#define ST13_ISR		0x64	// ����13��ѡ������
#define ST14_ISR		0x65	// ����14��ѡ������
#define ST15_ISR		0x66	// ����15��ѡ������
#define ST16_ISR		0x67	// ����16��ѡ������



/*CMDTYPE_ALARM*/
//new cmd


#define CMD_ALARM_CLR	(0x01)	//���
#define CMD_ALARM_GET	(0x02)	// ȡ��ͷ����״̬arg1(0-��ͷ����״̬,1-���򴫸�������״̬  2-���ش�����)
#define CMD_ALARM_SET_ENABLEMASK	(0x03)	// ���û�ͷ��������(arg1��bit0-15 Ϊ0��ʾ���Σ�Ϊ1��ʾ����)
#define CMD_ALARM_SET_OVERLOADENABLE	(0x04)  // ������⹦������arg1(0-��ֹ,!0--����)
#define CMD_ALARM_SET_TANZHEN	(0x05)	//arg1L(0-�ر�,1-����1��,2-����2��,3-����1�ź�2��) arg1H(0-�رգ�!0��ʾ��)arg2(�ӳ�ʱ��)
#define CMD_ALARM_CHECK		(0x06)	//arg1l(0-��ʼ��!0-����) arg1H(bitmask) arg2(����)
#define CMD_ALARM_SHOCK	(0x07)  //ײ����������arg1_l(0--��λ��1--�Ƿ�ʹ�ܣ�2--����������)��arg1_h(0-��ʹ��,!0-ʹ��),(0-100��ʾ������) ײ��������0-�����ڹر�.

#define CMD_ALARM_GETALERTSTR_CH		(0x08) 		//��ȡ��������arg1(alert_code),arg2(alert_arg,(--����)
#define CMD_ALARM_GETALERTSTR_EN		(0x09) 	//��ȡ��������arg1(alert_code),arg2(alert_arg,(--Ӣ��)
#define CMD_ALARM_SET_TEMP_DATA		(0x0C)	//���±����趨ֵ
#define CMD_ALARM_MODE_GET			(0x0F)	//��ȡ��ͷ�ı�����������




#define GET_DIR_ERR		0x06	
#define CLR_ERR			0x07	// �����ͷ����
#define GET_ERR			0x0A	// ȡ��ͷ����״̬
#define SET_ERR_ENABLE_ISR	0x2D	// ���û�ͷ��������

#define OVERLOAD_SETUP		0x47 /*
				      * ������⹦������
				      * 0: ����
				      * 1: ����
				      */
#define SET_TANZHEN_KNIT_DELAY	0x52	/* by wf 2013 11 05
					* ���ٻ�ת������̽�뱨���Ŀ��ƣ�
					* ����1 0:�ر�����̽�뱨����1:����1�ţ�2:����2�ţ�3:����1��2��
					* ����2 arg1>0ʱ��=0���رձ�����=1���򿪱�����>1:��ʱʱ�䣬ʱ����100msΪ��λ
					*/    
#define GET_ERROR_CODE		0x59 /*
				      * ���ش�����
				      */


#define TANZHEN_SETUP		0x6a
#define TANZHEN_NEW_SET	0x6b


#define CHECK_STATUS_START	0x70	/* ��ʼ��ѯ 
						* bit0: ���򴫸����¼�ⷽʽ��(Խ�� 1 - 0 - 1)
						*/
#define CHECK_STATUS_STOP		0x71	/* ������ѯ 
						* arg1:	bit0: ���򴫸����¼�ⷽʽ��Խ��
						* 		bit1: ���˵����ѯ
						*		bit2: ���Ŀ left sti prepared 0/2/4/6/
						*		bit3: �Ҷ�Ŀ right sti prepared	1/3/5/7/
						*		bit4: ɴ��״̬ yarn zero
						* arg2: param;
						*/


/*CMDTYPE_OTHERSET*/

#define CMD_OTHERSET_GET_VER		(0x01) //ȡ��ͷ����汾��
#define CMD_OTHERSET_GET_TYPE		(0x02) //ȡ��ͷ������
#define CMD_OTHERSET_GET_FUNC	(0x03) //ȡ��ͷ�幦��
#define CMD_OTHERSET_GET_BTIME	(0x04) //��ͷ��������ʱ��
#define CMD_OTHERSET_DEBUG		(0x05) //����

#define CMD_OTHERSET_REQUEST		(0x06) //��������

#define CMD_OTHERSET_RETURN_YARN_ZERO_ST   (0x0C)  

#define CMD_OTHERSET_TEST_HARD_FAULT	(0x0E) //�������һ�������ж�

#define CMD_OTHERSET_GET_HARD_FAULT_MSG	(0x0F) //��ȡ�쳣�����ʱ������ֵ

#define CMD_OTHERSET_GET_TIME2MAX_DELAY		(0x10) //��ȡ�쳣�����ʱ������ֵ

#define CMD_OTHERSET_TEST_YARN_DELAY		(0x11) //����ɴ��˫��ͬʱ��ͨ�����



#define GET_VER_ISR		0x30	// ȡ��ͷ����汾��
#define HEAD_DEBUG		0x37	// ���Խӿ�
#define CMD_HEAD_LOG_ENABLE	0xF1 /* wf, 2013/10/21
						* ����arg1:
						* 0x01	��ѡ�������		0x81	�ر�ѡ�������
						* 0x02	�����ǵ�������	0x82	�ر����ǵ�������
						* 0x03	��ɴ���������	0x83	�ر�ɴ���������
						* 0x04	�򿪶�Ŀ��������	0x84	�رն�Ŀ��������
						* 0x05	�����˵�������	0x85	�ر����˵�������
						* 0x06	��ѹ�ŵ�������	0x87	�ر�ѹ�ŵ�������
						* 0x0F	������			0x8F	�ر�����
						*/



#define UPGRADE_REQUEST		0x3A	/* �������� */ 
#define UPGRADE_DATA		0x3B	/* �������� */ 
#define UPGRADE_ENDDATA		0x3C	/* �������� */ 
#define UPGRADE_DATA_RECEIVED	0x3D	/* �������� */ 
#define UPGRADE_SUCCESS		0x3E	/* �������� */ 
#define UPGRADE_FORCE_REQ	0x3F	/* �������� */ 

#define GET_BOARD_TYPE		0x40	// ֧�ְ汾(0x2015)

#define GET_BOARD_FUNC_TYPE		0x4F	/* ֧�ְ汾 0x2065
					  * ��ͷ�书������
					  * bit1-bit0: ϵͳ����1-4
					  * bit2 = 1: ֧�ֱ�����
					  * bit3 = 1: ֧��ѹ��
					  */


#define GET_BOARD_BUILD_TIME		0x50    /* ֧�ְ汾 0x2065
					  * ��ͷ��������ʱ��
					  */




  #define TYPE_LF2407		0x8001
  #define TYPE_LPC2919		0x8002
  #define TYPE_F2812			0x8003
  #define TYPE_STM32E457	0x8006


#define TYPE_NUM	0x80							
#define TYPE_STI	0x01
#define TYPE_SK		0x02
#define TYPE_ACT	0x03
#define TYPE_YJ		0x04
#define TYPE_DETECT1	0x05
#define TYPE_DETECT2	0x06
#define TYPE_DIR1		0x07
#define TYPE_DIR2		0x08
#define TYPE_ENABLE	0x80



#define LOGOUT_XZQ_MASK	0x01
#define LOGOUT_DCT_MASK	0x02
#define LOGOUT_YARN_MASK	0x04
#define LOGOUT_STI_MASK	0x08
#define LOGOUT_SK_MASK		0x10
#define LOGOUT_YJ_MASK		0x20



/*
*	wf, 2013/10/21
*	��ͷlog��� cmd
*/
#define LOG_OUT_XZQ		0x10
#define LOG_OUT_DCT		0x11
#define LOG_OUT_YARN		0x12
#define LOG_OUT_STI1		0x13
#define LOG_OUT_STI2		0x14
#define LOG_OUT_STI3		0x15
#define LOG_OUT_STI4		0x16
#define LOG_OUT_STI5		0x17
#define LOG_OUT_STI6		0x18
#define LOG_OUT_STI7		0x19
#define LOG_OUT_STI8		0x1A
#define LOG_OUT_SK1		0x1B
#define LOG_OUT_SK2		0x1C
#define LOG_OUT_SK3		0x1D
#define LOG_OUT_SK4		0x1E
#define LOG_OUT_YJ1		0x1F	//ѹ��
#define LOG_OUT_YJ2		0x20
#define LOG_OUT_YJ3		0x21
#define LOG_OUT_YJ4		0x22
/* log out end */

// ˵��:
// �汾��Ϣ���4λ����ϵͳ��
// ��:
//    �汾Ϊ0x2011->֧�ֵ�˫ϵͳ �汾��Ϊ0x0011
//    �汾Ϊ0x3011->֧����ϵͳ �汾��Ϊ0x0011
//
// ��ʹ�ð汾��Ϊ0x2010֮��ĳ���
// �����ð汾��ѯ����(0x30)��ѯ����汾
//
//
