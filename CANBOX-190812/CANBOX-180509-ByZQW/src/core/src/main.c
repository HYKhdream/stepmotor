#include "gui_ebrd.h"
#include "guilib.h"
#include "hal_ebrd.h"
#include "canfifo.h"

int arch_init(void);
void CWJ_Test(void);
void step_motor_fre_test();
void moto_set();
int main()
{
	can_buf_init();
	arch_init();
	can_fifo_para_load();
	pageid = PAGE_ID_STEPMOTOR_TEST;
	while (1)
	{
		switch(pageid)
		{
			case PAGE_ID_TEST_WEIJI:   //运行主页面
				CWJ_Test();
				break;
			
			case PAGE_ID_MOTO_SET: 			//电机设置页
				moto_set();
				break;
			case PAGE_ID_STEPMOTOR_TEST:
				step_motor_fre_test();
				break;
			
			default:
				break;
		}
		
	}
}

