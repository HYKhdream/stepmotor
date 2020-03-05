/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void arch_delay_ms(int _ms);
int RA8875_TouchPress(void);
int RA8875_TouchReadXY(int *_usX, int *_usY);
#define MAX_CNT     10
#define TOUCH_FILTER_MAX	30
#define TOUCH_DAT_OFS     (MAX_CNT >> 1)

#define TOUCHPAD_MIN_X	50
#define TOUCHPAD_MAX_X	970
#define TOUCHPAD_MIN_Y	100
#define TOUCHPAD_MAX_Y	940

#define LCD_WIDTH	480
#define LCD_HEIGHT	272

int Touch_delay = 0;
int Touch_ticks = 0;
int Touch_delay_setup = 10;
int Touch_x = 0, Touch_y = 0;
int Touch_press = 0;
int Touch_Px, Touch_Py;

int (*touch_event)(int x, int y, int event) = NULL;

int arch_touch_callback_event(int (*hook)(int x, int y, int event))
{
	touch_event = hook;
	return 0;
}

int arch_touch_d2p(int *_px, int *_py)
{
	if(*_px < TOUCHPAD_MIN_X)
		*_px = TOUCHPAD_MIN_X;
	if(*_px > TOUCHPAD_MAX_X)
		*_px = TOUCHPAD_MAX_X;

	*_px = (*_px - TOUCHPAD_MIN_X) * LCD_WIDTH / (TOUCHPAD_MAX_X - TOUCHPAD_MIN_X);

	if(*_py < TOUCHPAD_MIN_Y)
		*_py = TOUCHPAD_MIN_Y;
	if(*_py > TOUCHPAD_MAX_Y)
		*_py = TOUCHPAD_MAX_Y;

	*_py = (*_py - TOUCHPAD_MIN_Y) * LCD_HEIGHT / (TOUCHPAD_MAX_Y - TOUCHPAD_MIN_Y);
	
	return 0;
}

#define __SWAP(_a, _b)	\
	do {int __s_ab; \
		__s_ab = _a; \
		_a = _b; \
		_b = __s_ab; \
	}while(0)

static void touch_data_insert(int dat, int *buf, int top)
{
	int j;

	buf[top] = dat;

	for(j = 0; j < top; j++) {
		if(dat >= buf[j]) {
			continue;
		}
		for(; j <= top; j++)
		__SWAP(dat, buf[j]);
		break;
	}
}

int arch_touch_read(int *_usX, int *_usY)
{
	int touch_x[MAX_CNT], touch_y[MAX_CNT];
	int ret;
	int t_x, t_y;
	int i;

	for(i = 0; i < MAX_CNT; i++) {
		ret = RA8875_TouchReadXY(&t_x, &t_y);
		if(ret <= 0) {
			return 0;
		}
		arch_delay_ms(1);

		touch_data_insert(t_x, touch_x, i);
		touch_data_insert(t_y, touch_y, i);
	}

	if(touch_x[TOUCH_DAT_OFS + 1] - touch_x[TOUCH_DAT_OFS] > TOUCH_FILTER_MAX) {
		return -1;
	}
	if(touch_y[TOUCH_DAT_OFS + 1] - touch_y[TOUCH_DAT_OFS] > TOUCH_FILTER_MAX) {
		return -1;
	}

	t_x = 0;
	t_y = 0;
	for(i = 0; i < 2; i++) {
		t_x += touch_x[TOUCH_DAT_OFS + i];
		t_y += touch_y[TOUCH_DAT_OFS + i];
	}
	t_x >>= 1;
	t_y >>= 1;

	*_usX = t_x;
	*_usY = t_y;

	return 1;
}

#if 0
int arch_touch_get_xy(int *_usX, int *_usY, int mode)
{
	int press;
	int arch_touch_read(int *_usX, int *_usY);
	int t_x, t_y;

	press = arch_touch_read(&t_x, &t_y);
	if(press == 0) {
		Touch_delay = 0;
		Touch_press = 0;
		if(Touch_ticks > 5000)
		RA8875_ClrScr(0x0000);
		return -1;
	}

	if(!mode)
		arch_touch_d2p(&t_x, &t_y);

	if(Touch_delay > 0) {
		if(_usX)
			*_usX = t_x;
		if(_usY)
			*_usY = t_y;
		return 0;
	}
	Touch_delay = Touch_delay_setup;


	if(Touch_press == 0) {
		Touch_ticks = 0;
		Touch_Px = t_x;
		Touch_Py = t_y;
	}
	else {
		char str[32];
		sprintf(str, "move [%d, %d]    ", t_x - Touch_Px, t_y - Touch_Py);
		RA8875_DispAscii(10, 60, str);
		//RA8875_PutPixel(t_x, t_y, 0xFFF);
		RA8875_DrawLine(Touch_x, Touch_y, t_x, t_y, 0xFFF, 0);
	}
	Touch_press = 1;

	Touch_x = t_x;
	Touch_y = t_y;

	if(_usX)
		*_usX = t_x;
	if(_usY)
		*_usY = t_y;

	return 1;
}
#endif

void arch_touch_timer()
{
	if(Touch_delay > 0) Touch_delay--;
	if(Touch_press) Touch_ticks++;
}

int touchctl_HotArea_Machit(int, int, int);
int arch_touch_proc(int x, int y, int event)
{
	if(event == 0) {
		if(Touch_press) // release
			touchctl_HotArea_Machit(Touch_x, Touch_y, 0);
		Touch_delay = 0;
		Touch_press = 0;
		return -1;
	}

	arch_touch_d2p(&x, &y);

	if(Touch_delay > 0) {
		return 0;
	}
	Touch_delay = Touch_delay_setup;


	if(Touch_press == 0) { // press
		touchctl_HotArea_Machit(x, y, 1);
		Touch_ticks = 0;
		Touch_Px = x;
		Touch_Py = y;
	}
	else { // move
#if 0
		char str[32];
		sprintf(str, "move [%d, %d]    ", x - Touch_Px, y - Touch_Py);
		RA8875_DispAscii(10, 60, str);
		RA8875_PutPixel(x, y, 0xFFF);
		RA8875_DrawLine(Touch_x, Touch_y, x, y, 0xFFF, 0);
#endif
#if 0
		int diff_x = abs(x - Touch_Px) >> 3;
		int diff_y = abs(y - Touch_Py) >> 3;
		RA8875_PutPixel(x, y, 0xFFF);
		RA8875_SetFrontColor(0xFFF);
		if(diff_x) {
			char str[32];
			sprintf(str, "x move %d  ", x - Touch_Px);
			RA8875_DispAscii(10, 60, str);
			Touch_Px = x;
		}
		if(diff_y) {
			char str[32];
			sprintf(str, "y move %d  ", y - Touch_Py);
			RA8875_DispAscii(10, 80, str);
			Touch_Py = y;
		}
#endif
	}
	Touch_press = 1;

	Touch_x = x;
	Touch_y = y;


	return 1;
}

void arch_touch_detect()
{
	int arch_touch_read(int *_usX, int *_usY);
	int event;
	int t_x, t_y;

	event = arch_touch_read(&t_x, &t_y);
	if(event < 0)
		return;

	arch_touch_proc(t_x, t_y, event);
	if(touch_event) {
		(*touch_event)(t_x, t_y, event);
	}
}

