#ifndef INDEX_H
#define INDEX_H
#include "DSP2803x_device.h"

//structure to split the whole and fractional parts of current position
typedef struct
  {
    volatile int32 whole;
    volatile Uint16 frac;
  }POS;

/*-----------------------------------------------------------------------------
	Define the indexing states
-----------------------------------------------------------------------------*/
typedef enum 
 {
  IDLE,
  ACCEL,
  CONST_SPD,
  DECEL
  }IDX_STATES ;
/*-----------------------------------------------------------------------------
	Define the motor direction
-----------------------------------------------------------------------------*/
typedef enum  
  {
  DIR_CW,
  DIR_CCW
  } MOT_DIR;

/*-----------------------------------------------------------------------------
	Define the structure of the Index Object 
-----------------------------------------------------------------------------*/
typedef struct 
  {
  Uint32 total_dist;
  Uint32 accel_dist;		// distance to accel
  Uint32 const_speed_dist;	// distance to run at constant speed
  volatile Uint32 dist;
  volatile Uint32 steps;  
  Uint32 s_speed;			// start speed 
  Uint32 i_speed;			// target speed 
  Uint32 i_accel;			// target accel 
  Uint32 i_decel;			// target accel 

  volatile Uint32 cur_speed;			// current speed 

  POS sour_pos;
  POS cur_pos;				//current position with whole and fractional part separated

  volatile int32 set_pos;
  volatile int32 p_pos;

  volatile Uint16 index_flag;
  volatile Uint16 start_flag;	
  volatile Uint16 reindex_flag;
  volatile Uint16 block_flag;
  volatile Uint16 done_flag;
  volatile Uint16 rst_done_flag;  
  volatile Uint16 err_flag; 
  volatile Uint16 step_lose_flag;
  volatile Uint16 zero_lose_flag;
  volatile Uint16 zero_pass_flag;
  volatile Uint16 alarm_flag;  
  volatile Uint16 dct_cmd_first_flag;
  Uint16 half_current_mode;
		
  MOT_DIR dir;			// motor direction 0 = CW, 1 = CCW
  
  IDX_STATES state;	// current indexing state
  } INDEX ;    

/*-----------------------------------------------------------------------------
	Define the structure of the motor control parameters Object 
-----------------------------------------------------------------------------*/
typedef struct 
  {
     int32 set_dist;
     Uint32 set_speed;		// distance to accel
     Uint32 set_accel;	// distance to run at constant speed
  } MOT_PRM;  
/*-----------------------------------------------------------------------------
	Define a Index_handle
-----------------------------------------------------------------------------*/
typedef INDEX *INDEX_handle;
#define START_SPD 2400

void Index_Init(INDEX *i);
void Index(INDEX *i,Uint32 s_spd,Uint32 i_spd, Uint32 accel,Uint32 decel);
void Index_update(INDEX *i);
void Index_reset(INDEX *i);
#endif


