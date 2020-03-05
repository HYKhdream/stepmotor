#include "guilib.h"
#include "arch_ra8875.h"
#include "arch_board.h"

int pageid;
//can数据结构体
extern struct{
	uint32_t msg_id;
	int send_num;
	int delay_ms;
	int zhen_delay_ms;
	unsigned char msg_data[8];
}CAN_CONT_SEND_DEF;


int system_event_push(int event, int status);
void arch_touch_detect(void);


/*
**********************************
*	绘制按键函数
* color:按键填充颜色
**********************************
*/
void GUI_Botton(int x1,int y1,int w,int h,int color)
{
	int x2,y2;
	x2 = x1 + w;
	y2 = y1 + h;
	RA8875_rounded_rectangle(x1,y1,x2,y2,color,10,10,1);
	RA8875_rounded_rectangle(x1,y1,x2,y2,VM_COLOR_DEEPGRAY,10,10,0);
	
}


/*显示中文*/
void GUI_set_CH(int x,int y,char **buf,int color)
{
	int i,j,k;
	for(i=0;i<2;i++)
	{
		for(j=0;j<16;j++)
		{
			for(k=0;k<8;k++)
			{
				if((buf[i][j]>>k)&0x01)
					RA8875_PutPixel(j+x,i+y,color);
				else
					RA8875_PutPixel(j+x,i+y,~color);
			}
		}
	}
}

/*
***************************************
* 数据显示框
***************************************
*/
void GUI_EDIT(int x,int y,int w,int h,int color)
{
	RA8875_DrawRect(x,y,h,w,VM_COLOR_LIGHTGRAY,0x01);
	RA8875_DrawRect(x+1,y+1,h-2,w-2,VM_COLOR_BLACK,0x01);
	RA8875_PartClrScr(x+2,y+2,w-4,h-4,color);
}

/* 
*******************************************
*	进度条显示
*******************************************
*/
void GUI_Progress_Bar(int x,int y,int w,int h,int color,int num,int max_num)
{
	if((num*100/max_num)==0){
		GUI_Local_color(x+2,y+2,w-4,h-4,VM_COLOR_WHITE); //清空进度条
		GUI_DrawRect(x,y,w,h,VM_COLOR_LIGHTGRAY,0x01);
		GUI_DrawRect(x+1,y+1,w-2,h-2,VM_COLOR_BLACK,0x01);
		GUI_DrawInt(x+w+5,y+5,0,VM_COLOR_BLACK,VM_COLOR_GRAY,3);
		GUI_DrawAscii(x+w+35,y+5,"%",VM_COLOR_BLACK,VM_COLOR_GRAY);
	}
	else{
	GUI_DrawRect(x,y,w,h,VM_COLOR_LIGHTGRAY,0x01);
	GUI_DrawRect(x+1,y+1,w-2,h-2,VM_COLOR_BLACK,0x01);
	GUI_DrawInt(x+w+5,y+5,num*100/max_num,VM_COLOR_BLACK,VM_COLOR_GRAY,3);
	GUI_DrawAscii(x+w+35,y+5,"%",VM_COLOR_BLACK,VM_COLOR_GRAY);
	RA8875_PartClrScr(x+2,y+2,(int)(w-4)*num/max_num,h-4,color);
	}
}



void GUI_DrawLine(int x1, int y1, int x2, int y2, int color)
{
	RA8875_DrawLine(x1, y1, x2, y2, color, 0x0);
}

void GUI_Local_color(int x1,int y1,int w1,int h1,int _usColor)
{
	RA8875_PartClrScr(x1,y1,w1,h1,_usColor);
	
}



void GUI_Can_SetClr(int x,int y,int h,int w,int color)
{
	BTE_SetTarBlock(x, y, h, w, 0);	/* 设置BTE位置和宽度高度以及目标图层（0或1） */
	BTE_SetOperateCode(0x0C);		/* 设定BTE 操作码和光栅运算码  REG[51h] Bit[3:0] = 0Ch */
	RA8875_SetFrontColor(color);	/* 设置BTE前景色 */
	BTE_Start();					/* 开启BTE 功能 */
	BTE_Wait();						/* 等待操作结束 */
}

void GUI_Clr(int color)
{
	RA8875_ClrScr(color);
}

void GUI_DrawRect(int x, int y, int w, int h, int color, int mode)
{
	RA8875_DrawRect(x, y, h, w, color, mode);
}

void GUI_DrawAscii(int x, int y, char *str, int color, int bcolor)
{
	RA8875_SetFrontColor(color);
	RA8875_SetBackColor(bcolor);
	RA8875_DispAscii(x, y, str);
}

void GUI_DrawInt(int x, int y, int dat, int color, int bcolor, int bmax)
{
	char str[16];
	sprintf(str, "%-*d", bmax, dat);
	GUI_DrawAscii(x, y, str, color, bcolor);
}

void GUI_DrawHex(int x, int y, int dat, int color, int bcolor, int bmax)
{
	char str[16];
	sprintf(str, "%-*X", bmax, dat);
	GUI_DrawAscii(x, y, str, color, bcolor);
}

typedef struct {
	int	hs_blkcnt;
	___HOTAREA_DEF	*hs_blks;
}___HOTAREA_SET_DEF;

#define HS_MAX_LEVELS	10
static ___HOTAREA_SET_DEF _hs_buf[HS_MAX_LEVELS];
static int hs_top = 0;

int touchctl_HotArea_Register(___HOTAREA_DEF *ha, int ha_cnt)
{
	_hs_buf[hs_top].hs_blks = ha;
	_hs_buf[hs_top].hs_blkcnt = ha_cnt;

	if(hs_top + 1 >= HS_MAX_LEVELS) {
		return -1;
	}

	hs_top++;
	return 0;
}

int touchctl_HotArea_Dregister(___HOTAREA_DEF *ha)
{
	int ofs = hs_top - 1;

	if(_hs_buf[ofs].hs_blks != ha) {
		return -1;
	}

	hs_top--;
	return 0;
}

___HOTAREA_SET_DEF *touchctl_HotArea_Peek()
{
	if(hs_top <= 0)
		return NULL;

	return &_hs_buf[hs_top - 1];
}

int touchctl_HotArea_Machit(int x, int y, int event)
{
	___HOTAREA_SET_DEF *hs;
	int i;

	hs = touchctl_HotArea_Peek();
	if(hs == NULL) {
		return -1;
	}

	for(i = 0; i < hs->hs_blkcnt; i++) {
		if(event ? (hs->hs_blks[i].ha_flag & 0x4000) : !(hs->hs_blks[i].ha_flag & 0x4000)) {
			continue;
		}

		if((x >= hs->hs_blks[i].ha_x) &&
		   (x < hs->hs_blks[i].ha_x + hs->hs_blks[i].ha_w) &&
		   (y >= hs->hs_blks[i].ha_y) &&
		   (y < hs->hs_blks[i].ha_y + hs->hs_blks[i].ha_h)) {
			// push event
			system_event_push(hs->hs_blks[i].ha_event, event);
			break;
		}
	}

	return 0;
}

typedef struct {
	int	event;
	int	status;
}__EVT_DEF_;

#define MAX_EVT_LEVELS	10
static __EVT_DEF_ _Event_ff[MAX_EVT_LEVELS];
static int _Event_wptr = 0;
static int _Event_rptr = 0;

int system_event_count()
{
	int count = MAX_EVT_LEVELS + _Event_wptr - _Event_rptr;
	if(count >= MAX_EVT_LEVELS)
		count -= MAX_EVT_LEVELS;

	return count;
}

int system_event_push(int event, int status)
{
	int wptr = _Event_wptr;

	if(system_event_count() >= MAX_EVT_LEVELS - 1)
		return -1;

	_Event_ff[wptr].event = event;
	_Event_ff[wptr].status = status;
	if(++wptr >= MAX_EVT_LEVELS)
		wptr -= MAX_EVT_LEVELS;
	_Event_wptr = wptr;

	return 0;
}

int system_event_pop(int *event, int *status)
{
	int rptr = _Event_rptr;

	arch_touch_detect();
	qwl_shell_poll();

	if(system_event_count() <= 0)
		return -1;

	if(event)
		*event = _Event_ff[rptr].event;
	if(status)
		*status = _Event_ff[rptr].status;

	if(++rptr >= MAX_EVT_LEVELS)
		rptr -= MAX_EVT_LEVELS;
	_Event_rptr = rptr;

	return 1;
}

