/**
  *****************************************************************************
  * @file    guilib.h
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2016-08-17
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/

#ifndef __GUILIB_H__
#define __GUILIB_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Public typedef ------------------------------------------------------------*/

typedef struct {
	uint16_t ha_x;
	uint16_t ha_y;
	uint16_t ha_w;
	uint16_t ha_h;
	uint16_t ha_flag;
	uint16_t ha_event;
}___HOTAREA_DEF,tBtnWidget;


extern int a;
extern int b;
/*
typedef struct {
	___HOTAREA_DEF btnHotArea;
	const char *btnText;
	int textColor;
	int textBgColor;
	void (*pfnOnClick)(void *param);
} tBtnWidget;
*/

/* Public define -------------------------------------------------------------*/

#define LCD_WIDTH		480	// X
#define LCD_HEIGH		272	// Y



enum {
	PAGE_ID_MENU = 0,
	PAGE_ID_CANMONITOR,
	PAGE_ID_FILTERID,
	PAGE_ID_FILTERDAT,
	PAGE_ID_CANSEND,
	PAGE_ID_TEST_CONT_CANSEND,
	PAGE_ID_TEST_CONT_CANSEND2,
	PAGE_ID_TEST_CONT_CANSEND3,
	PAGE_ID_TEST_CONT_CANSEND4,
	
	PAGE_ID_E534,
	PAGE_ID_E481,
	PAGE_ID_E438,

	PAGE_ID_TEST_MENU,
	PAGE_ID_TEST_XZQ,
	PAGE_ID_TEST_MOT,
	PAGE_ID_TEST_EMF,
	PAGE_ID_TEST_YARN,
	PAGE_ID_TEST_IN,
	PAGE_ID_TEST_E644,
	PAGE_ID_TEST_E697,
	PAGE_ID_TEST_E403,
	PAGE_ID_TEST_WEIJI,
	PAGE_ID_MOTO_SET,
	PAGE_ID_STEPMOTOR_TEST,
};





/* Public macro --------------------------------------------------------------*/

#define ButtonWidget(x, y, width, height, btnflag, btnidx)	\
					{					\
						x,				\
						y,				\
						width, 			\
						height, 		\
						btnflag, 		\
						btnidx			\
					}


#define VM_COLOR_RGB(r, g, b)	((int)(((r >> 3) << 11) | (g >> 2) << 5 | (b >> 3)))
#define VM_COLOR_WHITE	VM_COLOR_RGB(0xFF, 0xFF, 0xFF)
#define VM_COLOR_BLACK	VM_COLOR_RGB(0x00, 0x00, 0x00)
#define VM_COLOR_RED	VM_COLOR_RGB(0xFF, 0x00, 0x00)
#define VM_COLOR_GREEN	VM_COLOR_RGB(0x00, 0x64, 0x00)
#define VM_COLOR_BLUE	VM_COLOR_RGB(0x00, 0x00, 0xFF)
#define VM_COLOR_BG		VM_COLOR_RGB(0xFF, 0xFF, 0xFF)
#define VM_COLOR_GRY	VM_COLOR_RGB(0xC0, 0xC0, 0xC0)
#define VM_COLOR_YELLOW 	VM_COLOR_RGB(0xff, 0xff, 0x00)
#define VM_COLOR_GRAY			VM_COLOR_RGB(0xdb,0xdb,0xdb)
#define VM_COLOR_DEEPGRAY VM_COLOR_RGB(0x55,0x55,0x55)
#define VM_COLOR_LIGHTGRAY VM_COLOR_RGB(0xa8,0xa8,0xa8)


#define COUNTOF(__t)	(sizeof(__t) / sizeof(__t[0]))

/* Public variables ----------------------------------------------------------*/

extern int pageid;

/* Public function prototypes ------------------------------------------------*/


int touchctl_HotArea_Register(___HOTAREA_DEF *ha, int ha_cnt);
int touchctl_HotArea_Dregister(___HOTAREA_DEF *ha);
void GUI_Botton(int x1,int y1,int w,int h,int color);
void GUI_Clr(int color);
void GUI_DrawRect(int x, int y, int w, int h, int color, int mode);
void GUI_DrawLine(int x1, int y1, int x2, int y2, int color);
void GUI_DrawAscii(int x, int y, char *str, int color, int bcolor);
void GUI_DrawInt(int x, int y, int dat, int color, int bcolor, int bmax);
void GUI_DrawHex(int x, int y, int dat, int color, int bcolor, int bmax);
void GUI_Local_color(int x,int y,int w,int h,int _usColor);
int system_event_pop(int *event, int *status);
int GUI_GetInput(int wx, int wy, int *dat, int bmax, int mode);
void GUI_Can_SetClr(int x,int y,int h,int w,int color);
void GUI_EDIT(int x,int y,int w,int h,int color);
void GUI_Progress_Bar(int x,int y,int w,int h,int color,int num,int max_num);
void GUI_set_CH(int x,int y,char **buf,int color);
#ifdef __cplusplus
}
#endif

#endif /* __GUILIB_H__ */




