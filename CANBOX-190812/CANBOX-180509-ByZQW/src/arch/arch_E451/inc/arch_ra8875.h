#ifndef __ARCH_RA8875_H_
#define __ARCH_RA8875_H_

enum
{
	RA_FONT_16 = 0,		/* RA8875 ���� 16���� */
	RA_FONT_24 = 1,		/* RA8875 ���� 24���� */
	RA_FONT_32 = 2		/* RA8875 ���� 32���� */
};
static void RA8875_WriteCmd(int _ucRegAddr);//дRA8875ָ��Ĵ���
static void RA8875_WriteData(int _ucRegValue);//дRA8875ָ��Ĵ���
int RA8875_ReadData(int _ucRegAddr);//��RA8875�Ĵ���ֵ
static void RA8875_WriteData16(int _usRGB);//дRA8875�������ߣ�16bit������RGB�Դ�д��
static int RA8875_ReadData16(void);//��RA8875�Դ棬16bit RGB
static void RA8875_WriteReg(int _ucRegAddr, int _ucRegValue);//дRA8875�Ĵ���. RA8875�ļĴ�����ַ�����ݶ���8bit��
static int RA8875_ReadReg(int _ucRegAddr);//��RA8875�Ĵ���. RA8875�ļĴ�����ַ�����ݶ���8bit��
static int RA8875_ReadStatus(void);//��RA8875״̬�Ĵ���
int RA8875_ReadID(void);//��ȡLCD����оƬID
void RA8875_SetCursor(int _usX, int _usY);//����д�Դ�Ĺ��λ�ã�ͼ��ģʽ��
static void RA8875_SetReadCursor(int _usX, int _usY);//���ö��Դ�Ĺ��λ�á� �ܶ������Ŀ�����д���Ͷ��������ͬ�ļĴ���������RA8875�ǲ�ͬ�ġ�
static void RA8875_SetTextCursor(int _usX, int _usY);//�����ı�д����λ��
void RA8875_Cursor_Set_Position(int _usX, int _usY);//����д�Դ�Ĺ��λ�ã�ͼ��ģʽ��
void BTE_SetTarBlock(int _usX, int _usY, int _usHeight, int _usWidth, int _ucLayer);//����RA8875 BTEĿ�������Լ�Ŀ��ͼ��
void BTE_SetOperateCode(int _ucOperate);//�趨BTE ������͹�դ������
void RA8875_SetFrontColor(int _usColor);//�趨ǰ��ɫ
void RA8875_SetBackColor(int _usColor);//�趨����ɫ
void BTE_Start(void);//����BTE����
void BTE_Wait(void);//�ȴ�BTE��������
int RA8875_IsBusy(void);//RA8875�Ƿ�æ
void RA8875_WaitBusy(void);//�ȴ�RA8875����
void RA8875_Layer1Visable(void);//RA8875 ͼ��1��ʾ��
void RA8875_Layer2Visable(void);//ֻ��ʾͼ��2
void RA8875_TouchInit(void);//��ʼ��������
int RA8875_TouchPress(void);//��ȡ�����¼�״̬
int RA8875_TouchReadX(void);//��ȡXλ��ADC
int RA8875_TouchReadY(void);//��ȡYλ��ADC
int RA8875_TouchReadXY(int *_usX, int *_usY);//��ȡXYλ��ADC
void RA8875_DispOn(void);//����ʾ
void RA8875_DispOff(void);//�ر���ʾ
void RA8875_PutPixel(int _usX, int _usY, int _usColor);//��1������
int RA8875_GetPixel(int _usX, int _usY);//��ȡ1������
/*
*	�� �� ��: RA8875_ClrScr
*	����˵��: �����������ɫֵ����.RA8875֧��Ӳ����ɫ��䡣�ú������Ե�ǰ�������ʾ���ڽ�������. ��ʾ
*			 ���ڵ�λ�úʹ�С�� RA8875_SetDispWin() ������������
*/
void RA8875_ClrScr(int _usColor);
void LCD8875_DrawHLine(int _usX1 , int _usY1 , int _usX2 , int _usColor);//��Ҫ����UCGUI�Ľӿں���
void LCD8875_DrawVLine(int _usX1 , int _usY1 , int _usY2 , int _usColor);//��Ҫ����UCGUI�Ľӿں���
void RA8875_DrawBMP(int _usX, int _usY, int _usHeight, int _usWidth, int *_ptr);//��LCD����ʾһ��BMPλͼ��λͼ����ɨ����򣺴����ң����ϵ���
void RA8875_DrawLine(int _usX1 , int _usY1 , int _usX2 , int _usY2 , int _usColor, int mode);//����RA8875��Ӳ�����ܣ���2��仭һ��ֱ��
void RA8875_DrawTriangle(int _usX1 , int _usY1 , int _usX2 , int _usY2 ,int _usX3, int _usY3, int _usColor, int mode);//�������ε�������
void RA8875_DrawRect(int _usX, int _usY, int _usHeight, int _usWidth, int _usColor, int mode);//����RA8875Ӳ�����ܻ��ƾ���
void RA8875_DrawCircle(int _usX, int _usY, int _usRadius, int _usColor, int mode);//����һ��Բ���ʿ�Ϊ1������
void RA8875_DrawEllipse(int _usX, int _usY, int _usSRadius, int _usLRadius, int _usColor); //����һ����Բ���ʿ�Ϊ1������
void RA8875_Drawcurve(int _usX, int _usY, int _usSRadius, int _usLRadius, int _usColor,int Fill_mode,int direction);//��������
void RA8875_SetFont(int _ucFontType, int _ucLineSpace, int _ucCharSpace);//�ı�ģʽ�������������塢�о���־�
void RA8875_SetTextZoom(int _ucHSize, int _ucVSize);//�ı�ģʽ���������ֵķŴ�ģʽ��1X,2X,3X, 4X
void RA8875_DispAscii(int _usX, int _usY, char *_ptr);/* ��ʾASCII�ַ������ַ�����������RA8875�ڲ�ROM�е�ASCII�ַ�����8*16������֧�ֺ���.
		������ɫ��������ɫ���Ƿ�ͨ͸�������ĺ����������� */
void RA8875_DispStr(int _usX, int _usY, char *_ptr);/* ��ʾ�ַ������ַ�����������RA8875��ӵ��ֿ�оƬ��֧�ֺ���.
			������ɫ��������ɫ���Ƿ�ͨ͸�������ĺ����������� */
void RA8875_SetDispWin(int _usX, int _usY, int _usHeight, int _usWidth);//������ʾ���ڣ����봰����ʾģʽ���ڴ�����ʾģʽ������д�Դ棬�����Զ����趨�����ڽ��е���
void RA8875_QuitWinMode(void);//�˳�������ʾģʽ����Ϊȫ����ʾģʽ
void RA8875_SetBackLight(int _bright);//����RA8875оƬ��PWM1��ؼĴ���������LCD����
void RA8875_Init();//��ʼ��RA8875����оƬ
void RA8875_Test();
void RA8875_PartClrScr(int x,int y,int w,int h,int _usColor);//�ֲ����
void RA8875_rounded_rectangle(int x1, int y1, int x2, int y2, int _usColor,int r1,int r2,int mode);//����Բ�ǿ�
#endif
