/**
  *****************************************************************************
  * @file    qwl_types.h
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2017-03-25
  * @brief   
  *          
  *****************************************************************************
  * @note
  *****************************************************************************
**/

#ifndef __QWL_TYPES_H__
#define __QWL_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef WIN32
#include <stdbool.h>
#include <time.h>
//#include <arm_neon.h>
#endif

/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/

#if 0
typedef unsigned int 	uint32_t;
typedef int 			int32_t;
typedef unsigned short 	uint16_t;
typedef short 			int16_t;
typedef unsigned char 	uint8_t;
typedef char 			int8_t;


typedef uint8_t			u8,U8,uint8;
typedef int8_t	    	s8,S8,int8;

typedef uint16_t		u16,U16,uint16;
typedef int16_t			s16,S16,int16;

typedef uint32_t		u32,U32,uint32;
typedef int32_t			s32,S32,int32;

typedef unsigned char	uchar;
typedef unsigned int	ushort;
typedef unsigned long	ulong;
#endif
/*----------------------------------------------------------------------------*/

#ifndef NULL
#define NULL 			((void*)0)
#endif

#ifndef bool
#define bool			tBoolean
#endif

#ifndef TRUE
#define TRUE    		1
#define FALSE   		0
#endif

#ifndef WORD
#define WORD			int16_t
#endif
#ifndef DWORD
#define DWORD			int32_t
#endif

/* Public macro --------------------------------------------------------------*/

#ifndef COUNTOF
#define COUNTOF(a)		(sizeof(a) / sizeof(a[0]))
#endif

#ifndef MIN
#define MIN(a, b)		((a) > (b) ? (b) : (a))
#define MAX(a, b)		((a) < (b) ? (b) : (a))
#endif

/*----------------------------------------------------------------------------*/

#define BIT(n)				(1 << (n))
#define BitSET(REG,POS)		((REG) |= (1 << (POS)))
#define BitCLR(REG,POS)		((REG) &= (~(1 << (POS))))
#define BitXOR(REG,POS)		((REG) ^= (1 << (POS)))
#define BitCHK_1(REG,POS)	(((REG) & (1 << (POS))) == (1 << (POS)))
#define BitCHK_0(REG,POS)	(((REG) & (1 << (POS))) == 0x00)

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __QWL_TYPES_H__ */

