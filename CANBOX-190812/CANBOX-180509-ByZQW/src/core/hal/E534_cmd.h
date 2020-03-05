
#define POWER_ON		0x00	// ����ӵ�
#define POWER_OFF		0x01	// ����ϵ�
#define STEP_ON			0x02	// ��������ϵ�
#define STEP_OFF		0x03	// ��������ϵ�
#define STEP_ALL_RST	0x04	// ���ж�Ŀ�������
#define ST_INIT			0x05	// ����ѡ������ʼ��
#define GET_DIR_ERR		0x06	// ��黻�򴫸�������״̬
#define CLR_ERR			0x07	// �����ͷ����
#define GET_DIR			0x08	// ȡ��ͷ���򴫸���״̬
#define GET_DCT			0x09	// ȡɴ��״̬
#define GET_ERR			0x0A	// ȡ��ͷ����״̬
#define GET_ZERO 		0x0B	// ȡ��ͷ��Ŀ��λ״̬
#define GET_ACT 		0x0C	// ȡ��ͷ����״̬
#define GET_XZQ 		0x0D	// ȡ��ͷѡ����״̬
#define GET_MOT 		0x0E	// ȡ��ͷ��Ŀλ��
#define ZHEN_ISR		0x0F	// ����ѡ������
#define DIR_ISR			0x10	// ���û�ͷ��������
#define ST1_ISR			0x11	// ����1��ѡ������
#define ST2_ISR			0x12	// ����2��ѡ������
#define ST3_ISR			0x13	// ����3��ѡ������
#define ST4_ISR			0x14	// ����4��ѡ������
#define ST5_ISR			0x15	// ����5��ѡ������
#define ST6_ISR			0x16	// ����6��ѡ������
#define ST7_ISR			0x17	// ����7��ѡ������
#define ST8_ISR			0x18	// ����8��ѡ������
#define XZQ_HV_ISR 		0x19	// ����ѡ������ѹ
#define DCT_HV_ISR 		0x1A	// ���õ������ѹ
#define XZQ_LV_ISR		0x1B	// ����ѡ������ѹ
#define STEP_ISR		0x1C	// ��Ŀ����					
/*		��Ŀ����					
*		arg1	-��Ӧ��Ŀ�����
*		arg2 -�������
*		arg3 - �Ƿ��鴫����2��״̬( 0--����飬!0--Ҫ��� )
*/
#define XZQ_ISR			0x1D	// ѡ��������, test
#define DCT_ISR			0x1E	// ���������
#define SK_ISR			0x1F	// ���˵������
#define SK_RST_ISR		0x20	// ���˵������
#define GET_SK_BUSY		0x21	// ������˵��æ״̬
#define GET_STEP_BUSY		0x22	// ����Ŀ���æ״̬
#define GET_DAO_ISR		0x23	// ȡ��ǰ��
#define DAO_ISR			0x24	// ���õ�ǰ��
#define DAO_MAX_ISR		0x25	// �����ܵ���
#define GET_SK_ISR		0x26	// ȡ����λ��
#define SET_SK_ISR		0x27	// ��������λ��
#define SET_STEP_ISR		0x28	// ���ö�Ŀλ��
#define LEFT_STEP_RST_ISR	0x29	// ����Ŀ����
#define RIGHT_STEP_RST_ISR	0x2A 	// �Ҳ��Ŀ����
#define STEP_RST_SPD_ISR	0x2B	// ��Ŀ�����ٶ�
#define STEP_MAX_SPD_ISR	0x2C	// ��Ŀ�����ٶ�
#define SET_ERR_ENABLE_ISR	0x2D	// ���û�ͷ��������
#define SET_MODE_ISR		0x2E	// ��ϵͳģʽ????
#define SET_STEP_ENABLE		0x2F	// ���ö�Ŀ��������(Ĭ��ֵ0xFF)
#define GET_VER_ISR		0x30	// ȡ��ͷ����汾��
#define SET_XZQ_MODE_F		0x31	// ����ǰ��ѡ������
#define SET_XZQ_MODE_B		0x32	// ���ͺ�ѡ������
#define SET_XZQ_MODE_TYPE	0x33	/*
					 * ����ǰ�󴲹���ѡ����
					 * ������ + ǰ�� + ��
					 */
#define SIG_STEP_RST		0x34	// ������Ŀ�������
#define DCT_HV1_ISR		0x35	// ���õ������ѹ2
#define SET_STEP_MODE		0x36	// ���ò����������ģʽ
#define HEAD_DEBUG		0x37	// ���Խӿ�
#define SET_SETP_DIR		0x38	/* ���ò����������(Ĭ��ֵ0x0155)
					 * ֧�ְ汾(0x2012)
					 */
#define SET_STEP_RESOLUTION	0x39	/* ���ò��������ϸ����
					 * (1, 2, 4, 8 ...)
					 * ֧�ְ汾(0x2013)
					 */
#define UPGRADE_REQUEST		0x3A	/* �������� */ 
#define UPGRADE_DATA		0x3B	/* �������� */ 
#define UPGRADE_ENDDATA		0x3C	/* �������� */ 
#define UPGRADE_DATA_RECEIVED	0x3D	/* �������� */ 
#define UPGRADE_SUCCESS		0x3E	/* �������� */ 
#define UPGRADE_FORCE_REQ	0x3F	/* �������� */ 

#define GET_BOARD_TYPE		0x40	// ֧�ְ汾(0x2015)
  #define TYPE_LF2407	0x8001
  #define TYPE_LPC2919	0x8002
  #define TYPE_F2812		0x8003

#define SET_EMF_WORK_MODE	0x41 /*
				      *	by xhl 2010/06/17 
				      * ֧�ְ汾(0x2016)
				      * �������õ�����Ƿ����ڳ�ͨ��
				      * ģʽ��
				      * ������ + �������� + ����ģʽ
				      * ������1Ϊ����ʱ���õ����������
				      * ��ͨ��ģʽ��
				      * ����0Ϊ��������: 0 -- 39
				      * ����1Ϊ����ģʽ: 0/!0
				      */
#define GET_JQD_BLADE		0x42 /*
				      * by xhl 2010/08/09
				      * ȡ��ͷѡ������ǰ�����Ĺ�����ͷ
				      * ������ + ѡ�������(0 -- 7)
				      */
#define SET_STEP_CHECK		0x43 /*
				      * by xhl 2010/11/15
				      * ���õ��λ�ü�⾫��
				      * �����е����Ч
				      */
#define SET_STEP_FAST_MODE	0x44 /*
				      * ���������г̵���Ŀ��λ��
				      */
#define ENCODER_SETUP		0x45 /*
				      * �����������������
				      * 0: ����
				      * 1: ����
				      */
#define ENCODER_CHECK		0x46 /*
				      * ���ظ���������״̬
				      */
#define OVERLOAD_SETUP		0x47 /*
				      * ������⹦������
				      * 0: ����
				      * 1: ����
				      */
#define SINKMOTOR_MAX_SPEED	0x48 /*
				      * ���˵�������ٶ�
				      */
#define CMD_FEET_SETUP		0x49 /*
				      * ѹ�ŵ����������
				      * ����1: ���÷���
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
#define CMD_PARA_SETUP		0x4D /*
				      * �����������ýӿ�
				      * ����1: ��������
				      * 0x10: ѡ��������ӳ����ʼ��
				      *
				      *
				      */

#define JAQ_WORKMODE_8_16_SET		0x4E /*
				      * ѡ���������������ýӿ�
				      * ����1: ��������
				      * 1: ѡ����������8_16ģʽ��
				      * 0: ѡ�����ظ���8·ģʽ
				      */
#define GET_BOARD_FUNC_TYPE		0x4F	/* ֧�ְ汾 0x2065
					  * ��ͷ�书������
					  * bit1-bit0: ϵͳ����1-4
					  * bit2 = 1: ֧�ֱ�����
					  * bit3 = 1: ֧��ѹ��
					  */
#define GET_BOARD_BUILD_TIME		0x50    /* ֧�ְ汾 0x2065
					  * ��ͷ��������ʱ��
					  */
#define SET_SINKER_NUM			0x51 /* �������˵������ */

#define SET_TANZHEN_KNIT_DELAY	0x52	/* by wf 2013 11 05
					* ���ٻ�ת������̽�뱨���Ŀ��ƣ�
					* ����1 0:�ر�����̽�뱨����1:����1�ţ�2:����2�ţ�3:����1��2��
					* ����2 arg1>0ʱ��=0���رձ�����=1���򿪱�����>1:��ʱʱ�䣬ʱ����100msΪ��λ
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
					
#define STEP_ENCODER_CHECK	0x55 /*
				      * ����ָ�������������״̬
				      */
#define GET_STEPMOTOR_BUSY_STATUS	0x56 /* by wf 2013/11/26
					  * �ɱ��Ŀ ���æ״̬��飬
					  * ����arg1: ������
					  */



					  
#define GET_ZERO_OTHER 		0x5B	// ȡ��ͷ�����������λ״̬�����˶�Ŀ������



//0x5c--0x5f  Ԥ�����ɱ��Ŀ����� 

#define ST9_ISR			0x60	// ����9��ѡ������
#define ST10_ISR		0x61	// ����10��ѡ������
#define ST11_ISR		0x62	// ����11��ѡ������
#define ST12_ISR		0x63	// ����12��ѡ������
#define ST13_ISR		0x64	// ����13��ѡ������
#define ST14_ISR		0x65	// ����14��ѡ������
#define ST15_ISR		0x66	// ����15��ѡ������
#define ST16_ISR		0x67	// ����16��ѡ������

//2014-03-05 
#define	  SET_MOTOR_ENABLE		(0x68)			//����ʹ�ܵ��
#define	  GET_MOTOR_NEWSTATE	(0x69)			//ȡ�����ӵĴ�������״̬

#define GET_ERROR_CODE		0x59 /*
				      * ���ش�����
				      */

#define TANZHEN_SETUP		0x6a
#define TANZHEN_NEW_SET	0x6b

#define CMD_EXTENDED_	0x6c
/*��չ����Ժ����е����ӵ�����������arg1 ��ʾ�����arg2��arg3��ʾ����
arg1 = 0x01����Ŀ��ɻظ����
arg1 = 0x02�� ѡ�뵥����ͷ������arg2_L :�ڼ���ѡ����,arg2_H:�ڼ�����arg3:on_off(0,!0)*/


#define CMD_DEBUG_GETSTEP_DATA 0x6d

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
						
#define HP3G_NEEDLE_2STEP		0x72 /* �������������������
							* ѡ�������ж� ���� �ӼӼ�1��Ϊ�Ӽ�2��
							*/

#define KEYDOG_READ		0x33
#define KEYDOG_CHECK		0x35

#define NEW_STEP_TYPE_SET		0x80 /* ����������� 
							* ����1: ��������
							* ����2: start index
							* ����3: stop index
							*/
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
						*    	ע��	1: Ŀǰ��ͷ���������Ϊ14�������ԣ�����Щϵͳ��(��ϵͳ)
						*			û�ж�Ӧ��ϵ�ĵ����Ӧ�����ó�0XF��
						*		2:���õ��������ظ��ˣ���ô��������յ�������ָ��ִ�С�
						*		3:����Ŵ�0��ʼ��
						*		4:��ͷ�����˳������:	0,1,��ϵͳ�󴲶�Ŀ���
						*					2,3,��ϵͳǰ����Ŀ���
						*					4,5,��ϵͳ��/ǰ�����˵��
						*					6,7,��ϵͳ�󴲶�Ŀ���
						*					8,9,��ϵͳǰ����Ŀ���
						*					10,11,��ϵͳ��/ǰ�����˵��
						*/

#define GET_STEPS_TYPE (0x77)
						/* ��ȡ��������Զ���*/
						/*	����1 ���ܺ�: (0-�޶���,1-��Ŀ���(���8��),2-��˵��(���4��), 3-�������(���4��),4-ѹ�ŵ��(���4��))*/
						/* 	*/


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
