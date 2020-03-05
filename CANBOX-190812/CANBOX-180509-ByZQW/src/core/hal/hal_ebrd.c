/**
  *****************************************************************************
  * @file    hal_ebrd.c
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2016-08-18
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/
  
/* Includes ------------------------------------------------------------------*/
#include "hal_ebrd.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void hal_ebrd_init(int whichboard)
{
	int ret;
/*EBRD_E534 = 1,
	EBRD_E459,
	EBRD_E481,
	EBRD_E644,*/
	switch (whichboard)
	{
		case EBRD_E459:
			ret = hal_e459_check();
			break;
		case EBRD_E534:
			ret = hal_e534_check();
			break;
		case EBRD_E481:
			ret = hal_e481_check();
			break;
		case EBRD_E644:	
			ret = hal_e644_check();
			break;
		case EBRD_E697:	
			ret = hal_e697_check();
			break;
				
	}
	
	
	
	
}
