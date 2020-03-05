#ifndef __ARCH_RA8875_H_
#define __ARCH_RA8875_H_

enum
{
	RA_FONT_16 = 0,		/* RA8875 字体 16点阵 */
	RA_FONT_24 = 1,		/* RA8875 字体 24点阵 */
	RA_FONT_32 = 2		/* RA8875 字体 32点阵 */
};
static void RA8875_WriteCmd(int _ucRegAddr);//写RA8875指令寄存器
static void RA8875_WriteData(int _ucRegValue);//写RA8875指令寄存器
int RA8875_ReadData(int _ucRegAddr);//读RA8875寄存器值
static void RA8875_WriteData16(int _usRGB);//写RA8875数据总线，16bit，用于RGB显存写入
static int RA8875_ReadData16(void);//读RA8875显存，16bit RGB
static void RA8875_WriteReg(int _ucRegAddr, int _ucRegValue);//写RA8875寄存器. RA8875的寄存器地址和数据都是8bit的
static int RA8875_ReadReg(int _ucRegAddr);//读RA8875寄存器. RA8875的寄存器地址和数据都是8bit的
static int RA8875_ReadStatus(void);//读RA8875状态寄存器
int RA8875_ReadID(void);//读取LCD驱动芯片ID
void RA8875_SetCursor(int _usX, int _usY);//设置写显存的光标位置（图形模式）
static void RA8875_SetReadCursor(int _usX, int _usY);//设置读显存的光标位置。 很多其他的控制器写光标和读光标是相同的寄存器，但是RA8875是不同的。
static void RA8875_SetTextCursor(int _usX, int _usY);//设置文本写入光标位置
void RA8875_Cursor_Set_Position(int _usX, int _usY);//设置写显存的光标位置（图形模式）
void BTE_SetTarBlock(int _usX, int _usY, int _usHeight, int _usWidth, int _ucLayer);//设置RA8875 BTE目标区块以及目标图层
void BTE_SetOperateCode(int _ucOperate);//设定BTE 操作码和光栅运算码
void RA8875_SetFrontColor(int _usColor);//设定前景色
void RA8875_SetBackColor(int _usColor);//设定背景色
void BTE_Start(void);//启动BTE操作
void BTE_Wait(void);//等待BTE操作结束
int RA8875_IsBusy(void);//RA8875是否忙
void RA8875_WaitBusy(void);//等待RA8875空闲
void RA8875_Layer1Visable(void);//RA8875 图层1显示打开
void RA8875_Layer2Visable(void);//只显示图层2
void RA8875_TouchInit(void);//初始化触摸板
int RA8875_TouchPress(void);//读取触摸事件状态
int RA8875_TouchReadX(void);//读取X位置ADC
int RA8875_TouchReadY(void);//读取Y位置ADC
int RA8875_TouchReadXY(int *_usX, int *_usY);//读取XY位置ADC
void RA8875_DispOn(void);//打开显示
void RA8875_DispOff(void);//关闭显示
void RA8875_PutPixel(int _usX, int _usY, int _usColor);//画1个像素
int RA8875_GetPixel(int _usX, int _usY);//读取1个像素
/*
*	函 数 名: RA8875_ClrScr
*	功能说明: 根据输入的颜色值清屏.RA8875支持硬件单色填充。该函数仅对当前激活的显示窗口进行清屏. 显示
*			 窗口的位置和大小由 RA8875_SetDispWin() 函数进行设置
*/
void RA8875_ClrScr(int _usColor);
void LCD8875_DrawHLine(int _usX1 , int _usY1 , int _usX2 , int _usColor);//主要用于UCGUI的接口函数
void LCD8875_DrawVLine(int _usX1 , int _usY1 , int _usY2 , int _usColor);//主要用于UCGUI的接口函数
void RA8875_DrawBMP(int _usX, int _usY, int _usHeight, int _usWidth, int *_ptr);//在LCD上显示一个BMP位图，位图点阵扫描次序：从左到右，从上到下
void RA8875_DrawLine(int _usX1 , int _usY1 , int _usX2 , int _usY2 , int _usColor, int mode);//采用RA8875的硬件功能，在2点间画一条直线
void RA8875_DrawTriangle(int _usX1 , int _usY1 , int _usX2 , int _usY2 ,int _usX3, int _usY3, int _usColor, int mode);//画三角形第三个点
void RA8875_DrawRect(int _usX, int _usY, int _usHeight, int _usWidth, int _usColor, int mode);//采用RA8875硬件功能绘制矩形
void RA8875_DrawCircle(int _usX, int _usY, int _usRadius, int _usColor, int mode);//绘制一个圆，笔宽为1个像素
void RA8875_DrawEllipse(int _usX, int _usY, int _usSRadius, int _usLRadius, int _usColor); //绘制一个椭圆，笔宽为1个像素
void RA8875_Drawcurve(int _usX, int _usY, int _usSRadius, int _usLRadius, int _usColor,int Fill_mode,int direction);//绘制曲线
void RA8875_SetFont(int _ucFontType, int _ucLineSpace, int _ucCharSpace);//文本模式，设置文字字体、行距和字距
void RA8875_SetTextZoom(int _ucHSize, int _ucVSize);//文本模式，设置文字的放大模式，1X,2X,3X, 4X
void RA8875_DispAscii(int _usX, int _usY, char *_ptr);/* 显示ASCII字符串，字符点阵来自于RA8875内部ROM中的ASCII字符点阵（8*16），不支持汉字.
		文字颜色、背景颜色、是否通透由其他的函数进行设置 */
void RA8875_DispStr(int _usX, int _usY, char *_ptr);/* 显示字符串，字符点阵来自于RA8875外接的字库芯片，支持汉字.
			文字颜色、背景颜色、是否通透由其他的函数进行设置 */
void RA8875_SetDispWin(int _usX, int _usY, int _usHeight, int _usWidth);//设置显示窗口，进入窗口显示模式。在窗口显示模式，连续写显存，光标会自动在设定窗口内进行递增
void RA8875_QuitWinMode(void);//退出窗口显示模式，变为全屏显示模式
void RA8875_SetBackLight(int _bright);//配置RA8875芯片的PWM1相关寄存器，控制LCD背光
void RA8875_Init();//初始化RA8875驱动芯片
void RA8875_Test();
void RA8875_PartClrScr(int x,int y,int w,int h,int _usColor);//局部填充
void RA8875_rounded_rectangle(int x1, int y1, int x2, int y2, int _usColor,int r1,int r2,int mode);//绘制圆角框
#endif
