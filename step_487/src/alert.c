#include "massage.h"
#include "alert.h"
#include "config.h"

typedef struct {
	int alert_code;
	int alert_arg;
}ALERT_TYPE;

#define ALERT_MAX	30

ALERT_TYPE alertbuff[ALERT_MAX];

volatile unsigned int alert_rptr, alert_wptr;
volatile unsigned int alertcode;
volatile unsigned int alertarg;
// by xhl 2010/06/13
volatile unsigned int alert_delay;
volatile unsigned int alert_disable;

void alert_init()
{
	alert_rptr = 0;
	alert_wptr = 0;
	alertcode = 0;

	// by xhl 2010/06/13
	//alert_delay = 500 * 30;
	// by xhl 2011/01/13
	alert_delay = 500;
	alert_disable = 0;
}

int alert_count()
{
	int count;
	count = ALERT_MAX + alert_wptr - alert_rptr;
	if(count >= ALERT_MAX)
		count -= ALERT_MAX;

	return count;
}

int alert_find(int alert_code, int alert_arg);
int alert_push(int alert_code, int alert_arg)
{
	ALERT_TYPE *Alert;
	
#ifdef FEET_SUPPORT
		extern volatile int feet_alarm_enable;
//		extern volatile int testalarmcount;
	
		if (!feet_alarm_enable)
		{
			if ((alert_code >= 0xC5) && (alert_code <=0xC8))
				return 0;
		}
#endif

	if((alert_code == alertcode) &&
	   (alert_arg == alertarg)) {
		return 0;
	}
	if(alert_count() == ALERT_MAX - 1) {
		return -1;
	}

//	if(alert_find(alert_code, alert_arg) == 0)
	{

		Alert = &alertbuff[alert_wptr];

		Alert->alert_code = alert_code;
		Alert->alert_arg = alert_arg;
		alert_wptr ++;
		if(alert_wptr >= ALERT_MAX) {
			alert_wptr -= ALERT_MAX;
		}
	}

	return 1;
}

int alert_pop(int *alert_code, int *alert_arg)
{
	ALERT_TYPE *Alert;

	*alert_code = 0;
find_next:
	if(alert_count() == 0) {
		return 0;
	}

	Alert = &alertbuff[alert_rptr];

	*alert_code = Alert->alert_code;
	*alert_arg = Alert->alert_arg;

	alert_rptr ++;
	if(alert_rptr >= ALERT_MAX) {
		alert_rptr -= ALERT_MAX;
	}

	if(Alert->alert_code == 0) {
		goto find_next;
	}

	return 1;
}

int alert_find(int alert_code, int alert_arg)
{
	int i, idx;
	int count = alert_count();

	idx = alert_rptr;
	for(i = 0, idx = alert_rptr; i < count; i ++, idx ++) {
		if(idx >= ALERT_MAX) {
			idx -= ALERT_MAX;
		}
		if((alertbuff[idx].alert_code == alert_code) &&
		   (alertbuff[idx].alert_arg == alert_arg)) {
			return 1;
		}
	}
	return 0;
}

int alert_delete(int alert_code, int alert_arg)
{
	int i;

	for(i = 0; i < ALERT_MAX; i ++) {
		if((alertbuff[i].alert_code == alert_code) &&
		   (alertbuff[i].alert_arg == alert_arg)) {
			alertbuff[i].alert_code = 0;
		}
	}

	return 0;
}

void Alert_Clear()
{
	if(alertcode) {
		alert_delete(alertcode, alertarg);
		alertcode = 0;
	}
}

unsigned int Scan_error(void);
void Alert_Poll()
{
	unsigned int ret;
	extern volatile unsigned int Main_sysisok;
	//unsigned int alert_arg;
//20151231  
	if (!Main_sysisok) 
		return;
	
	// by xhl 2010/06/13
	if(alert_disable)
		return ;

	if(alertcode) 
		return ;

	ret = alert_pop((int *)&alertcode, (int *)&alertarg);
	if(ret) {
//		if(alertcode != LINE_ERR)
		Massage_Send_Alert(alertcode, alertarg);
		alert_delete(alertcode, alertarg);
	}

	ret = 0; //Scan_error();
	if(ret) {
		if(alert_find(ret, 0) == 0) {
			alert_push(ret, 0);
		}
	}
}

// by xhl 2010/06/13
void alert_time_poll()
{
	if(alert_delay) {
		alert_delay --;
		if(alert_delay == 0) {
			// enable alert
			alert_disable = 0;
		}
	}
}

void alert_cpu_fatal()
{
	Massage_Send_Alert(DSP_FATAL_ERR, 0x0);
}
