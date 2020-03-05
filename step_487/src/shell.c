/*
 * shell.c	(by xhl 2014/11/01)
 */
#include "shell.h"

SHELL_HISTORY shell_rcv_idx[SHELL_MAX_LIST];
volatile unsigned int shell_rcv_rptr;
volatile unsigned int shell_rcv_wptr;
#ifdef SHELL_CMD_ECHO_SUPPORT
volatile unsigned int shell_rcv_attr;
#endif // SHELL_CMD_ECHO_SUPPORT
volatile unsigned int shell_find_idx;
volatile unsigned long shell_time;
#ifdef SHELL_LOGIN_SUPPORT
volatile unsigned char Shell_Login_Flag;
#endif // SHELL_LOGIN_SUPPORT
volatile int Shell_State = Shell_State_Stopped;
volatile int Shell_Total_cmd = 0;

volatile char Shell_Buff[SHELL_BUFFER_SIZE];
volatile char Shell_LineBuff[SHELL_LINE_BUFFER_SIZE];

void Shell_PrintStr(char *str)
{
	do {
		Shell_PrintChar(*str++);
	} while(*str);
}

void Shell_PrintLine(char *str)
{
	Shell_PrintStr(str);
	Shell_PrintStr(Shell_EndLineStr);
}

#ifdef SHELL_LOGIN_SUPPORT
int shell_Login_Status()
{
	return Shell_Login_Flag;
}

int shell_Login_in()
{
	Shell_Login_Flag = 1;
}

int shell_Login_out()
{
	Shell_Login_Flag = 0;
}

#endif // SHELL_LOGIN_SUPPORT

#ifdef SHELL_CMD_ECHO_SUPPORT
void Shell_Set_Attr(int attr)
{
	shell_rcv_attr = attr;
}
#endif // SHELL_CMD_ECHO_SUPPORT

void shell_timer_1ms()
{
	if(shell_time < 10000)
		shell_time++;
}

int shell_write_to_Buff(char ch);
int shell_is_ctrl_key(char ch, int time);
void shell_push_char(char ch)
{
	int idx;

	if(shell_is_ctrl_key(ch, shell_time)) {
		goto done;
	}

#ifdef SHELL_CMD_ECHO_SUPPORT
	if(shell_rcv_attr & Shell_Echo)
	{
		if(shell_rcv_attr & Shell_Echo_Pswd) {
			Shell_PrintChar('*');
		}
		else if(shell_rcv_attr & Shell_EchoChar)
			Shell_PrintChar(shell_rcv_attr & Shell_EchoChar);
		else
			Shell_PrintChar(ch);
	}
#endif // SHELL_CMD_ECHO_SUPPORT

	idx = shell_write_to_Buff(ch);
done:
	shell_time = 0;
	return ;
}

int shell_write_to_Buff(char ch)
{
	int idx;

	idx = shell_rcv_idx[shell_rcv_wptr].eidx;
	Shell_Buff[idx++] = ch;
	if(idx >= SHELL_BUFFER_SIZE)
		idx -= SHELL_BUFFER_SIZE;

	shell_rcv_idx[shell_rcv_wptr].eidx = idx;

	return idx;
}

int shell_BackSpace()
{
	if(shell_rcv_idx[shell_rcv_wptr].sidx != shell_rcv_idx[shell_rcv_wptr].eidx)
	{
		if(shell_rcv_idx[shell_rcv_wptr].eidx > 0)
			shell_rcv_idx[shell_rcv_wptr].eidx --;
		else
			shell_rcv_idx[shell_rcv_wptr].eidx = SHELL_BUFFER_SIZE - 1;
		Shell_Buff[shell_rcv_idx[shell_rcv_wptr].eidx] = '\0';
#ifdef SHELL_CMD_ECHO_SUPPORT
		if(shell_rcv_attr & Shell_Echo)
			Shell_PrintStr("\b \b");
#endif // SHELL_CMD_ECHO_SUPPORT
	}

	return 0;
}

int shell_Return()
{
	int idx;
	int wptr;

#ifdef SHELL_CMD_ECHO_SUPPORT
	if(shell_rcv_attr & Shell_AutoEnter)
#endif // SHELL_CMD_ECHO_SUPPORT
		Shell_PrintStr(Shell_EndLineStr);

	idx = shell_write_to_Buff('\0');

	wptr = shell_rcv_wptr + 1;
	if(wptr >= SHELL_MAX_LIST) {
		wptr -= SHELL_MAX_LIST;
	}
	if(wptr == shell_rcv_rptr) {
		// is full ???
		shell_rcv_idx[shell_rcv_wptr].eidx = shell_rcv_idx[shell_rcv_wptr].sidx;
	}
	else {
		shell_rcv_wptr = wptr;
		shell_find_idx = wptr;
		shell_rcv_idx[wptr].sidx = idx;
		shell_rcv_idx[wptr].eidx = idx;
	}

	return 0;
}

#ifdef SHELL_CMD_ECHO_SUPPORT
int shell_find(int mode)
{
	int idx = shell_find_idx;
	int cnt;

find_next:
	if(mode == 0) {
		if(idx == shell_rcv_wptr)
			return 0;
		idx ++;
		if(idx >= SHELL_MAX_LIST)
			idx -= SHELL_MAX_LIST;
	}
	else if(mode == 1) {
		if(idx > 0) {
			idx --;
		}
		else {
			idx = SHELL_MAX_LIST - 1;
		}
		if(idx == shell_rcv_wptr)
			return 0;
	}

	cnt = shell_rcv_idx[idx].eidx - shell_rcv_idx[idx].sidx;
	if(cnt < 0) cnt += SHELL_BUFFER_SIZE;
	if(cnt <= 1) goto find_next;

	shell_find_idx = idx;

	cnt = shell_rcv_idx[shell_rcv_wptr].eidx - shell_rcv_idx[shell_rcv_wptr].sidx;
	if(cnt < 0) cnt += SHELL_BUFFER_SIZE;
	while(cnt--) {
		Shell_PrintStr("\b \b");
	}
	//Shell_PrintStr(Shell_StartStr);
	shell_rcv_idx[shell_rcv_wptr].eidx = shell_rcv_idx[shell_rcv_wptr].sidx;
	// copy it
	idx = shell_rcv_idx[shell_find_idx].sidx;
#if 0
	do {
		char ch = Shell_Buff[idx];
		if(ch == 0) break;
		shell_write_to_Buff(ch);
		Shell_PrintChar(ch);
		idx ++;
		if(idx >= SHELL_BUFFER_SIZE)
			idx -= SHELL_BUFFER_SIZE;
	} while(idx != shell_rcv_idx[shell_find_idx].eidx);
#else
	while(idx != shell_rcv_idx[shell_find_idx].eidx) {
		char ch = Shell_Buff[idx];
		if(ch == 0) break;
		shell_write_to_Buff(ch);
		Shell_PrintChar(ch);
		idx ++;
		if(idx >= SHELL_BUFFER_SIZE)
			idx -= SHELL_BUFFER_SIZE;
	}
#endif

	return 0;
}

void hook_left_key()
{
	//printf("\b");
}

void hook_up_key()
{
	shell_find(1);
}

void hook_down_key()
{
	shell_find(0);
}
#endif // SHELL_CMD_ECHO_SUPPORT

int Shell_GetLine(char *buff, int size)
{
	int rptr;
	int i;

	if(shell_rcv_rptr == shell_rcv_wptr) {
		return 0;
	}

	rptr = shell_rcv_idx[shell_rcv_rptr].sidx;
	i = 0;
	do {
		buff[i] = Shell_Buff[rptr];
		if(rptr++ >= SHELL_BUFFER_SIZE)
			rptr -= SHELL_BUFFER_SIZE;
		if(i++ >= size) break;
	} while(rptr != shell_rcv_idx[shell_rcv_rptr].eidx);
	rptr = shell_rcv_rptr + 1;
	if(rptr >= SHELL_MAX_LIST) {
		rptr -= SHELL_MAX_LIST;
	}
	shell_rcv_rptr = rptr;
	// by xhl 2010/12/16
	shell_find_idx = shell_rcv_rptr;

	return i;
}

int Shell_ReadLine_Poll(char *buff, int size)
{
	return Shell_GetLine((char*)buff, size);
}

static int Shell_DoCMD(char *CMD);
void Shell_Poll(void)
{
	if(Shell_ReadLine_Poll((char*)Shell_LineBuff, SHELL_LINE_BUFFER_SIZE)) {
		//printf("%s\n\r", Shell_LineBuff);
		if(strlen((const char*)Shell_LineBuff) != 0)
			Shell_DoCMD((char*)Shell_LineBuff);
		Shell_PrintStr(Shell_StartStr);
	}
}

void Shell_Buff_init()
{
	int i;

	shell_rcv_rptr = 0;
	shell_rcv_wptr = 0;
	shell_find_idx = 0;
	for(i = 0; i < SHELL_MAX_LIST; i++) {
		shell_rcv_idx[i].sidx = 0;
		shell_rcv_idx[i].eidx = 0;
	}
}

void Shell_Init(void)
{
	Shell_Buff_init();
#ifdef SHELL_CMD_ECHO_SUPPORT
	Shell_Set_Attr(Shell_RL_Attr);
    #ifdef SHELL_LOGIN_SUPPORT
	Shell_Set_Attr(Shell_Echo | Shell_AutoEnter | Shell_Echo_Pswd);
	Shell_Login_Flag = 0;
    #endif // SHELL_LOGIN_SUPPORT
#endif // SHELL_CMD_ECHO_SUPPORT

#ifdef SHELL_COLOR_SUPPORT
	{
	void Shell_SetTxtAttr(char FGColor,char BGColor,char Attrs);
	Shell_SetTxtAttr(-1,-1,Shell_Attrs_None);
	Shell_SetTxtAttr(Shell_DefFGColor,Shell_DefBGColor,Shell_DefAttr);
	}
#endif //SHELL_COLOR_SUPPORT

	{
	int Shell_GetCmdCount(void);
	Shell_Total_cmd = Shell_GetCmdCount();
	}

	Shell_PrintLine(Shell_StrVer);
	Shell_PrintStr(Shell_StartStr);
}


char Shell_Help(char **argv, int argc);
char Shell_Clear(char **argv, int argc);
char Shell_Exit(char **argv, int argc);
char Shell_Ver(char **argv, int argc);
static CMD_DEF Shell_CMDs[SHELL_CMD_HIDEIDX] = {
	{"help", Shell_Help, "Shell Help."},
	{"cls", Shell_Clear, "Clear the Shell Output."},
#ifdef SHELL_LOGIN_SUPPORT
	{"quit", Shell_Exit, "Quit Shell."},
#endif
	{"version", Shell_Ver, "Show Shell version."},
#ifdef SHELL_COLOR_SUPPORT
	//{"color", Shell_Color, "Shell Set Attr."},
#endif //SHELL_COLOR_SUPPORT
	{NULL, NULL, NULL},
};

int Shell_GetCmdCount(void)
{
	int i;

	for(i = 0; i < SHELL_CMD_HIDEIDX; i ++) {
		if(Shell_CMDs[i].cmd_str == 0) break;
	}

	return i;
}

int Shell_AddCMD(char *cmd, char *help, char (*func)(char **, int))
{
	int i = 0;

	if(Shell_Total_cmd < SHELL_CMD_HIDEIDX) {
		while((Shell_CMDs[i].cmd_str != 0) && (i <= Shell_Total_cmd /*SHELL_CMD_HIDEIDX*/)) {
			if(!shell_strcmp(cmd, (char*)Shell_CMDs[i].cmd_str)) {
				return SHELLERR_SUCCESS;
			}
			i ++;
		}
		
		Shell_CMDs[i+1].cmd_str = Shell_CMDs[i].cmd_str;
		Shell_CMDs[i+1].cmd_info = Shell_CMDs[i].cmd_info;
		Shell_CMDs[i+1].cmd_func = Shell_CMDs[i].cmd_func;

		Shell_CMDs[i].cmd_str = cmd;
		Shell_CMDs[i].cmd_info = help;
		Shell_CMDs[i].cmd_func = func;
		Shell_Total_cmd ++;
	}

	return SHELLERR_SUCCESS;
}

char Shell_Exit(char **argv, int argc)
{
	Shell_State = Shell_State_Stopped;
	Shell_PrintLine(Shell_ByeStr);
#ifdef SHELL_LOGIN_SUPPORT
	Shell_Login_Flag = 0;
	Shell_Set_Attr(Shell_NoEcho | Shell_AutoEnter);
#endif // SHELL_LOGIN_SUPPORT

	return SHELLERR_SUCCESS;
}

char Shell_Clear(char **argv, int argc)
{
	 int i = Shell_Height;

	while(i--)
	{
		Shell_PrintLine(" ");
	}

	return SHELLERR_SUCCESS;
}

char Shell_Help(char **argv, int argc)
{
	int i = -1;

	Shell_PrintLine(Shell_StrInfo);
	Shell_PrintLine("commands:");
	while((Shell_CMDs[++i].cmd_str != 0) && (i <= Shell_Total_cmd /*SHELL_CMD_HIDEIDX*/)) {
		Shell_PrintChar('\t');
		Shell_PrintStr(Shell_CMDs[i].cmd_str);
		Shell_PrintStr(" -- ");
		Shell_PrintLine(Shell_CMDs[i].cmd_info);
	}

	return SHELLERR_SUCCESS;
}

char Shell_Ver(char **argv, int argc)
{
	Shell_PrintLine(Shell_StrVer);

	return SHELLERR_SUCCESS;
}

static int Shell_ParsePara(char **para, char *cmd)
{
	int c = 0, foundpara = 0, paranum = 0;

	do {
		if(cmd[c] == '\0') {
			if(foundpara) {
				if(para != NULL)
					para[paranum] = &cmd[foundpara - 1];
				paranum++;
			}
			break;
		}
		else if(cmd[c] == ' ') {
			if(foundpara) {
				if(para != NULL) {
					cmd[c] = '\0';
					para[paranum] = &cmd[foundpara - 1];
				}
				paranum++;
				foundpara = 0;
			}
		}
		else {
			if(!foundpara)
				foundpara = c + 1;
		}
		c++;
	} while(1);

	return paranum;
}

#ifndef SHELL_MALLOC_SUPPORT
char *para[SHELL_PARA_NUM];
#endif // SHELL_MALLOC_SUPPORT

static int Shell_DoCMD(char *CMD)
{
	int i = -1;
	int para_num = 0;

#ifdef SHELL_MALLOC_SUPPORT
	char **para = NULL;

	para_num = Shell_ParsePara(NULL, CMD);
	para = (char**)malloc(para_num * sizeof(char*));
#endif

	para_num = Shell_ParsePara(para, CMD);

	if(para_num == 0)
		return SHELLERR_CMD_NOT_FOUND;

#ifdef SHELL_LOGIN_SUPPORT
	if(!shell_Login_Status()) {
		if(!shell_strcmp((char*)para[0], (char*)USR_PASSWORD)) {
			shell_Login_in();
			Shell_Set_Attr(Shell_RL_Attr);
			Shell_Buff_init();
			return SHELLERR_SUCCESS;
		}
		return SHELLERR_CMD_NOT_FOUND;
	}
#endif // SHELL_LOGIN_SUPPORT

	while((Shell_CMDs[++i].cmd_str != 0) && (i < Shell_Total_cmd /*SHELL_CMD_HIDEIDX*/)) {
		if(!shell_strcmp((char*)para[0], (char*)Shell_CMDs[i].cmd_str)) {
			return Shell_CMDs[i].cmd_func(para, para_num);
		}
	}

#ifdef SHELL_CMD_ECHO_SUPPORT
	Shell_PrintStr("COMMAND:\"");
	Shell_PrintStr(para[0]);
	Shell_PrintLine("\" not found");
#endif // SHELL_CMD_ECHO_SUPPORT

#ifdef SHELL_MALLOC_SUPPORT
	free(para);
	para = NULL;
#endif // SHELL_MALLOC_SUPPORT

	return SHELLERR_CMD_NOT_FOUND;
}


#ifdef SHELL_COLOR_SUPPORT
void Shell_SetTxtAttr(char FGColor,char BGColor,char Attrs)
{
	Shell_WriteESC();
	if(FGColor != -1)
	{
		Shell_WriteFG(FGColor);
	}
	if(BGColor != -1)
	{
		Shell_WriteSpacer();
		Shell_WriteBG(BGColor);
	}
	if(Attrs == Shell_Attrs_None)
	{
		Shell_WriteSpacer();
		Shell_WriteAttr(Shell_Attr_None);
	}
	else
	{
		if(Attrs & Shell_Attrs_HighLight)
		{
			Shell_WriteSpacer();
			Shell_WriteAttr(Shell_Attr_HighLight);
		}
		if(Attrs & Shell_Attrs_UnderLine)
		{
			Shell_WriteSpacer();
			Shell_WriteAttr(Shell_Attr_UnderLine);
		}
		if(Attrs & Shell_Attrs_Blink)
		{
			Shell_WriteSpacer();
			Shell_WriteAttr(Shell_Attr_Blink);
		}
		if(Attrs & Shell_Attrs_Inverse)
		{
			Shell_WriteSpacer();
			Shell_WriteAttr(Shell_Attr_Inverse);
		}
		if(Attrs & Shell_Attrs_Disappear)
		{
			Shell_WriteSpacer();
			Shell_WriteAttr(Shell_Attr_Disappear);
		}
	}

	Shell_WriteESCEnd();
}

/*	Usage
 *	Press:	'Esc'(\e) and '[' as ESC
 *			'3' + COLOR as FGColor(COLOR is from '0' to '7')
 *			'4' + COLOR ad BGColor
 *			'0' or '1' as Font
 *			';' as spacer
 *			'm' as end
 *	Exg:'Esc'+'['+'3'+'1'+';'+'4'+'0'+';'+'1'+'m'("\e[31;40;1m")
 */
/*
char Shell_Color(char **argv, int argc)
{
	char ch = 0;

	Shell_PrintStr("God bless U know how 2 use...(\'q\' to quit),Input:");
	while((ch = Shell_ReadChar()) != 'q')
	{
		Shell_PrintChar(ch);
	}

	Shell_Clear();
	Shell_Clear();

	return SHELLERR_SUCCESS;
}
*/
#endif // SHELL_COLOR_SUPPORT


