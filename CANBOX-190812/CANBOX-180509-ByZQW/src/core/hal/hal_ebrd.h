/**
  *****************************************************************************
  * @file    hal_ebrd.h
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2016-08-18
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/

#ifndef __HAL_EBRD_H__
#define __HAL_EBRD_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include "hal_cmd.h"

/* Public typedef ------------------------------------------------------------*/

typedef struct {
	char name[16];
	uint8_t sysno;	//ϵͳ��
	uint8_t ver;

	uint8_t online;
	
	uint8_t power;	//��Դ״̬
	uint8_t son;	//���ʹ��
	
	uint8_t xzqs;	//ѡ��������
	uint8_t emfs;	//���������
	uint8_t mots;	//�������
	uint8_t encs;	//����������
	uint8_t sins;	//�������

	uint16_t mot_ma;

	uint8_t xzq_mode;
	uint8_t xzq_t_idx;
	uint8_t xzq_t_dao;

	uint8_t emf_mode;
	uint8_t emf_t_idx;
	
	uint8_t mot_mode;
	uint8_t mot_t_idx;

	uint8_t xzq_spd;
	uint8_t emf_spd;
	uint8_t mot_spd;
	
	uint16_t xzq_tick;
	uint16_t emf_tick;
	uint16_t mot_tick;

	uint16_t cancomid;	//����ID
	uint16_t cantxid;
	uint16_t canrxid;
	uint32_t Version_32;
}EBRD_INFO_t;


/* Public define -------------------------------------------------------------*/

enum {
	EBRD_E534 = 1,
	EBRD_E459,
	EBRD_E481,
	EBRD_E644,
	EBRD_E697,
	
};


#define BRD_E534_CANDID			0x0320
#define BRD_E534_CANID2			0x0360

#define BRD_E459_CANDID			0x0330

#define BRD_EBRD_CANID			0x0361		//�㲥ID

#define BRD_E644_CANID			0x0235

#define BRD_E644_CANID_RX		0x0535	/*E644�ķ���ID��*/

#define BRD_E697_CANID			0x0261

#define BRD_E697_CANID_RX		0x0561	/*E644�ķ���ID��*/





/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __HAL_EBRD_H__ */

