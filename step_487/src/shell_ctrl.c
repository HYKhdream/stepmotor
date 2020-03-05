/*
 * shell_ctrl.c	(by xhl 2014/11/01)
 */
#include "shell.h"

void hook_left_key(void);
void hook_up_key(void);
void hook_down_key(void);
int shell_Return(void);
int shell_BackSpace(void);
int shell_Login_Status(void);

CTRL_KEY_DEF CtrlKeys[] = {
#ifdef SHELL_CMD_ECHO_SUPPORT
	{"LEFT",   3, {27, 91, 68}, &hook_left_key},
	{"RIGHT",  3, {27, 91, 67}, NULL},
	{"UP",     3, {27, 91, 65}, &hook_up_key},
	{"DOWN",   3, {27, 91, 66}, &hook_down_key},
#endif // SHELL_CMD_ECHO_SUPPORT

	{"F1",     3, {27, 79, 80}, NULL},
	{"F2",     3, {27, 79, 81}, NULL},
	{"F3",     3, {27, 79, 82}, NULL},
	{"F4",     3, {27, 79, 83}, NULL},

	{"F5",     5, {27, 91, 49, 53, 126}, NULL},
	{"F6",     5, {27, 91, 49, 55, 126}, NULL},
	{"F7",     5, {27, 91, 49, 56, 126}, NULL},
	{"F8",     5, {27, 91, 49, 57, 126}, NULL},

	{"F9",     5, {27, 91, 50, 48, 126}, NULL},
	{"F10",    5, {27, 91, 50, 49, 126}, NULL},
	{"F11",    5, {27, 91, 50, 51, 126}, NULL},
	{"F12",    5, {27, 91, 50, 52, 126}, NULL},

	{"INSERT", 4, {27, 91, 50, 126}, NULL},
	{"DELETE", 4, {27, 91, 51, 126}, NULL},

	{"PAGEUP", 4, {27, 91, 53, 126}, NULL},
	{"PAGEDN", 4, {27, 91, 54, 126}, NULL},

	{"HOME",   3, {27, 91, 72}, NULL},
	{"END",    3, {27, 91, 70}, NULL},
};

int ctrl_key_count = 0;
char ctrl_key_buf[5];

int shell_is_ctrl_key(char ch, int time)
{
	int i;
	int is_ctrl_key = 0;

	if(ctrl_key_count == 0 || time > 5) {
		switch(ch) {
		case SHELL_CTRL_CHAR_NL:		// NewLine
			break;
		case SHELL_CTRL_CHAR_RT:		// Return
			shell_Return();
		#ifdef __CYGWIN__
			Shell_Poll();
		#endif
			break;
		case SHELL_CTRL_CHAR_BP:		// Backspace
			shell_BackSpace();
			break;
		case 27:
			ctrl_key_buf[0] = ch;
			ctrl_key_count = 1;
			break;
		default:
			goto done;
		}
		is_ctrl_key = 1;
		goto done;
	}
	else if(time < 5/*ms*/) {
		is_ctrl_key = 1;

		ctrl_key_buf[ctrl_key_count] = ch;
		ctrl_key_count++;
		for(i = 0; i < COUNTOF(CtrlKeys); i++) {
			if(CtrlKeys[i].count != ctrl_key_count)
				continue;
			if(memcmp(ctrl_key_buf, CtrlKeys[i].keys, ctrl_key_count) == 0) {
				//printf("%s\n", CtrlKeys[i].name);
				ctrl_key_count = 0;
			#ifdef SHELL_LOGIN_SUPPORT
				if(!shell_Login_Status())
					goto done;
			#endif // SHELL_LOGIN_SUPPORT
				if(CtrlKeys[i].prog) {
					(*CtrlKeys[i].prog)();
				}
				goto done;
			}
		}
	#if 0
		if(ctrl_key_count >= 5) {
			printf("\nunknow key %d, %d, %d, %d, %d\n",
				ctrl_key_buf[0], ctrl_key_buf[1],
				ctrl_key_buf[2], ctrl_key_buf[3],
				ctrl_key_buf[4]);
		}
	#endif
	}
done:
	return is_ctrl_key;
}

