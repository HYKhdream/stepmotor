/**
  *****************************************************************************
  * @file    gui_ebrd.h
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2016-08-18
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/

#ifndef __GUI_EBRD_H__
#define __GUI_EBRD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "guilib.h"

/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/

#define MSG_LIST_NUM					8
#define MSG_LIST_X						80
#define MSG_LIST_Y						30
#define MSG_LIST_W						340
#define MSG_LIST_H						30

#define MSG_LIST_IDX_X					MSG_LIST_X
#define MSG_LIST_IDX_Y					MSG_LIST_Y
#define MSG_LIST_IDX_W					60
#define MSG_LIST_IDX_H					MSG_LIST_H

#define MSG_LIST_X_MAX					(MSG_LIST_X + MSG_LIST_W)
#define MSG_LIST_Y_MAX					(MSG_LIST_Y + MSG_LIST_H * MSG_LIST_NUM)

#define BTN_PGUP_X						MSG_LIST_X_MAX
#define BTN_PGUP_Y						MSG_LIST_Y
#define BTN_PGUP_W						(LCD_WIDTH - BTN_PGUP_X)
#define BTN_PGUP_H						(MSG_LIST_H * MSG_LIST_NUM >> 1)

#define BTN_PGDN_X						BTN_PGUP_X
#define BTN_PGDN_Y						(BTN_PGUP_Y+BTN_PGUP_H+2)
#define BTN_PGDN_W						BTN_PGUP_W
#define BTN_PGDN_H						BTN_PGUP_H

/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/

extern void gui_ebrd_process(void);

extern void gui_ebrd_reg(char *title, void (*pfn)(void));

#ifdef __cplusplus
}
#endif

#endif /* __GUI_EBRD_H__ */

