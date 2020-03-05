/**
  *****************************************************************************
  * @file    qwl_shell.c
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2017-04-11
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/
  
/* Includes ------------------------------------------------------------------*/
#include "qwl_shell.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
	DBGU_MODE_KEY,
	DBGU_MODE_CMD,
}DBGU_MODE_t;

typedef enum {
	DBGU_LOGIN_NULL = 0,
	DBGU_LOGIN_VERIFY,	
	DBGU_LOGIN_USER,
	DBGU_LOGIN_ROOT,
}DBGU_LOGIN_t;

#define INPUT_BIT_SIZE			8

/* Private define ------------------------------------------------------------*/

#define DEV_NAME		"QWL"

#define USER_NAME		"user"
#define USER_PASSWORD	"user"

#define SUDO_NAME		"root"
#define SUDO_PASSWORD	"root"

#define DBGU_STRBUF_SIZE			0x3F						/* 指令缓冲最大长度 */

/* Private macro -------------------------------------------------------------*/

#define SH_GETCHAR(ch)				arch_uart_getc(ch)			/* 读取一个字符 */
#define SH_PUTCHAR(ch)				arch_uart_putc(ch)			/* 输出一个字符 */
#define SH_PRINTF(...)				printf(__VA_ARGS__)
#define SU_PRINTF(...)				printf(__VA_ARGS__)

#define SH_ATOI(a)					atoi(a)

/* Private variables ---------------------------------------------------------*/

static uint8_t dbgu_init_flag = 0;
static DBGU_LOGIN_t dbgu_login = DBGU_LOGIN_NULL;
static DBGU_MODE_t dbgu_mode = DBGU_MODE_KEY;

static unsigned char dbgu_ch_cnt = 0;
static unsigned char dbgu_str_buf[DBGU_STRBUF_SIZE+1];
static unsigned char dbgu_str_prev[DBGU_STRBUF_SIZE+1];
static unsigned int dbgu_idx, dbgu_val;
static unsigned char dbgu_key_enable = 1;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static int qwl_shell_pick(char *str)
{
	int cnt = 0;
	char *ptr[6], *buf = str;
	
	while (ptr[cnt] == strtok(buf, " "))/* 提取每个参数 */
	{
		if (++cnt > 4)
			return 0;
		
		buf = NULL;
	}
	SH_PRINTF("%d ", cnt);
	return cnt;
}

/* Public functions ----------------------------------------------------------*/

static void qwl_shell_parse_user(unsigned char **argv, unsigned char argc)
{
	int para_d[4];
	unsigned char **argv_i = argv;
	unsigned char *pfname = argv[0];

	argc--;

	if (strcmp(pfname, "setqep") == 0)
	{
		SH_ATOI(argv[1]);
	}
}

static void qwl_shell_parse_su(unsigned char **argv, unsigned char argc)
{
	char *pfname = argv[0];
	
	if (strcmp(pfname, "bulid") == 0)
	{
		SU_PRINTF("bulid time: %s %s\r\n", __DATE__, __TIME__);
	}
	else if (strcmp(pfname, "reset") == 0)
	{
		SU_PRINTF("Reset Now\r\n");
		SysCtlReset();
	}
}

/*----------------------------------------------------------------------------*/

static void qwl_shell_key(char ch)
{
	static uint32_t val = 0;

	if (!dbgu_key_enable)
		return;
	
	switch (ch)
	{
		case ']': dbgu_idx += 2;
		case '[': dbgu_idx--;
			SH_PRINTF("[%d]",dbgu_idx);
			break;
		case '\\': dbgu_idx = 0;
			SH_PRINTF("[%d]",dbgu_idx);
			break;
		case '}': dbgu_idx += 100;
		case '{': dbgu_idx -= 50;
			SH_PRINTF("[%d]",dbgu_idx);
			break;
		//
		case '.': dbgu_val += 2;
		case ',': dbgu_val--;
			SH_PRINTF("(%d)",dbgu_val);
			break;
		case '/': dbgu_val = 0;
			SH_PRINTF("(%d)",dbgu_val);
			break;
		case '>': dbgu_val += 100;
		case '<': dbgu_val -= 50;
			SH_PRINTF("(%d)",dbgu_val);
			break;

		case ';':{
			val = val ? 0 : 1;
			SH_PRINTF("<%d>",val);
		}break;

		case 'A':break;
		case 'B':break;
		case 'C':break;
		case 'D':break;
		case 'E':break;
		case 'F':break;
		case 'O':break;
		case 'P':break;
		////////////////////////////////////////////////////////
		//SH_PRINTF("(%d %d)\r\n", dbgu_idx, dbgu_val);
		
		case 'a':{
			can_export_uart(dbgu_idx, dbgu_val);
		}break;
		case 'b':break;
		case 'c':break;
		case 'd':break;
		case 'e':break;
		case 'f':break;
		case 'g':break;
		case 'h':break;
		case 'i':break;
		case 'j':break;
		case 'k':break;
		case 'l':break;
		case 'm':break;
		case 'n':break;
		case 'o':break;
		case 'p':break;
		case 'r':break;
		case 's':break;
		case 't':break;
		case 'u':break;
		case 'v':break;
		case 'w':break;
		case 'x':break;
		case 'y':break;
		case 'z':break;
		//case '':break;
		default:break;
	}
}

static void qwl_shell_cmd(char ch)
{
	static unsigned char prev_cnt = 0;
	unsigned char *ptr[10], *buf;
	unsigned char **argv, argc = 0;
	
	if (ch == '\r')
	{
		SH_PUTCHAR('\r');
		SH_PUTCHAR('\n');
		dbgu_str_buf[dbgu_ch_cnt] = '\0';
		buf = dbgu_str_buf;
		
		memcpy((void *)dbgu_str_prev, (void *)dbgu_str_buf, dbgu_ch_cnt);
		prev_cnt = dbgu_ch_cnt;
		
		while (ptr[argc] = strtok(buf, " "))/* 提取每个参数 */
		{
			if (++argc > 6)
			{
				argc = 0;
				break;
			}
			buf = NULL;
		}
		
		if (argc)
		{
			argv = &ptr[0];
			if (dbgu_login == DBGU_LOGIN_USER)
			{
				qwl_shell_parse_user(argv, argc);
				SH_PRINTF("%s@%s: ", USER_NAME, DEV_NAME);
			}
			else if (dbgu_login == DBGU_LOGIN_ROOT)
			{
				qwl_shell_parse_su(argv, argc);
				SH_PRINTF("%s@%s: ", SUDO_NAME, DEV_NAME);
			}
		}
		dbgu_ch_cnt = 0;
	}
	else if(ch == '\b')
	{
	  	if (dbgu_ch_cnt)
		{
			SH_PUTCHAR('\b');
			SH_PUTCHAR(' ');
			SH_PUTCHAR('\b');
			dbgu_ch_cnt--;
		}
	}
	else if (ch == '`' && dbgu_ch_cnt == 0)	//上一次输入命令还原
	{
		if (prev_cnt)
		{
			dbgu_ch_cnt = prev_cnt;
			memcpy((void *)dbgu_str_buf, (void *)dbgu_str_prev, dbgu_ch_cnt);
			SH_PRINTF("%s", dbgu_str_buf);
		}
	}
	else if (ch >= ' ')	//可见字符
	{
	  	SH_PUTCHAR(ch);
		dbgu_str_buf[dbgu_ch_cnt] = ch;
		dbgu_ch_cnt++;
		dbgu_ch_cnt &= DBGU_STRBUF_SIZE;
	}
}

static void qwl_shell_login(char ch)
{
	if (ch == '\r')
	{
		if (dbgu_login == DBGU_LOGIN_NULL)
		{
			dbgu_str_buf[dbgu_ch_cnt] = ' ';
			dbgu_login = DBGU_LOGIN_VERIFY;
			SH_PRINTF("\r\nPassword: ");
			dbgu_ch_cnt++;
			dbgu_ch_cnt &= DBGU_STRBUF_SIZE;
		}
		else
		{
			char name[16], pass[16];
			
			dbgu_str_buf[dbgu_ch_cnt] = '\0';
			sscanf(dbgu_str_buf, "%s %s", name, pass);
			
			if (strcmp(name, USER_NAME) == 0 
				&& strcmp(pass, USER_PASSWORD) == 0)
			{
				dbgu_login = DBGU_LOGIN_USER;
				SH_PRINTF("\r\n================================");
				SH_PRINTF("\r\n==\t\t");
				SH_PRINTF("\r\n================================");
				SH_PRINTF("\r\n\r\n");
				SH_PRINTF("\r\n%s@%s: ", USER_NAME, DEV_NAME);
			}
			else if (strcmp(name, SUDO_NAME) == 0 
				&& strcmp(pass, SUDO_PASSWORD) == 0)
			{
				dbgu_login = DBGU_LOGIN_ROOT;
				SH_PRINTF("\r\n================================");
				SH_PRINTF("\r\n==\t\t");
				SH_PRINTF("\r\n================================");
				SH_PRINTF("\r\n\r\n");
				SH_PRINTF("\r\n%s@%s: ", SUDO_NAME, DEV_NAME);
			}
			else
			{
				dbgu_login = DBGU_LOGIN_NULL;
				SH_PRINTF("\r\nLogin Error!\r\nLogin name: ");
			}
			dbgu_ch_cnt = 0;
		}
	}
	else if (ch == '\b')
	{
		if (dbgu_ch_cnt)
		{
			dbgu_ch_cnt--;
			SH_PUTCHAR('\b');
			SH_PUTCHAR(' ');
			SH_PUTCHAR('\b');
		}
	}
	else if (ch >= ' ')	//可见字符
	{
		if (dbgu_login == DBGU_LOGIN_VERIFY)
			SH_PUTCHAR('*');
		else
			SH_PUTCHAR(ch);
		
		dbgu_str_buf[dbgu_ch_cnt] = ch;
		dbgu_ch_cnt++;
		dbgu_ch_cnt &= DBGU_STRBUF_SIZE;
	}
}

/*----------------------------------------------------------------------------*/

void qwl_shell_poll(void)
{
  	char ch;

	if (dbgu_init_flag == 0)
	{
		dbgu_init_flag = 1;
		return;
	}

	if (!SH_GETCHAR(&ch))
	{
		//qwl_shell_loop();
		return;
	}
	
	if (dbgu_mode == DBGU_MODE_KEY)
	{
	  	if (ch == '`')
		{
		  	dbgu_mode = DBGU_MODE_CMD;
			SH_PRINTF("Login name: ");
			return;
		}
		qwl_shell_key(ch);
	}
	else
	{
		if (ch == 0x1B)		//ESC
		{
			SH_PRINTF("Login out!\r\n");
			SH_PUTCHAR(0x0C);
			SH_PUTCHAR(0x0C);
			
			dbgu_login = DBGU_LOGIN_NULL;
			dbgu_mode = DBGU_MODE_KEY;
			dbgu_ch_cnt = 0;
			return;
		}
		
		switch (dbgu_login)
		{
			case DBGU_LOGIN_NULL:
			case DBGU_LOGIN_VERIFY:
				qwl_shell_login(ch);
				break;
				
			case DBGU_LOGIN_USER:
			case DBGU_LOGIN_ROOT:
				qwl_shell_cmd(ch);
				break;
				
			default:
				dbgu_login = DBGU_LOGIN_NULL;
				break;
		}
	}
}

void qwl_shell_init(void)
{
	//qwl_dbgu_init();
	//printf("\r\nQWL Ver-0.0(%s)\r\n", __DATE__);
}


/*----------------------------------------------------------------------------*/

