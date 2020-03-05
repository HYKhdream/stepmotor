/**
  *****************************************************************************
  * @file    hal_E534.h
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2016-08-16
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/

#ifndef __HAL_E534_H__
#define __HAL_E534_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/

extern void hal_E534_init(void);

extern void hal_E534_poll(void);

extern void hal_E534_process(void);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_E534_H__ */

