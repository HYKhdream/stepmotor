#ifndef __ALERT_H__
#define __ALERT_H__

// error define
#define STEP_ERR		0xB0	/* ��Ŀ���1-8�� ���� */
#define DIR1_ERR			0xB8	/* ���򴫸��� */
#define DIR2_ERR			0xB9	/* �ҷ��򴫸��� */
#define CAN_BUF_ERR		0xBA	/* CAN��������� */
//#define CAN_SEND_ERR	0x

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

#define DSP_FATAL_ERR	0xC0	/* ��ͷ��ʧЧ */
#define LINE_ERR		0xC1	/* ̽�� */
#define SINKER1_ERR	0xC3	/* �������1 ���� */
#define SINKER2_ERR	0xC4	/* �������2 ���� */
#define SINKER3_ERR	0xCE	/* �������3 ���� */
#define SINKER4_ERR	0xCF	/* �������4 ���� */

#define FEET1_ERR		0xC5	/* ѹ�����1���� */
#define FEET2_ERR		0xC6	/* ѹ�����2���� */
#define FEET3_ERR		0xC7	/* ѹ�����3���� */
#define FEET4_ERR		0xC8	/* ѹ�����4���� */

#define TRIANGLE1_ERR	(0xD5)   /*�������1����*/
#define TRIANGLE2_ERR	(0xD6)   /*�������2����*/
#define TRIANGLE3_ERR	(0xD7)   /*�������3����*/
#define TRIANGLE4_ERR	(0xD8)   /*�������4����*/



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

#define YARN_STATE_NOT_CLEAR   0x30  /*ɴ��״̬δ����*/

#define ACT_STEP_FAULT1	0xA4  /* �������1���� */
#define ACT_STEP_FAULT2	0xA5  /* �������2���� */
#define ACT_STEP_FAULT3	0xA6  /* �������3���� */
#define ACT_STEP_FAULT4	0xA7  /* �������4���� */

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

/*ѡ�������µ�Ƭͬʱ��ͨ*/
#define JQD_OPERATE_ERROR	(0xEE)		

#endif

