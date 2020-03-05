#ifndef __ALERT_H__
#define __ALERT_H__

#include "config.h"

// error define
//#define STEP_ERR	0xB0	/* 度目马达1-8号 故障 */
//#define DIR1_ERR	0xB8	/* 左方向传感器 */
//#define DIR2_ERR	0xB9	/* 右方向传感器 */
#define CAN_BUF_ERR	0xBA	/* CAN缓冲区溢出 */

#if 0
#ifdef	ALERT_STM32
#define YARN_PERR	0xBD //0xC2	/* 主板24v保险丝失效     *//* F1 -24V*/
#define V12_PERR  	0xBE //0xCA	/* 主板12v保险丝失效     *//* F2 +12V*/
#define XZQ1_PERR	0xBF //0xBB	/* 机头板1-24v保险丝失效     *//* F3 -24v*/
#define ACT1_PERR	0xC2 //0xBD	/* 机头板1+24v保险丝失效     *//* F4 +24v*/
#ifdef SUPPORT_2S
#define ACT2_PERR	0xBB //0xBE	/* 机头板2-24v保险丝失效     *//* F5 -24V*/
#define XZQ2_PERR	0xBC	/* 机头板2+24v保险丝失效     *//* F6 +24v*/
#define STEP_PERR	0xCB //0xBF	/* 步进板24v保险丝失效     *//* F7 24V*/
#else
#define STEP_PERR	0xBC //0xBF	/* 单系统 马达24V F6 24V*/
#endif
#else
#define XZQ1_PERR	0xBB	/* 机头板1-24v保险丝失效     *//* F3 -24v*/
#define XZQ2_PERR	0xBC	/* 机头板2+24v保险丝失效     *//* F6 +24v*/
#define ACT1_PERR	0xBD	/* 机头板1+24v保险丝失效     *//* F4 +24v*/
#define ACT2_PERR	0xBE	/* 机头板2-24v保险丝失效     *//* F5 -24V*/
#define STEP_PERR	0xBF	/* 步进板24v保险丝失效     *//* F7 24V*/
#define YARN_PERR	0xC2	/* 主板24v保险丝失效     *//* F1 -24V*/
#define V12_PERR  	0xCA	/* 主板12v保险丝失效     *//* F2 +12V*/
#endif

#endif


#define DSP_FATAL_ERR	0xC0	/* MCU失效 */


#define ACT_STEP_FAULT1	0xA4  /* 动作马达1故障 */
#define ACT_STEP_FAULT2	0xA5  /* 动作马达2故障 */
#define ACT_STEP_FAULT3	0xA6  /* 动作马达3故障 */
#define ACT_STEP_FAULT4	0xA7  /* 动作马达4故障 */


#define YARN_STEP1_ERR	(0xD5)   /*沙嘴马达1故障*/
#define YARN_STEP2_ERR	(0xD6)   /*沙嘴马达2故障*/
#define YARN_STEP3_ERR	(0xD7)   /*沙嘴马达3故障*/
#define YARN_STEP4_ERR	(0xD8)   /*沙嘴马达4故障*/

#define YARN_STEP5_ERR	(0xD9)   /*沙嘴马达5故障*/
#define YARN_STEP6_ERR	(0xDA)   /*沙嘴马达6故障*/
#define YARN_STEP7_ERR	(0xDB)   /*沙嘴马达7故障*/
#define YARN_STEP8_ERR	(0xDC)   /*沙嘴马达8故障*/

#define YARN_DC24_LOW	(0xDE)  /*电机24伏电源欠压*/		
#define YARN_DC24_HIGH	(0xDF)  /*电机24伏电源过压*/
#define YARN_DC12_LOW	(0xE0)  /*传感器12伏电源欠压*/
#define YARN_DC12_HIGH	(0xE1)  /*传感器12伏电源过压*/



#if 0
#define RIGHT_TANZHEN_ERR	0xC9	/* 右探针 */
#define LEFT_RIGHT_TANZHEN_ERR 	(0xCA)    /*左右探针*/
#define LEFT_TANZHEN_ERR	0xCB	/* 左探针 */



//#define STEP2_PERR	0xCB	/* 步进板24v保险丝失效     *//* F7 +24V*/

#define OVERLOAD_HEAD	0xFD /* 机头板过流报警 */
#define OVERLOAD_YARN	0xA0 /* 纱嘴电磁铁过流
			      * 参数 对应纱嘴号
			      */
#define OVERLOAD_JQD	0xA1 /* 选针器过流
			      * 参数
			      *   高八位为选针器号
			      *   低八位为刀并头号
			      */
#define OVERLOAD_ACT	0xA2 /* 动作电磁铁过流
			      * 参数
			      *   高八位为组号
			      *   低八位为电磁铁号
			      */
#define YARN_ACT_FAULT        0xA3 /*纱嘴电磁铁故障*/



#define	DIR_ERR			0xD0	/* 越发，新的换向检测方式，*/
#define	SINKER_DONE_ERR	0xD2	/* 越发，生克动作未完成 */
#define	STI_DONE_ERR	0xD3	/* 越发，度目动作未完成 */

#define DEVICE_CONFIG_NUM_ERROR	0xD4	/* 电机配置 命令数 出错 */

#define ENCODE_ERROR1	0xe1 /* 1号电机 编码器错 */
#define ENCODE_ERROR2	0xe2 /* 2号电机 编码器错 */
#define ENCODE_ERROR3	0xe3 /* 3号电机 编码器错 */
#define ENCODE_ERROR4	0xe4 /* 4号电机 编码器错 */
#define ENCODE_ERROR5	0xe5 /* 5号电机 编码器错 */
#define ENCODE_ERROR6	0xe6 /* 6号电机 编码器错 */
#define ENCODE_ERROR7	0xe7 /* 7号电机 编码器错 */
#define ENCODE_ERROR8	0xe8 /* 8号电机 编码器错 */
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

