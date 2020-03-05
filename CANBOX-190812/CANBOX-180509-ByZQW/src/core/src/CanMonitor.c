#include "guilib.h"
#include "canfifo.h"
#include "gui_ebrd.h"
#include "math.h"
#include "arch_board.h"
#include "arch_uart.h"
#include "arch_ra8875.h"
#include "canfifo.h"


#define BASE_BOT
#define HOOK_CHECK
#define BASE_DATA
#define CAN_DATA_INIT

#define MSG_MAGIC		0xF5AF

extern unsigned long CWJ_RECV_ID;
extern int CWJ_RECV_LEN;
extern unsigned char CWJ_RECV_DATA[8];
typedef struct {
	uint16_t hook;
	uint16_t magic;
	uint16_t msg_id;
	uint16_t msg_len;
	unsigned char msg_dat[8];
	uint16_t send_delayms;
	int	send_count;
	int	send_total;
} CAN_SEND_DEF;


int a=0;


/* 电机返回数据 */
typedef struct{
	unsigned int now_step;
	unsigned int all_step;
	unsigned int cwj_data;
	
}STEP_MOTOR_BACK;

STEP_MOTOR_BACK can_back;
/* can数据结构体 */
typedef struct{
	uint16_t hook;
	uint16_t msg_id;
	uint16_t msg_len;
	int send_num;
	unsigned char msg_data[8];
}CAN_CONT_SEND_DEF;


CAN_CONT_SEND_DEF ptr[16];
CAN_SEND_DEF Msg_Send;
unsigned int msg_baud;


extern uint8_t uart_rx_fifo[];

unsigned int Hook[16]={0};
void JM_INIT1(void);
/*----------------------------------------------------------------------------*/

#define FN_BTN_NUM			6
#define FN_BTN_H		(240 / FN_BTN_NUM)

/*----------------------------------------------------------------------------*/

enum {
	BTN_EVT_FRAME_MSG_ID = 0,
	BTN_EVT_FRAME_MSG_DLC,
	BTN_EVT_FRAME_MSG_CNT,
	BTN_EVT_FRAME_MSG_DLY,
	BTN_EVT_FRAME_MSG_BAUD,
	
	BTN_EVT_FRAME_MSG_DAT0,
	BTN_EVT_FRAME_MSG_DAT1,
	BTN_EVT_FRAME_MSG_DAT2,
	BTN_EVT_FRAME_MSG_DAT3,
	BTN_EVT_FRAME_MSG_DAT4,
	BTN_EVT_FRAME_MSG_DAT5,
	BTN_EVT_FRAME_MSG_DAT6,
	BTN_EVT_FRAME_MSG_DAT7,

	BTN_EVT_SEND,
	BTN_EVT_SAVE,
	BTN_EVT_EXIT,
	BTN_EVT_RESET
};

enum {
	FN_BTN_CLEAN = 0,
	FN_BTN_STOP,
	FN_BTN_SEND,
	FN_BTN_PG_SEND,
	FN_BTN_PG_FID,
	FN_BTN_PG_FDAT,
	FN_BTN_PGUP,
	FN_BTN_PGDN,
	FN_BTN_EXIT,
	FN_BTN_RESET
};

int can_bps_save(unsigned int val);

/* 测微机 */
enum {
	CWJ_START_BUTTON = 0,
	CWJ_CLEAR_DATA,
	CWJ_ALL_STEP,
	CWJ_TOTAL_LEN,
	CWJ_STEP_ID,
	CWJ_STEP_ELE,
	CWJ_STEP_X,
	CWJ_STEP_STEP,
	CWJ_STEP_FANGXAING,
	CWJ_STEP_ZHUANGLING,
};
/* 测微计电机设置 */

/* 步进电机 */
enum{
	STEP_MOTOR_START = 0,
	STEP_MOTOR_STOP,
	STEP_MOTOR_FRE,
	STEP_MOTOR_DIR,
};


enum{
	Zhuang_ling = 0,
	DIRECTION,
	BACK,

};

___HOTAREA_DEF HA_monitor_buttons[] = {
	// x    y    w    h   flags    event
	{  80,   1,  80,  30, 0x0000,  FN_BTN_CLEAN},	// Clean 
	{   1,  60,  80,  70, 0x0000,  FN_BTN_STOP},	// Stop /start
	{   1, 130,  80,  70, 0x4000,  FN_BTN_SEND},	// Send
	{  1,  200,  80,  70, 0x0000,  FN_BTN_PG_SEND},	// Enter SendPage
	{  1,  200,  80,  70, 0x0000,  FN_BTN_PG_FID},	// Enter FILTER ID
	{  1,  200,  80,  70, 0x0000,  FN_BTN_PG_FDAT},	// Enter FILTER DATA
	{ BTN_PGUP_X,BTN_PGUP_Y, BTN_PGUP_W, BTN_PGUP_H, 0x0000,  FN_BTN_PGUP},	// pgup
	{ BTN_PGDN_X,BTN_PGDN_Y, BTN_PGDN_W, BTN_PGDN_H, 0x0000,  FN_BTN_PGDN},	// pgdn
	
	{ 420,   5,  60,  30, 0x0000,  FN_BTN_EXIT},	// Exit	
};

int pageid_bkp = PAGE_ID_MENU;

void Graw_Show_Button()
{
	if (CanRecv_status())
	{
		GUI_DrawAscii(HA_monitor_buttons[FN_BTN_STOP].ha_x + 15, HA_monitor_buttons[FN_BTN_STOP].ha_y+(FN_BTN_H-16)/2,
					"Stop ", VM_COLOR_RGB(0xFF, 0x00, 0xFF), VM_COLOR_BG);
	}
	else
	{
		GUI_DrawAscii(HA_monitor_buttons[FN_BTN_STOP].ha_x + 15, HA_monitor_buttons[FN_BTN_STOP].ha_y+(FN_BTN_H-16)/2,
					"Start", VM_COLOR_RGB(0xFF, 0x00, 0x00), VM_COLOR_BG);
	}
}

void Can_send_frame()
{
	arch_can_send(Msg_Send.msg_id, Msg_Send.msg_len, Msg_Send.msg_dat);
	ShowFifo_push(Msg_Send.msg_id, Msg_Send.msg_len, Msg_Send.msg_dat);
}

void Draw_bar(void)
{
	GUI_Clr(VM_COLOR_BG);
	
	GUI_DrawLine(0, MSG_LIST_Y-2, LCD_WIDTH, MSG_LIST_Y-2, VM_COLOR_BLACK);
	GUI_DrawLine(0, MSG_LIST_Y-1, LCD_WIDTH, MSG_LIST_Y-1, VM_COLOR_GRY);
	GUI_DrawLine(0, MSG_LIST_Y-0, LCD_WIDTH, MSG_LIST_Y-0, VM_COLOR_GRY);
}


int can_send_load(CAN_SEND_DEF *pSend)
{
	if (pSend == NULL)
		return -1;
	arch_eeprom_read((void *)pSend, 1000, sizeof(CAN_SEND_DEF));
	if (pSend->magic != MSG_MAGIC)
	{
		memset((void *)pSend, 0, sizeof(CAN_SEND_DEF));
		pSend->msg_id = 0x370;
		pSend->msg_len = 2;
		pSend->msg_dat[0] = 0x34;
		pSend->msg_dat[1] = 0x09;
		return -2;
	}
	return 0;
}


/************************************************************************
页面按键触摸区域定义
*************************************************************************/
___HOTAREA_DEF CWJ_BUTTON[]={
	{20 ,100 ,100,50, 0x4000,  CWJ_START_BUTTON},
	{20 ,20  ,100,50, 0x4000,  CWJ_CLEAR_DATA  },
	{300,180 ,70 ,25, 0x4000,	 CWJ_ALL_STEP    },
	{250,100 ,70 ,25, 0x4000,	 CWJ_TOTAL_LEN   },
	{150,50	 ,70 ,25, 0x4000,	 CWJ_STEP_ID   	 },
	{230,50  ,70 ,25, 0x4000,	 CWJ_STEP_ELE    },
	{310,50  ,70 ,25, 0x4000,	 CWJ_STEP_X   	 },
	{390,50	 ,70 ,25, 0x4000,	 CWJ_STEP_STEP   },
	{400,100 ,50 ,30, 0x4000,	 CWJ_STEP_FANGXAING },
	{400,150 ,50 ,30, 0x4000,	 CWJ_STEP_ZHUANGLING },
};

/*步进电机测试按键触摸区域定义*/
___HOTAREA_DEF STEP_MOTOR_BUTTON[]={
	{20 ,20  ,100,50, 0x4000, STEP_MOTOR_START},
	{20 ,100 ,100,50, 0x4000, STEP_MOTOR_STOP},
	{20 ,180 ,100,50, 0x4000, STEP_MOTOR_DIR},
	{160,100 ,100,25, 0x4000, STEP_MOTOR_FRE},
};


___HOTAREA_DEF MOTO_SET[]={
	{20 ,100 ,100,50, 0x4000,  Zhuang_ling},
	{20 ,200 ,50 ,30, 0x4000,  BACK				},
	{200,100 ,100,50, 0x4000,  DIRECTION},
};

/*
******************************************
* 电机返回数据读取
******************************************
*/
void can_motor_back(void)
{
	can_back.all_step=(CWJ_RECV_DATA[4]<<8)+CWJ_RECV_DATA[5];
	can_back.now_step=(CWJ_RECV_DATA[2]<<8)+CWJ_RECV_DATA[3];
	can_back.cwj_data=(CWJ_RECV_DATA[6]<<8)+CWJ_RECV_DATA[7];
}

void can_receive_clc(void)
{
	int i;
	for(i=0;i<8;i++)
		CWJ_RECV_DATA[i]=0;
	CWJ_RECV_ID=0;
	CWJ_RECV_LEN=0;
}
/******************************************
*	程序功能：can数据发送给电机
*
*	参数：
*				@electricity：电流大小（0~1000）
*   		@command：指令参数
*											0：开始
*											1：结束
*											2：电机按最小刻度运行
*											3：记录当前刻度
*				@multiple：电机最小单位运动的倍数X（0.02°*X）
*				@max_scale：最大刻度
*			
*******************************************/
void CWJ_CAN_Send(int step_motor_ID,int electricity,int command,int multiple,int max_scale)
{
	
	Msg_Send.msg_dat[0]=0x40;
	Msg_Send.msg_dat[1]=step_motor_ID;
	Msg_Send.msg_dat[2]=electricity;   
	Msg_Send.msg_dat[3]=electricity>>8;
	Msg_Send.msg_dat[4]=command;
	Msg_Send.msg_dat[5]=multiple;
	Msg_Send.msg_dat[6]=max_scale;
	Msg_Send.msg_dat[7]=max_scale>>8;
	Can_send_frame();
}
void CWJ_CAN_Send1(int ID)
{
	
	Msg_Send.msg_dat[0]=0x07;
	Msg_Send.msg_dat[1]=ID;
	Msg_Send.msg_dat[2]=0;   
	Msg_Send.msg_dat[3]=0;
	Msg_Send.msg_dat[4]=0;
	Msg_Send.msg_dat[5]=0;
	Msg_Send.msg_dat[6]=0;
	Msg_Send.msg_dat[7]=0;
	Can_send_frame();
}
void CWJ_CAN_Send_DOWN(int ID)
{
	uint32_t rmp=32770;
	Msg_Send.msg_dat[0]=0x02;
	Msg_Send.msg_dat[1]=ID;
	Msg_Send.msg_dat[2]=rmp;   
	Msg_Send.msg_dat[3]=rmp>>8;
	Msg_Send.msg_dat[4]=0;
	Msg_Send.msg_dat[5]=0;
	Msg_Send.msg_dat[6]=1;
	Msg_Send.msg_dat[7]=0;
	Can_send_frame();
}
void CWJ_CAN_Send_UP(int ID)
{
	uint32_t rmp=0x8002;
	Msg_Send.msg_dat[0]=0x02;
	Msg_Send.msg_dat[1]=ID;
	Msg_Send.msg_dat[2]=rmp;   
	Msg_Send.msg_dat[3]=rmp>>8;
	Msg_Send.msg_dat[4]=0;
	Msg_Send.msg_dat[5]=0;
	Msg_Send.msg_dat[6]=0;
	Msg_Send.msg_dat[7]=0;
	Can_send_frame();
}

void CWJ_CAN_Send2(int ID,int speed)
{
	int torque=1000;
	Msg_Send.msg_dat[0]=0x01;
	Msg_Send.msg_dat[1]=ID;
	Msg_Send.msg_dat[2]=speed;   
	Msg_Send.msg_dat[3]=speed>>8;
	Msg_Send.msg_dat[4]=torque;
	Msg_Send.msg_dat[5]=torque>>8;
	Msg_Send.msg_dat[6]=0;
	Msg_Send.msg_dat[7]=0;
	Can_send_frame();
}

void CWJ_CAN_Send_CMD(int cmd,int ID,int data1,int data2,int data3)
{
	Msg_Send.msg_dat[0]=cmd;
	Msg_Send.msg_dat[1]=ID;
	Msg_Send.msg_dat[2]=data1;   
	Msg_Send.msg_dat[3]=data1>>8;
	Msg_Send.msg_dat[4]=data2;
	Msg_Send.msg_dat[5]=data2>>8;
	Msg_Send.msg_dat[6]=data3;
	Msg_Send.msg_dat[7]=data3>>8;
	Can_send_frame();
}

/*step motor 频率测试程序*/
void step_motor_fre_test()
{
	int ret,evt,sts,dat=0,motor_fre=500,dir=1,start_or_stop=0x03;
	
	top:
	/* 按键定义区 */
	RA8875_SetTextZoom(0,0);
	GUI_Clr(VM_COLOR_GRAY);
	//start按键
	GUI_Botton(20,20,100,50,VM_COLOR_LIGHTGRAY);
	GUI_DrawAscii(45,35,"start",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	//stop按键
	GUI_Botton(20,100,100,50,VM_COLOR_LIGHTGRAY);
	GUI_DrawAscii(45,115,"stop",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	//频率输入区
	GUI_EDIT(160,100,100,25,VM_COLOR_WHITE);
	GUI_DrawAscii(165,75,"step_motor_fre",VM_COLOR_BLACK,VM_COLOR_GRAY);
	GUI_DrawInt(165,105,motor_fre,VM_COLOR_BLACK,VM_COLOR_WHITE,6);
	//转动方向
	GUI_Botton(20,180,100,50,VM_COLOR_LIGHTGRAY);
	GUI_DrawAscii(45,195,"DIR",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	
	Msg_Send.msg_id=0x370;
	Msg_Send.msg_len=8;
	can_bps_save(1000);
	if(dir){
		GUI_Botton(20,180,100,50,VM_COLOR_BLUE);
		GUI_DrawAscii(45,195,"DIR",VM_COLOR_BLACK,VM_COLOR_BLUE);
	}else{
		GUI_Botton(20,180,100,50,VM_COLOR_LIGHTGRAY);
		GUI_DrawAscii(45,195,"DIR",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	}
	touchctl_HotArea_Register((___HOTAREA_DEF *)STEP_MOTOR_BUTTON, COUNTOF(STEP_MOTOR_BUTTON));
	while(pageid == PAGE_ID_STEPMOTOR_TEST)
	{
		ret = system_event_pop(&evt, &sts);
		if (ret <= 0)
			continue;
		
		switch(evt)
		{
			case STEP_MOTOR_START:
			{
				start_or_stop=0xfd;
				GUI_Botton(20,20,100,50,VM_COLOR_RED);
				GUI_DrawAscii(45,35,"start",VM_COLOR_BLACK,VM_COLOR_RED);
				CWJ_CAN_Send_CMD(start_or_stop,0x1b,motor_fre,dir,0);
				arch_delay_ms(50);
				GUI_Botton(20,20,100,50,VM_COLOR_LIGHTGRAY);
				GUI_DrawAscii(45,35,"start",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
			}break;
			case STEP_MOTOR_STOP:
			{
				start_or_stop=0x03;
				GUI_Botton(20,100,100,50,VM_COLOR_RED);
				GUI_DrawAscii(45,115,"stop",VM_COLOR_BLACK,VM_COLOR_RED);
				CWJ_CAN_Send_CMD(start_or_stop,0,0,0,0);
				arch_delay_ms(50);
				GUI_Botton(20,100,100,50,VM_COLOR_LIGHTGRAY);
				GUI_DrawAscii(45,115,"stop",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
			}break;
			
			case STEP_MOTOR_DIR:
			{
				dir=!dir;
				if(dir){
					GUI_Botton(20,180,100,50,VM_COLOR_BLUE);
					GUI_DrawAscii(45,195,"DIR",VM_COLOR_BLACK,VM_COLOR_BLUE);
				}else{
					GUI_Botton(20,180,100,50,VM_COLOR_LIGHTGRAY);
					GUI_DrawAscii(45,195,"DIR",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
				}
				CWJ_CAN_Send_CMD(start_or_stop,0x1b,motor_fre,dir,0);
			}break;
			
			case STEP_MOTOR_FRE:
			{
				GUI_GetInput(250, 20, &dat, 8, 1);
				motor_fre=dat;
				GUI_DrawInt(165,105,motor_fre,VM_COLOR_BLACK,VM_COLOR_WHITE,6);
				goto top;
			}break;
		}
		
	}
	touchctl_HotArea_Dregister((___HOTAREA_DEF *)STEP_MOTOR_BUTTON);
}


/* 测微计程序 */

int step_ID=107;
uint16_t step_let[1000]={0};
void CWJ_Test(void)
{
	uint16_t top_place[2]={0};
	int j=0,k=0,l=0;
	char step_fangxiang=0;
	int Msg_Top = -1;
	int Msg_Cnt = -1;
	unsigned long msgID=0x05;
	unsigned char dta[8];
	int CWJ_ret,ret,evt, sts,i,full_step=0 ,dat=0;
	char str[1]={'%'},str1[]={"start"},str2[]={"clear"};
	uint16_t 	send_data[8]={0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0x0B};
	uint16_t 	reset_data[8]={0x01,0x06,0x08,0x00,0xAB,0x56,0x74,0xA4};
	uint8_t receive_data[9];
	int clc_val=0;
	uint32_t now_step=0,total_len=2000;
	uint32_t step_x=10,step_num=100,step_back=0; /*step_ele:电流大小，step_x：角度倍数，step_num：步数*/
	int step_ele=1000;
began:
	RA8875_SetTextZoom(0,0);
	GUI_Clr(VM_COLOR_GRAY);
	/************************* 界面定义 *******************************/
	/* 开始按键 */
	GUI_Botton(20,100,100,50,VM_COLOR_LIGHTGRAY);
	GUI_DrawAscii(45,115,str1,VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	
	/* 电机方向控制按键 */
	GUI_Botton(400,100,50,30,VM_COLOR_LIGHTGRAY);
	//GUI_DrawAscii(405,105,"CW",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	GUI_Botton(400,150,50,30,VM_COLOR_LIGHTGRAY);
	GUI_DrawAscii(410,155,"M_S",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	/* 清零按键 */
	GUI_Botton(20,20,100,50,VM_COLOR_LIGHTGRAY);
	GUI_DrawAscii(45,35,str2,VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	
	/* 发送接收读取框 */
	
	GUI_EDIT(160,100,70,25,VM_COLOR_WHITE);
	GUI_DrawAscii(165,75,"step_num",VM_COLOR_BLACK,VM_COLOR_GRAY);
	GUI_DrawInt(165,105,now_step,VM_COLOR_BLACK,VM_COLOR_WHITE,5);
	
	GUI_EDIT(250,100,70,25,VM_COLOR_WHITE);
	GUI_DrawAscii(270,75,"len",VM_COLOR_BLACK,VM_COLOR_GRAY);
	GUI_DrawAscii(325,105,"um",VM_COLOR_BLACK,VM_COLOR_GRAY);
	GUI_DrawInt(255,105,total_len,VM_COLOR_BLACK,VM_COLOR_WHITE,5);
	
	/* 电机参数设置 */
	GUI_DrawRect(145,5,320,73,VM_COLOR_BLACK,0);
	GUI_DrawAscii(200,8,"Motor parameter setting",VM_COLOR_RED,VM_COLOR_GRAY);
	
	GUI_EDIT(150,50,70,25,VM_COLOR_WHITE);  
	GUI_DrawAscii(155,25,"STEP_ID",VM_COLOR_BLACK,VM_COLOR_GRAY);
	GUI_DrawInt(175,55,step_ID,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
	
	GUI_EDIT(230,50,70,25,VM_COLOR_WHITE);  
	GUI_DrawAscii(235,25,"step_ele",VM_COLOR_BLACK,VM_COLOR_GRAY);
	GUI_DrawInt(235,55,step_ele,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
	
	GUI_EDIT(310,50,70,25,VM_COLOR_WHITE);  
	GUI_DrawAscii(315,25,"step_x",VM_COLOR_BLACK,VM_COLOR_GRAY);
	GUI_DrawInt(315,55,step_x,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
	
	step_num=total_len/10;
	GUI_EDIT(390,50,70,25,VM_COLOR_WHITE);  
	GUI_DrawAscii(395,25,"set_step",VM_COLOR_BLACK,VM_COLOR_GRAY);
	GUI_DrawInt(395,55,step_num,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
	
	/* 进度条 */
	GUI_DrawAscii(0,220,"Progress Bar",VM_COLOR_BLACK,VM_COLOR_GRAY);
	GUI_Progress_Bar(100,220,300,25,VM_COLOR_GREEN,0,full_step);
	
	/* 电机返回参数 */
	GUI_EDIT(20,180,70,25,VM_COLOR_WHITE);  
	GUI_DrawAscii(25,160,"step_back",VM_COLOR_BLACK,VM_COLOR_GRAY);
	GUI_DrawInt(25,185,step_back,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
	
	/* 传感器数据转换读取框 */
	GUI_EDIT(300,140,70,25,VM_COLOR_WHITE);
	GUI_DrawAscii(170,145,"Micrometer Data:",VM_COLOR_BLACK,VM_COLOR_GRAY);
	GUI_DrawInt(305,145,clc_val,VM_COLOR_BLACK,VM_COLOR_WHITE,5);
	GUI_DrawAscii(375,145,"um",VM_COLOR_BLACK,VM_COLOR_GRAY);
	
	/* 发送次数 */
	GUI_EDIT(300,180,70,25,VM_COLOR_WHITE);
	GUI_DrawAscii(90,185," Motor transmission times:",VM_COLOR_BLACK,VM_COLOR_GRAY); 
	GUI_DrawInt(305,185,full_step,VM_COLOR_BLACK,VM_COLOR_WHITE,5);
	/*******************************************************************/
	
	/* CAN数据初始化 */
	Msg_Send.msg_id=0x01;
	Msg_Send.msg_len=8;
	can_bps_save(1000);
	
	if(step_fangxiang>0)
		GUI_DrawAscii(410,105,"DOWN",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	else
		GUI_DrawAscii(405,105,"UP",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	
	/* 传感器计数清零 */
	for(i=0;i<8;i++){
		arch_uart_putc(reset_data[i]);
	}
	arch_delay_ms(10);
	
	for(i=0;i<step_num;i++){
		step_let[i]=10*(i+1);
	}
	
	touchctl_HotArea_Register((___HOTAREA_DEF *)CWJ_BUTTON, COUNTOF(CWJ_BUTTON));
	while(pageid == PAGE_ID_TEST_WEIJI)
	{
		ret = system_event_pop(&evt, &sts);
		if (ret <= 0)
			continue;
		
		switch(evt)
		{
			
			case CWJ_START_BUTTON:   //开始测试
			{
				now_step=0;
				full_step=0;
				GUI_DrawInt(305,185,full_step,VM_COLOR_BLACK,VM_COLOR_WHITE,5);
				GUI_Botton(20,100,100,50,VM_COLOR_RED);
				GUI_DrawAscii(45,115,str1,VM_COLOR_BLACK,VM_COLOR_RED);
				/*在开始前重置测微计*/
				for(i=0;i<8;i++){
					arch_uart_putc(reset_data[i]);
				}
				arch_delay_ms(10);
				for(i=0;i<9;i++){
					receive_data[i]=0;
					arch_uart_getc(&receive_data[i]);
				}
				/**/
				while(total_len==0)  //错误检测
				{
					RA8875_SetTextZoom(1,1);
					
					GUI_DrawAscii(40,90,"ERROR:full_step is empty!",VM_COLOR_RED,VM_COLOR_BLUE);
					GUI_DrawAscii(90,120,"Please restart and ",VM_COLOR_RED,VM_COLOR_BLUE);
					GUI_DrawAscii(120,150,"set full_step!",VM_COLOR_RED,VM_COLOR_BLUE);
					arch_delay_ms(2000);
					goto began;
				}
				
				CWJ_CAN_Send(step_ID,500,0,step_x,2);	
				arch_delay_ms(5);
				for(l=0;l<step_num;l++){
				while(clc_val<step_let[l]){
					CWJ_CAN_Send(step_ID,step_ele,2,step_x,step_num);
					arch_delay_ms(5);
					now_step++;
					step_back=CWJ_RECV_DATA[2];
					GUI_DrawInt(25,185,step_back,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
					
					GUI_DrawInt(165,105,now_step,VM_COLOR_BLACK,VM_COLOR_WHITE,5);
					/* 传感器数据读取 */
					/************************************************************/
					for(i=0;i<8;i++){
						arch_uart_putc(send_data[i]);
					}
					arch_delay_ms(5);
					for(i=0;i<9;i++){
						receive_data[i]=0;
						arch_uart_getc(&receive_data[i]);
					}
					/************************************************************/
					arch_delay_ms(10);
					clc_val=(receive_data[5]<<8)+receive_data[6];
					
					/*撞零检测*/
					
					
					k++;
					if(k>10){
						top_place[j]=clc_val;
						j++;
						if(j>1){
							j=0;
							if(top_place[0]==top_place[1]){
								RA8875_SetTextZoom(1,1);
								GUI_DrawAscii(40,90,"ERROR:We are at zero ",VM_COLOR_RED,VM_COLOR_BLUE);
								GUI_DrawAscii(90,120,"or Motor is stuck!",VM_COLOR_RED,VM_COLOR_BLUE);
								CWJ_CAN_Send(step_ID,step_ele,1,step_x,step_num);
								arch_delay_ms(2000);
								goto began;
							}
						}
						k=0;
					}
					
					if(receive_data[3]==1)
						GUI_DrawInt(305,145,-clc_val,VM_COLOR_BLACK,VM_COLOR_WHITE,5);
					else
						GUI_DrawInt(305,145,clc_val,VM_COLOR_BLACK,VM_COLOR_WHITE,5);
					/* 进度条 */
					if(clc_val<total_len){
						GUI_Progress_Bar(100,220,300,25,VM_COLOR_GREEN,clc_val,total_len);
						
					}
					
					else{
						GUI_Progress_Bar(100,220,300,25,VM_COLOR_GREEN,total_len,total_len);
					}
					
				}
				full_step++;
				CWJ_CAN_Send(step_ID,step_ele,3,step_x,step_num);
				GUI_DrawInt(305,185,full_step,VM_COLOR_BLACK,VM_COLOR_WHITE,5);
			}
				CWJ_CAN_Send(step_ID,step_ele,1,step_x,step_num);
				GUI_DrawInt(25,185,0,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
				step_back=CWJ_RECV_DATA[2];
				GUI_DrawInt(25,185,step_back,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
				clc_val=0;
				can_receive_clc();
				GUI_Botton(20,100,100,50,VM_COLOR_LIGHTGRAY);
				GUI_DrawAscii(45,115,str1,VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
				CWJ_ret=0;
			}break;
			
			/* 清零按键 */
			case CWJ_CLEAR_DATA:
			{
				full_step=0;
				GUI_Botton(20,20,100,50,VM_COLOR_RED);
				GUI_DrawAscii(45,35,str2,VM_COLOR_BLACK,VM_COLOR_RED);
				CWJ_CAN_Send1(step_ID);
				for(i=0;i<8;i++){
					arch_uart_putc(reset_data[i]);
				}
				arch_delay_ms(10);
				for(i=0;i<9;i++){
					receive_data[i]=0;
					arch_uart_getc(&receive_data[i]);
				}
				clc_val=0;
				GUI_DrawInt(255,105,total_len,VM_COLOR_BLACK,VM_COLOR_WHITE,5);   //测微计长度清零
				GUI_Progress_Bar(100,220,300,25,VM_COLOR_GREEN,clc_val,10);
				arch_delay_ms(500);
				GUI_Botton(20,20,100,50,VM_COLOR_LIGHTGRAY);
				GUI_DrawAscii(45,35,"clear",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
				GUI_DrawInt(305,185,full_step,VM_COLOR_BLACK,VM_COLOR_WHITE,5);
			}
			break;
			
			case CWJ_TOTAL_LEN:
			{
				GUI_GetInput(10, 10, &dat, 8, 1);
				total_len=dat;
				
				GUI_DrawInt(325,55,total_len,VM_COLOR_BLACK,VM_COLOR_WHITE,5);
				goto began;
			}break;
			
			case CWJ_STEP_ID:
			{
				GUI_GetInput(10, 10, &dat, 8, 1);
				step_ID=dat;
				
				GUI_DrawInt(180,55,step_ID,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
				goto began;
			}break;
			
			case CWJ_STEP_ELE:
			{
				GUI_GetInput(10, 10, &dat, 8, 1);
				step_ele=dat;
				if(step_fangxiang<0)
					step_ele=0-step_ele;
				GUI_DrawInt(235,55,step_ele,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
				goto began;
			}break;
			
			case CWJ_STEP_X:
			{
				GUI_GetInput(10, 10, &dat, 8, 1);
				step_x=dat;
				
				GUI_DrawInt(315,55,step_x,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
				goto began;
			}break;
			
			case CWJ_STEP_STEP:
			{
				GUI_GetInput(10, 10, &dat, 8, 1);
				step_num=dat;
				
				GUI_DrawInt(395,55,step_num,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
				goto began;
			}break;
			
			case CWJ_STEP_FANGXAING:
			{
				GUI_Can_SetClr(232,52,20,66,VM_COLOR_WHITE);
				step_fangxiang=~step_fangxiang;
				if(step_fangxiang){
					GUI_Botton(400,100,50,30,VM_COLOR_LIGHTGRAY);
					GUI_DrawAscii(405,105,"UP",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
					if(step_ele<0)
						step_ele=0-step_ele;
					GUI_DrawInt(235,55,step_ele,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
				}else{
					GUI_Botton(400,100,50,30,VM_COLOR_RED);
					GUI_DrawAscii(410,105,"DOWN",VM_COLOR_BLACK,VM_COLOR_RED);
					if(step_ele>0)
						step_ele=0-step_ele;
					GUI_DrawInt(235,55,step_ele,VM_COLOR_BLACK,VM_COLOR_WHITE,4);
				}
			}break;
			
			case CWJ_STEP_ZHUANGLING:
			{
				pageid = PAGE_ID_MOTO_SET;
			}break;
		}
		
	}
	touchctl_HotArea_Dregister((___HOTAREA_DEF *)CWJ_BUTTON);
}

void moto_set()
{
	int ret,evt, sts,speed =100,i=1;
	
	Msg_Send.msg_id=0x01;
	Msg_Send.msg_len=8;
	can_bps_save(1000);
	
	RA8875_SetTextZoom(0,0);
	GUI_Clr(VM_COLOR_GRAY);
	
	GUI_Botton(20,100,100,50,VM_COLOR_LIGHTGRAY);
	GUI_DrawAscii(30,115,"zhuangling",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	
	GUI_Botton(20,200,50,30,VM_COLOR_LIGHTGRAY);
	GUI_DrawAscii(30,205,"Back",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	
	GUI_Botton(200,100,100,50,VM_COLOR_LIGHTGRAY);
	GUI_DrawAscii(230,115,"DOWN",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
	CWJ_CAN_Send_DOWN(step_ID);
	
	touchctl_HotArea_Register((___HOTAREA_DEF *)MOTO_SET, COUNTOF(MOTO_SET));
	while(pageid == PAGE_ID_MOTO_SET)
	{
		ret = system_event_pop(&evt, &sts);
		if (ret <= 0)
			continue;
		
		switch(evt)
		{
			case Zhuang_ling:
			{
				CWJ_CAN_Send_CMD(0X78,step_ID,1,0,0);
				arch_delay_ms(100);
				GUI_Botton(20,100,100,50,VM_COLOR_RED);
				GUI_DrawAscii(30,115,"zhuangling",VM_COLOR_BLACK,VM_COLOR_RED);
				CWJ_CAN_Send2(step_ID,speed);
				arch_delay_ms(1000);
				GUI_Botton(20,100,100,50,VM_COLOR_LIGHTGRAY);
				GUI_DrawAscii(30,115,"zhuangling",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
				CWJ_CAN_Send1(step_ID);
			}break;
			
			case DIRECTION:
			{
				if(i){
					
					GUI_Botton(200,100,100,50,VM_COLOR_RED);
					GUI_DrawAscii(230,115,"UP",VM_COLOR_BLACK,VM_COLOR_RED);
					CWJ_CAN_Send_UP(step_ID);
					i=0;
				}else{
					
					GUI_Botton(200,100,100,50,VM_COLOR_LIGHTGRAY);
					GUI_DrawAscii(230,115,"DOWN",VM_COLOR_BLACK,VM_COLOR_LIGHTGRAY);
					CWJ_CAN_Send_DOWN(step_ID);
					i=1;
				}
			}break;
			
			case BACK:
			{
				pageid = PAGE_ID_TEST_WEIJI;
			}break;
		}
		
	}
	touchctl_HotArea_Dregister((___HOTAREA_DEF *)MOTO_SET);
}





