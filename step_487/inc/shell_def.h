/*
 * shell_def.h	(by xhl 2014/11/01)
 */

#ifdef __CYGWIN__
	#define SHELL_CTRL_CHAR_NL	0x0D/*'\r'*/
	#define SHELL_CTRL_CHAR_RT	0x0A/*'\n'*/
	#define SHELL_CTRL_CHAR_BP	0x7F/*'\b'*/
#else
	#define SHELL_CTRL_CHAR_NL	0x0A/*'\n'*/
	#define SHELL_CTRL_CHAR_RT	0x0D/*'\r'*/
	#define SHELL_CTRL_CHAR_BP	0x08/*'\b'*/
#endif

#define Shell_StartStr			"#"
#define Shell_EndLineStr		"\r\n"
#define Shell_StrVer			"2.0Beta"
#define Shell_StrInfo			"Shell V2.0Beta by Hailong Xu"
#define Shell_ByeStr			"Shell:Bye!"


// Shell_State definations
#define Shell_State_Running				(1 << 0)
#define Shell_State_Busy				(1 << 1)
#define Shell_State_Paused				(1 << 4)
#define Shell_State_Stopped				(1 << 7)

#define SHELLERR_CMD_NOT_FOUND		1
#define SHELLERR_SUCCESS		0

#define Shell_NoEcho	0
#define Shell_EchoChar	0xFF
#define Shell_Echo	(1 << 8)
#define Shell_AutoEnter	(1 << 9)
#define Shell_Echo_Pswd	(1 << 10)

#ifdef SHELL_CMD_ECHO_SUPPORT
  #define Shell_RL_Attr	(Shell_Echo | Shell_AutoEnter)
#else
  #define Shell_RL_Attr	Shell_NoEcho
#endif

#ifdef SHELL_COLOR_SUPPORT
#define Shell_DefFGColor				Shell_Color_Blue
#define Shell_DefBGColor				Shell_Color_White
#define Shell_DefAttr					(Shell_Attrs_HighLight)

#define Shell_Attrs_None				(0)
#define Shell_Attrs_HighLight				(1 << 0)
#define Shell_Attrs_UnderLine				(1 << 3)
#define Shell_Attrs_Blink				(1 << 4)
#define Shell_Attrs_Inverse				(1 << 6)
#define Shell_Attrs_Disappear				(1 << 7)

#define Shell_Attr_None					'0'
#define Shell_Attr_HighLight				'1'
#define Shell_Attr_UnderLine				'4'
#define Shell_Attr_Blink				'5'
#define Shell_Attr_Inverse				'7'
#define Shell_Attr_Disappear				'8'

#define Shell_ESC					0x1B
#define Shell_Spacer					';'
#define Shell_Color_FG					'3'
#define Shell_Color_BG					'4'
#define Shell_Color_Black				'0'
#define Shell_Color_Red					'1'
#define Shell_Color_Green				'2'
#define Shell_Color_Yellow				'3'
#define Shell_Color_Blue				'4'
#define Shell_Color_Magenta				'5'
#define Shell_Color_Cyan				'6'
#define Shell_Color_White				'7'
#define Shell_WriteESC()				(Shell_PrintChar(Shell_ESC),Shell_PrintChar('['))		// Shell 写入转义字符
#define Shell_WriteFG(COLOR)				(Shell_PrintChar(Shell_Color_FG),Shell_PrintChar((COLOR)))
#define Shell_WriteSpacer()				(Shell_PrintChar(Shell_Spacer))
#define Shell_WriteBG(COLOR)				(Shell_PrintChar(Shell_Color_BG),Shell_PrintChar((COLOR)))
#define Shell_WriteAttr(n)				(Shell_PrintChar((n)))
#define Shell_WriteESCEnd()				(Shell_PrintChar('m'))
#endif // SHELL_COLOR_SUPPORT


#define COUNTOF(__S)	(sizeof(__S) / sizeof(__S[0]))

typedef struct {
	unsigned int sidx;	// start
	unsigned int eidx;	// end
}SHELL_HISTORY;

typedef struct {
	char	*name;
	char	count;
	char	keys[5];
	void	(*prog)(void);
} CTRL_KEY_DEF;

typedef struct {
	char *cmd_str;		// command string
	char (*cmd_func)(char **argv, int argc);
	char *cmd_info;
}CMD_DEF;

