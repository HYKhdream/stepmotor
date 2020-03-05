/*
 * shell_cfg.h	(by xhl 2014/11/01)
 */


// when receive data from interface, will Shell echo?
#define SHELL_CMD_ECHO_SUPPORT		1

////#define SHELL_LOGIN_SUPPORT		1

////#define SHELL_MALLOC_SUPPORT		1

////#define SHELL_COLOR_SUPPORT		1

#define SHELL_BUFFER_SIZE	(1024L * 2L)

#define SHELL_MAX_LIST		50

#define SHELL_LINE_BUFFER_SIZE	512

#define SHELL_PARA_NUM		100

#define SHELL_CMD_HIDEIDX	30

#define Shell_Height		30

#define shell_strcmp(s0, s1)			strcmp(s0, s1)

#define USR_PASSWORD				"test"

