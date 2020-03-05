

#define GET_DC_V  (0xFE)  //arg1 =1   ---DC24,arg1=2   ----DC12 , arg1 =3 ---读取拨码值
#define DCT_TEST_JUSTRUN  (0xFF)  //测试电机用的
#define DCT_TEST_GET_POS  (0xFC)  //测试电机用的获取当前pos值
#define DCT_TEST_GET_RUNINGSTS  (0xFB)  //测试电机用的获取电机运行状态

#define GET_STEP_HZ_T_DEBUG	(0xFA)   //获取电机的运行曲线

//#define DCT_SET_STEP_END_REPORT  (0xFB)  //设置电机完成应答使能


#define CLR_ERR 0x07
#define GET_DCT_OLD			0x09	// 取纱嘴状态
#define GET_DCT			0xFD	// 取纱嘴状态;//设置沙嘴状态//取宽度;//设置宽度,//复位
/*	cmd =0xfd; 
*	arg1,一个字节bit0-7.其中	0  表示取纱嘴状态 (需要板号)
*						  	1  表示设置沙嘴状态 (需要板号)
*						  	2  表示取传感器宽度 (需要板号+沙嘴号（单板0-15，多板0-7）)
*							3  表示设置传感器宽度(需要板号+沙嘴号（单板0-15，多板0-7,）)
*							4  单个沙嘴复位(测量传感器宽度)(需要板号+沙嘴号（单板0-15，多板0-7,）)
*							5  所有沙嘴复位（不分板号，不分沙嘴号）
*							6 读取版本号(需要板号)
							7 纱嘴数量 (0-16)  //对应电机数量0-8.
							8 设置细分值(1,2,4,8)   -- 全波，2细分，4细分，8细分
							9 设置纱嘴打出的步数()
*							... 备用
								  
*  arg2,两个字节,bit0-15. 其中	bit0-7组成的字节表示板号(0x10+boardid)   boardid =0,1...
*							bit8-15组成的字节表示沙嘴号(单板0-15，多板0-7,取或设置传感器宽度时用到)	
*  arg3 ,两个字节,bit0-15,	表示纱嘴状态(单板bit0-15,  多板bit0-7) 或传感器宽度值 


*	
*	
*	应答:   取值的时候才有应答
*   取沙嘴状态的话，最前面两个字节表示沙嘴状态，第三字节为0，第四个字节表示子板号(0x10+boardid)
*   取传感器宽度的时候，最前面两个字节表示宽度，第三字节表示沙嘴号，第四字节表示子板号(0x10+boardid)
*   读取版本号的话，第一个字节为版本号(0x00-0xff)，第二字节bit0-3表示一共几个板(至少1个)，bit4-7表示当前第几个板(0开始)
*   

*注：读取版本的时候可以确认当前是一块板沙嘴板还是两块沙嘴板，如果是一块，那么板号为0，沙嘴号0-15。 如果是多个板，那么板号0,1,2这样下去（目前就0,1），沙嘴号0-7。
*/								  
#define DCT_ISR				0x1E		// 电磁铁动作
#define UPGRADE_REQUEST	0x3A		// reboot this board

#define GET_BOARD_TYPE_ID	0x40		// Get board type id
#define GET_VERSION			0x30


#define STEP_ON			0x02	// 步进电机上电
#define STEP_OFF		0x03	// 步进电机断电


#define CMD_TEST_MODE_START	0x6c  //进入测试模式
/*
参数1(1字节):0XFE  --准备进入测试模式;0xFF ---进入测试模式

参数2(2字节):!0x0000  --开启测试模式

参数3(2字节):0x4851


*/

#ifdef FEET_SUPPORT

#define CMD_FEET_SETUP		0x49 /*
				      * 压脚电机控制设置
				      * 参数1: 设置分类
				      * 参数2: 参数
				      */
#define CMD_FEET_RUN		0x4A /*
				      * 压脚电机控制支持
				      *	参数1: 电机号
				      *	参数2: 16bit指令
				      *	  bit0~bit5:压脚修正
				      *	  bit14: 1->压脚作用 0->压脚抬起
				      *	  bit15: 1->压脚复位
				      */
#endif 




#define TRIANGLE_STEP_ISR		0x73 /* 三角电机动作命令 
							* 参数1: 三角电机号(0-3 然后本地自行转换到10-13).
							* 参数2: 	arg3.bit15=0,		表示位置值(arg2.bit0,arg2.bit1)(01--left,11--zero,10--right,0--reset)
							*			arg3.bit15=1，	arg2.bit0-15表示实际需要走到的位置。		
							*参数3:   	arg3   bit0,bit1表示此时是否需要检测相应传感器的状态				
							
							*/
#define TRIANGLE_STEP_GET 	0x74
							/* 三角电状态获取
							* 参数1: 三角电机号(0-3 然后本地自行转换到10-13).
							* 参数2:arg2>0  表示获取三角电机的实际postion 值，
							*			否则，获取三角电机当前位置的传感器信号.							
							*/

#define TRIANGLE_STEP_SET 	0x75
							/* 三角位置设置
							* 参数1: 三角电机号(0-3 然后本地自行转换到10-13).
							* 参数2:设置值
							* 参数3:arg3>0  表示设置三角电机的实际postion 值，
							*			否则，设置三角电机当前位置的传感器信号.							
							*/

#define TRIANGLE_STEP_MODE_SET	0x76
							/* 三角控制模式的设置。
							* arg1:0--普通模式，两个传感器的，三个位置。
							*	    1--特殊模式，两个传感器的，多个位置，零位与工作位分开。
							*
							*/

/*由一控4的板子，切换到一控八的板子程序*/
#define VER02 (0x02)   //版本号

/* by hlc 2014-12-16  修改:  上一个动作未做完的时候下一个动作继续的时候，没有按照最快线路走 */
#define VER03 (0x03)   //版本号

/* by hlc 2014-12-26  修改:增加了DC24和DC12的采样,还有拨码读取 */
#define VER04 (0x04)   //版本号

/*by hlc 20150114  修改:纱嘴电机实现整步代码，三角电机实现*/
#define VER05 (0x05)   //版本号


#define VER06 (0x06)   //版本号

#define VER07 (0x07)   //版本号//by hlc 20150126 确保来回都能在中心，并且每次到0位都能自动矫正.支持速度设置

#define VER08 (0x08)   //版本号//by hlc 20150205 实现4个版本的程序(通过最后2位拨码实现)

#define VER09 (0x09)   //版本号//by hlc 20150520 动作电机板，复位的时候，翻针位归零和吊目位归零，转的方向反了一下。

//#define VER (0x10)   //版本号//by hlc 20151007 动作电机板转速降低到3K  step.h里有个宏定义

//#define VER (0x11)   //版本号//by hlc 20151207 纱嘴电机速度加快到5.5K

//#define VER12 (0x12)   //版本号//by hlc 20151211纱嘴速度恢复到5K 增加了一些测试命令，还增加了纱嘴电机到工作完成之后的应答
#define VER13 (0x13) 		//by hlc 20151217 动作电机的时候，多一种运动模式，两个传感器的，通过0x76命令设置。
#define VER14 (0x14) 		//by hlc 20151217 动作电机的时候，多一种运动模式，两个传感器的，通过0x76命令设置。

#define VER15 (0x15) 		//by hlc 20160312 纱嘴电机做了别的用处。

#define VER16 (0x16) 		//by hlc 20160329 纱嘴电机做别的用的时候，电机可以分开设置了

#define VER17 (0x17) 		//by hlc 20160506 强隆支持5个状态的独立版本，step.h中有宏定义

#define VER18 (0x18) 		//by hlc 20160511 新增速度设置接口，根据频率值设置

#define VER19 (0x19) 		//by hlc 20160603 新增了电机方向设置，序号映射。支持动作电机数量扩展到8个。

#define VER20 (0x20) 		//by hlc 20160604 单个传感器的三角电机，负位置一直为0，考虑走穿的问题。那么需要，当不在
						//零位的时候，需要向正方向走走看，走50步，如果找到零位了，那就继续走，如果找不到，那就调头。说明没有走穿

#define VER21 (0x21) 		//by hlc 20160612 电机过零位的时候做了一下失步检查，检查的值为20步


#define VER22 (0x22) 		//by hlc 20160701  0XFD 07 07 命令以前有问题，现在改了一下。

#define VER23 (0x23) 		//by hlc 20160713 三角电机，-100到0的时候走过头了需要自动回来。


#define VER24	 (0x24) 		//by hlc 20160726 找到2号拉杆出问题的原因了，TIME2为32位定时器

//#define VER	 (0x25) 		//by hlc 20160805 过零检查这里做了信号滤波。速度分开设置

#define VER26	 (0x26) 		//by hlc 20160824 通过拨码来实现1+1模式

#define VER27	 (0x27) 		//by hlc 20161206作为拉杆电机的时候，断电续织起来，第一次失步报警的范围放大
#define VER28	 (0x28) 		//by hlc 20170108 
#define VER29 		(0x29)		//by hlc 20170110 正负同时修正
#define VER30 		(0x30)		//by hlc 20170112 检查延时改成10ms，取消实时检查。报警过滤由原来的2次改成5次
#define VER31 		(0x31)		//by hlc 20170114 过零的时候，失步报警加上。


#define VER32 		(0x32)		//by hlc 20170115,从135到两边的时候 自动减到3K--32
#define VER33 			(0x33)			//by hlc 20170115,从信号抖动
#define VER34 			(0x34)			//by hlc 20170519 byhlc 归零这里稍加修改,及时减速，避免过冲

#define VER 			(0x35)			//by hlc 20190712 byhlc 慈星用作推针电机的时候回零位不报警


/*
**	对应拨码4,3,2,1
**	bit3,bit2,bit1,bit0
**	0,	0,	0,	0	--控制动作电机，用一个传感器
**	0,	1,	0,	0	--控制动作电机，用两个传感器
**	1,	0,	0,	0	--控制沙嘴电机，用八个电机
**	1,	1,	0,	0	--控制沙嘴电机，用四个电机
**
*/



#define YARN_STEP_BOARD_ID	(0x8830)

#define TRIANGLE_BOARD_ID		(0x8840)




#if 0

#define POWER_ON		0x00	// 外设加电
#define POWER_OFF		0x01	// 外设断电
#define STEP_ON			0x02	// 步进电机上电
#define STEP_OFF		0x03	// 步进电机断电

#define STEP_ALL_RST	0x04	// 所有度目电机归零
#define ST_INIT			0x05	// 所有选针器初始化
#define GET_DIR_ERR		0x06	// 检查换向传感器错误状态
#define CLR_ERR			0x07	// 清除机头错误
#define GET_DIR			0x08	// 取机头换向传感器状态
#define GET_DCT			0x09	// 取纱嘴状态
#define GET_ERR			0x0A	// 取机头错误状态
#define GET_ZERO 		0x0B	// 取机头度目零位状态
#define GET_ACT 		0x0C	// 取机头三角状态
#define GET_XZQ 		0x0D	// 取机头选针器状态
#define GET_MOT 		0x0E	// 取机头度目位置
#define ZHEN_ISR		0x0F	// 发送选针数据
#define DIR_ISR			0x10	// 设置机头工作方向
#define ST1_ISR			0x11	// 设置1号选针器起刀
#define ST2_ISR			0x12	// 设置2号选针器起刀
#define ST3_ISR			0x13	// 设置3号选针器起刀
#define ST4_ISR			0x14	// 设置4号选针器起刀
#define ST5_ISR			0x15	// 设置5号选针器起刀
#define ST6_ISR			0x16	// 设置6号选针器起刀
#define ST7_ISR			0x17	// 设置7号选针器起刀
#define ST8_ISR			0x18	// 设置8号选针器起刀
#define XZQ_HV_ISR 		0x19	// 设置选针器高压
#define DCT_HV_ISR 		0x1A	// 设置电磁铁高压
#define XZQ_LV_ISR		0x1B	// 设置选针器低压
#define STEP_ISR		0x1C	// 度目动作					
/*		度目动作					
*		arg1	-对应度目电机号
*		arg2 -电机步数
*		arg3 - 是否检查传感器2的状态( 0--不检查，!0--要检查 )
*/
#define XZQ_ISR			0x1D	// 选针器动作, test
#define DCT_ISR			0x1E	// 电磁铁动作
#define SK_ISR			0x1F	// 生克电机动作
#define SK_RST_ISR		0x20	// 生克电机归零
#define GET_SK_BUSY		0x21	// 检查生克电机忙状态
#define GET_STEP_BUSY		0x22	// 检查度目电机忙状态
#define GET_DAO_ISR		0x23	// 取当前刀
#define DAO_ISR			0x24	// 设置当前刀
#define DAO_MAX_ISR		0x25	// 设置总刀数
#define GET_SK_ISR		0x26	// 取生克位置
#define SET_SK_ISR		0x27	// 设置生克位置
#define SET_STEP_ISR		0x28	// 设置度目位置
#define LEFT_STEP_RST_ISR	0x29	// 左侧度目归零
#define RIGHT_STEP_RST_ISR	0x2A 	// 右侧度目归零
#define STEP_RST_SPD_ISR	0x2B	// 度目归零速度
#define STEP_MAX_SPD_ISR	0x2C	// 度目运行速度
#define SET_ERR_ENABLE_ISR	0x2D	// 设置机头错误屏蔽
#define SET_MODE_ISR		0x2E	// 单系统模式????
#define SET_STEP_ENABLE		0x2F	// 设置度目工作开关(默认值0xFF)
#define GET_VER_ISR		0x30	// 取机头程序版本号
#define SET_XZQ_MODE_F		0x31	// 发送前床选针数据
#define SET_XZQ_MODE_B		0x32	// 发送后床选针数据
#define SET_XZQ_MODE_TYPE	0x33	/*
					 * 设置前后床工作选针器
					 * 命令码 + 前床 + 后床
					 */
#define SIG_STEP_RST		0x34	// 单个度目电机归零
#define DCT_HV1_ISR		0x35	// 设置电磁铁高压2
#define SET_STEP_MODE		0x36	// 设置步进电机工作模式
#define HEAD_DEBUG		0x37	// 调试接口
#define SET_SETP_DIR		0x38	/* 设置步进电机方向(默认值0x0155)
					 * 支持版本(0x2012)
					 */
#define SET_STEP_RESOLUTION	0x39	/* 设置步进电机板细分数
					 * (1, 2, 4, 8 ...)
					 * 支持版本(0x2013)
					 */
#define UPGRADE_REQUEST		0x3A	/* 升级命令 */ 
#define UPGRADE_DATA		0x3B	/* 升级命令 */ 
#define UPGRADE_ENDDATA		0x3C	/* 升级命令 */ 
#define UPGRADE_DATA_RECEIVED	0x3D	/* 升级命令 */ 
#define UPGRADE_SUCCESS		0x3E	/* 升级命令 */ 
#define UPGRADE_FORCE_REQ	0x3F	/* 升级命令 */ 

#define GET_BOARD_TYPE		0x40	// 支持版本(0x2015)
  #define TYPE_LF2407	0x8001
  #define TYPE_LPC2919	0x8002
  #define TYPE_F2812		0x8003

#define SET_EMF_WORK_MODE	0x41 /*
				      *	by xhl 2010/06/17 
				      * 支持版本(0x2016)
				      * 用于设置电磁铁是否工作在常通电
				      * 模式下
				      * 命令码 + 电磁铁序号 + 工作模式
				      * 当参数1为非零时，该电磁铁工作在
				      * 常通电模式下
				      * 参数0为电磁铁序号: 0 -- 39
				      * 参数1为工作模式: 0/!0
				      */
#define GET_JQD_BLADE		0x42 /*
				      * by xhl 2010/08/09
				      * 取机头选针器当前就绪的工作刀头
				      * 命令码 + 选针器序号(0 -- 7)
				      */
#define SET_STEP_CHECK		0x43 /*
				      * by xhl 2010/11/15
				      * 设置电机位置检测精度
				      * 对所有电机生效
				      */
#define SET_STEP_FAST_MODE	0x44 /*
				      * 电机以最短行程到达目标位置
				      */
#define ENCODER_SETUP		0x45 /*
				      * 电机编码器功能设置
				      * 0: 禁用
				      * 1: 启用
				      */
#define ENCODER_CHECK		0x46 /*
				      * 返回各编码器的状态
				      */
#define OVERLOAD_SETUP		0x47 /*
				      * 过流检测功能设置
				      * 0: 禁用
				      * 1: 启用
				      */
#define SINKMOTOR_MAX_SPEED	0x48 /*
				      * 生克电机运行速度
				      */
#define CMD_FEET_SETUP		0x49 /*
				      * 压脚电机控制设置
				      * 参数1: 设置分类
				      * 参数2: 参数
				      */
#define CMD_FEET_RUN		0x4A /*
				      * 压脚电机控制支持
				      *	参数1: 电机号
				      *	参数2: 16bit指令
				      *	  bit0~bit5:压脚修正
				      *	  bit14: 1->压脚作用 0->压脚抬起
				      *	  bit15: 1->压脚复位
				      */
#define CMD_STEP_RESET_DELAY_TIME	0x4B /*
				      * 步进电机复位时延时设置
				      *	参数1: 电机号
				      *	参数2: 延时时间（2ms）
				      */
#define CMD_SETUP_STEP		0x4C /*
				      * 步进电机工作参数设置接口
				      * 参数1: 参数类型
				      * 参数2: 参数值
				      *
				      * 参数类型=
				      *		1: 度目零位工作区间
				      *		2: 生克零位工作区间
				      *		3: 度目零位修正
				      */
#define CMD_PARA_SETUP		0x4D /*
				      * 工作参数设置接口
				      * 参数1: 参数类型
				      * 0x10: 选针器重新映射起始刀
				      *
				      *
				      */

#define JAQ_WORKMODE_8_16_SET		0x4E /*
				      * 选针器工作参数设置接口
				      * 参数1: 参数类型
				      * 1: 选针器工作在8_16模式下
				      * 0: 选针器回复到8路模式
				      */
#define GET_BOARD_FUNC_TYPE		0x4F	/* 支持版本 0x2065
					  * 机头箱功能类型
					  * bit1-bit0: 系统数，1-4
					  * bit2 = 1: 支持编码器
					  * bit3 = 1: 支持压脚
					  */
#define GET_BOARD_BUILD_TIME		0x50    /* 支持版本 0x2065
					  * 机头箱代码编译时间
					  */
#define SET_SINKER_NUM			0x51 /* 设置生克电机个数 */

#define SET_TANZHEN_KNIT_DELAY	0x52	/* by wf 2013 11 05
					* 快速回转，增加探针报警的控制；
					* 参数1 0:关闭两个探针报警；1:设置1号；2:设置2号；3:设置1和2号
					* 参数2 arg1>0时；=0，关闭报警；=1，打开报警；>1:延时时间，时间以100ms为单位
					*/
					
#define SET_STEPMOTOR_MODE_FAST		0x53 	/* by wf 2013/11/25
					* 可变度目设置命令；此命令设置后，使用驱动器驱动度目,生克，压脚电机
					* bit0 = 1; sti用驱动器； 0 正常使用原来的方式
					* bit1 = 1; sk
					* bit2 = 1; yj
					* 2014/4 改为每个bit对应1个电机
					*/
					
#define GET_STEPMOTOR_ZERO_SIG		0x54	/* by wf 2013/11/25
					* 获取单个度目电机的零位；参数arg1: 电机序号1 - 8
					*/
					
#define STEP_ENCODER_CHECK	0x55 /*
				      * 返回指定电机编码器的状态
				      */
#define GET_STEPMOTOR_BUSY_STATUS	0x56 /* by wf 2013/11/26
					  * 可变度目 电机忙状态检查，
					  * 参数arg1: 电机序号
					  */



					  
#define GET_ZERO_OTHER 		0x5B	// 取机头其他电机的零位状态；除了度目和生克

#define ST9_ISR			0x60	// 设置9号选针器起刀
#define ST10_ISR		0x61	// 设置10号选针器起刀
#define ST11_ISR		0x62	// 设置11号选针器起刀
#define ST12_ISR		0x63	// 设置12号选针器起刀
#define ST13_ISR		0x64	// 设置13号选针器起刀
#define ST14_ISR		0x65	// 设置14号选针器起刀
#define ST15_ISR		0x66	// 设置15号选针器起刀
#define ST16_ISR		0x67	// 设置16号选针器起刀

//2014-03-05 
#define	  SET_MOTOR_ENABLE		(0x68)			//设置使能电机
#define	  GET_MOTOR_NEWSTATE	(0x69)			//取新增加的传感器的状态

#define GET_ERROR_CODE		0x59 /*
				      * 返回错误码
				      */

#define TANZHEN_SETUP		0x6a
#define TANZHEN_NEW_SET	0x6b

#define CHECK_STATUS_START	0x70	/* 开始查询 
						* bit0: 方向传感器新检测方式，(越发 1 - 0 - 1)
						*/
#define CHECK_STATUS_STOP		0x71	/* 结束查询 
						* arg1:	bit0: 方向传感器新检测方式，越发
						* 		bit1: 生克电机查询
						*		bit2: 左度目 left sti prepared 0/2/4/6/
						*		bit3: 右度目 right sti prepared	1/3/5/7/
						*		bit4: 纱嘴状态 yarn zero
						* arg2: param;
						*/
						
#define HP3G_NEEDLE_2STEP		0x72 /* 慈星三针机，新增命令
							* 选针器针中断 计算 从加减1改为加减2；
							*/

#define KEYDOG_READ		0x33
#define KEYDOG_CHECK		0x35

#define NEW_STEP_TYPE_SET		0x80 /* 电机配置命令 
							* 参数1: 配置类型
							* 参数2: start index
							* 参数3: stop index
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


#define TRIANGLE_STEP_ISR		0x73 /* 三角电机动作命令 
							* 参数1: 三角电机号(0-3 然后本地自行转换到10-13).
							* 参数2: 	arg2   bit15=0,		表示位置值(bit0,bit1)(01--left,11--zero,10--right,0--reset)
							*				  bit15 =1，	bit0-14表示实际需要走到的位置。		
							*参数3:   	arg3   bit0,bit1表示此时是否需要检测相应传感器的状态				
							
							*/
#define TRIANGLE_STEP_GET 	0x74
							/* 三角电状态获取
							* 参数1: 三角电机号(0-3 然后本地自行转换到10-13).
							* 参数2:arg2>0  表示获取三角电机的实际postion 值，
							*			否则，获取三角电机当前位置的传感器信号.							
							*/

#define TRIANGLE_STEP_SET 	0x75
							/* 三角位置设置
							* 参数1: 三角电机号(0-3 然后本地自行转换到10-13).
							* 参数2:设置值
							* 参数3:arg3>0  表示设置三角电机的实际postion 值，
							*			否则，设置三角电机当前位置的传感器信号.							
							*/


#define CMD_HEAD_LOG_ENABLE	0xF1 /* wf, 2013/10/21
						* 参数arg1:
						* 0x01	打开选针器输出		0x81	关闭选针器输出
						* 0x02	打开三角电磁铁输出	0x82	关闭三角电磁铁输出
						* 0x03	打开纱嘴电磁铁输出	0x83	关闭纱嘴电磁铁输出
						* 0x04	打开度目电磁铁输出	0x84	关闭度目电磁铁输出
						* 0x05	打开生克电磁铁输出	0x85	关闭生克电磁铁输出
						* 0x06	打开压脚电磁铁输出	0x87	关闭压脚电磁铁输出
						* 0x0F	打开所有			0x8F	关闭所有
						*/
#define LOGOUT_XZQ_MASK	0x01
#define LOGOUT_DCT_MASK	0x02
#define LOGOUT_YARN_MASK	0x04
#define LOGOUT_STI_MASK	0x08
#define LOGOUT_SK_MASK		0x10
#define LOGOUT_YJ_MASK		0x20



/*
*	wf, 2013/10/21
*	机头log输出 cmd
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
#define LOG_OUT_YJ1		0x1F	//压脚
#define LOG_OUT_YJ2		0x20
#define LOG_OUT_YJ3		0x21
#define LOG_OUT_YJ4		0x22
/* log out end */

// 说明:
// 版本信息最高4位代表系统数
// 如:
//    版本为0x2011->支持单双系统 版本号为0x0011
//    版本为0x3011->支持三系统 版本号为0x0011
//
// 请使用版本号为0x2010之后的程序
// 可以用版本查询命令(0x30)查询程序版本
//
//

#endif
