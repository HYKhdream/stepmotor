#ifndef __ALERT_H__
#define __ALERT_H__

#include "config.h"

// error define
//#define STEP_ERR	0xB0	/* ��Ŀ���1-8�� ���� */
//#define DIR1_ERR	0xB8	/* ���򴫸��� */
//#define DIR2_ERR	0xB9	/* �ҷ��򴫸��� */
#define CAN_BUF_ERR	0xBA	/* CAN��������� */

#if 0
#ifdef	ALERT_STM32
#define YARN_PERR	0xBD //0xC2	/* ����24v����˿ʧЧ     *//* F1 -24V*/
#define V12_PERR  	0xBE //0xCA	/* ����12v����˿ʧЧ     *//* F2 +12V*/
#define XZQ1_PERR	0xBF //0xBB	/* ��ͷ��1-24v����˿ʧЧ     *//* F3 -24v*/
#define ACT1_PERR	0xC2 //0xBD	/* ��ͷ��1+24v����˿ʧЧ     *//* F4 +24v*/
#ifdef SUPPORT_2S
#define ACT2_PERR	0xBB //0xBE	/* ��ͷ��2-24v����˿ʧЧ     *//* F5 -24V*/
#define XZQ2_PERR	0xBC	/* ��ͷ��2+24v����˿ʧЧ     *//* F6 +24v*/
#define STEP_PERR	0xCB //0xBF	/* ������24v����˿ʧЧ     *//* F7 24V*/
#else
#define STEP_PERR	0xBC //0xBF	/* ��ϵͳ ���24V F6 24V*/
#endif
#else
#define XZQ1_PERR	0xBB	/* ��ͷ��1-24v����˿ʧЧ     *//* F3 -24v*/
#define XZQ2_PERR	0xBC	/* ��ͷ��2+24v����˿ʧЧ     *//* F6 +24v*/
#define ACT1_PERR	0xBD	/* ��ͷ��1+24v����˿ʧЧ     *//* F4 +24v*/
#define ACT2_PERR	0xBE	/* ��ͷ��2-24v����˿ʧЧ     *//* F5 -24V*/
#define STEP_PERR	0xBF	/* ������24v����˿ʧЧ     *//* F7 24V*/
#define YARN_PERR	0xC2	/* ����24v����˿ʧЧ     *//* F1 -24V*/
#define V12_PERR  	0xCA	/* ����12v����˿ʧЧ     *//* F2 +12V*/
#endif

#endif


#define DSP_FATAL_ERR	0xC0	/* MCUʧЧ */


#define ACT_STEP_FAULT1	0xA4  /* �������1���� */
#define ACT_STEP_FAULT2	0xA5  /* �������2���� */
#define ACT_STEP_FAULT3	0xA6  /* �������3���� */
#define ACT_STEP_FAULT4	0xA7  /* �������4���� */


#define YARN_STEP1_ERR	(0xD5)   /*ɳ�����1����*/
#define YARN_STEP2_ERR	(0xD6)   /*ɳ�����2����*/
#define YARN_STEP3_ERR	(0xD7)   /*ɳ�����3����*/
#define YARN_STEP4_ERR	(0xD8)   /*ɳ�����4����*/

#define YARN_STEP5_ERR	(0xD9)   /*ɳ�����5����*/
#define YARN_STEP6_ERR	(0xDA)   /*ɳ�����6����*/
#define YARN_STEP7_ERR	(0xDB)   /*ɳ�����7����*/
#define YARN_STEP8_ERR	(0xDC)   /*ɳ�����8����*/

#define YARN_DC24_LOW	(0xDE)  /*���24����ԴǷѹ*/		
#define YARN_DC24_HIGH	(0xDF)  /*���24����Դ��ѹ*/
#define YARN_DC12_LOW	(0xE0)  /*������12����ԴǷѹ*/
#define YARN_DC12_HIGH	(0xE1)  /*������12����Դ��ѹ*/



#if 0
#define RIGHT_TANZHEN_ERR	0xC9	/* ��̽�� */
#define LEFT_RIGHT_TANZHEN_ERR 	(0xCA)    /*����̽��*/
#define LEFT_TANZHEN_ERR	0xCB	/* ��̽�� */



//#define STEP2_PERR	0xCB	/* ������24v����˿ʧЧ     *//* F7 +24V*/

#define OVERLOAD_HEAD	0xFD /* ��ͷ��������� */
#define OVERLOAD_YARN	0xA0 /* ɴ����������
			      * ���� ��Ӧɴ���
			      */
#define OVERLOAD_JQD	0xA1 /* ѡ��������
			      * ����
			      *   �߰�λΪѡ������
			      *   �Ͱ�λΪ����ͷ��
			      */
#define OVERLOAD_ACT	0xA2 /* �������������
			      * ����
			      *   �߰�λΪ���
			      *   �Ͱ�λΪ�������
			      */
#define YARN_ACT_FAULT        0xA3 /*ɴ����������*/



#define	DIR_ERR			0xD0	/* Խ�����µĻ����ⷽʽ��*/
#define	SINKER_DONE_ERR	0xD2	/* Խ�������˶���δ��� */
#define	STI_DONE_ERR	0xD3	/* Խ������Ŀ����δ��� */

#define DEVICE_CONFIG_NUM_ERROR	0xD4	/* ������� ������ ���� */

#define ENCODE_ERROR1	0xe1 /* 1�ŵ�� �������� */
#define ENCODE_ERROR2	0xe2 /* 2�ŵ�� �������� */
#define ENCODE_ERROR3	0xe3 /* 3�ŵ�� �������� */
#define ENCODE_ERROR4	0xe4 /* 4�ŵ�� �������� */
#define ENCODE_ERROR5	0xe5 /* 5�ŵ�� �������� */
#define ENCODE_ERROR6	0xe6 /* 6�ŵ�� �������� */
#define ENCODE_ERROR7	0xe7 /* 7�ŵ�� �������� */
#define ENCODE_ERROR8	0xe8 /* 8�ŵ�� �������� */
#endif
void alert_init(void);
int alert_count(void);
int alert_push(int alert_code, int alert_arg);
int alert_pop(int *alert_code, int *alert_arg);
int alert_find(int alert_code, int alert_arg);
int alert_delete(int alert_code, int alert_arg);
void Alert_Clear(void);
void Alert_Poll(void);

#endif

