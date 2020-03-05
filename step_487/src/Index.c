#include "DSP28x_Project.h"
#include "Index.h"
#include <math.h>
#include "IQmathLib.h"

/*****************************************************************************/
/* Initialize the index structure to a known state                           */
/*****************************************************************************/
void Index_Init(INDEX *i)
  {
  i->state = IDLE;
  i->dir = DIR_CW;
  i->sour_pos.frac = 0;
  i->sour_pos.whole = 0;
  i->cur_pos.frac = 0;
  i->cur_pos.whole = 0;
  i->cur_speed = 0;
  i->total_dist = 0;
  i->accel_dist = 0;
  i->const_speed_dist= 0;
  i->index_flag=0;
  i->start_flag=0;
  i->reindex_flag=0;
  i->block_flag=0;
  i->done_flag=0;
  i->rst_done_flag=0;  
  i->step_lose_flag=0;
  i->zero_lose_flag=0;
  i->zero_pass_flag=0;  
  i->err_flag=0;
  i->alarm_flag=0;
  i->half_current_mode=1;
  i->dct_cmd_first_flag=1;
  i->set_pos=0;
  i->p_pos=0;
  i->dist = 0;
  i->steps= 0;  
  }

/*****************************************************************************/
/* This functions kicks off an index                                         */
/* Inputs:                                                                   */
/*         i - pointer to an INDEX structure				                 */
/*         dist - signed 32-bit index distance  (pulses)                      */
/*         speed - unsigned speed (pulses/sec)                                */
/*         accel - unsigned acceleration (pulses/sec^2)                       */
/*         nowspeed - current speed _IQ(pulses/sec)                      */
/*****************************************************************************/
void Index(INDEX *i, Uint32 s_spd,Uint32 i_spd, Uint32 accel,Uint32 decel)
 {
      int32 tmp;
      Uint32 decel_dist;
      MOT_DIR tmp_dir;
      if(i->set_pos!=i->cur_pos.whole)
      {
          tmp=i->set_pos -i->cur_pos.whole;
          if(tmp>=400)
          {
              tmp=800-tmp;
              i->total_dist=(Uint32)tmp;
	       tmp_dir=DIR_CCW;
	   }	
	   else if(tmp<=-400)
	   {
              tmp=800+tmp;
              i->total_dist=(Uint32)tmp;
	       tmp_dir=DIR_CW;	      
	   }
	   else
	   {
              if(tmp>=0)
              {
                 i->total_dist=(Uint32)tmp;
	          tmp_dir=DIR_CW;
	       }
	       else
	       {
	          tmp=-tmp;
                 i->total_dist=(Uint32)tmp;
	          tmp_dir=DIR_CCW;	    
	       } 	     
	   }
	   if(i->total_dist>=4)
	   {
	       i->dir=tmp_dir;
	       i->s_speed=s_spd;
	       i->i_speed=i_spd;
	       i->i_accel=accel;
              i->i_decel=decel;
	       if(i->i_speed<=i->s_speed)
		{
		    i->i_speed=i->s_speed;
		    i->accel_dist= 0;
	           i->const_speed_dist =i->total_dist-1;	  
		}
		else
		{
		    if(i->i_accel<20)
		    {
		       i->i_accel=20;
		    }
		    if(i->i_decel<20)
		    {
		       i->i_decel=20;
		    }
                  i->accel_dist=(i->i_speed-i->s_speed-1)/i->i_accel+1;
                  decel_dist=(i->i_speed-i->s_speed-1)/i->i_decel+1;	  
                  if(i->total_dist<=(i->accel_dist+decel_dist))
                  {
	               i->const_speed_dist = 0;
	               i->accel_dist = i->total_dist*i->i_decel/(i->i_accel+i->i_decel);
                  }
                  else
	           {
	               i->const_speed_dist = i->total_dist-i->accel_dist-decel_dist; 
  	           }
		}
               i->sour_pos.whole= i->cur_pos.whole;
               i->sour_pos.frac= i->cur_pos.frac; 
               //initialize any other variables
               i->cur_speed =i->s_speed;
               i->dist=0;   
	        i->steps=0; 
               //set the state to ACCEL to begin the move on the next interrupt
               i->p_pos=i->set_pos;
               i->state = ACCEL;	
               i->start_flag=1;
	  }
      }
 }

  
/*****************************************************************************/
/* This function should be called from your interrupt to generate the index  */
/* profile.                                                                  */  
/*****************************************************************************/
void Index_update(INDEX *i)
  {
       i->steps++;
       i->dist++;
	if(i->dir==DIR_CW)
	{
	   i->cur_pos.whole++;
	   if(i->cur_pos.whole>=800)
	   {
	       i->cur_pos.whole-=800;
	   }
	}
	else
	{
	    i->cur_pos.whole--;
	    if(i->cur_pos.whole<0)
	    {
	       i->cur_pos.whole+=800;
	    }		
	}
    switch(i->state)
    {
	/* Accelerating case.                                    */
	/* cur_speed is incremented by i_accel each time         */
	/*********************************************************/
	case ACCEL:
		if(i->dist<i->accel_dist)
		{
		   i->cur_speed+=i->i_accel;
		   if(i->cur_speed>i->i_speed)
		   {
		      i->cur_speed=i->i_speed;
		   }
		}
		else
		{
		   if(i->const_speed_dist>0)
		   {
		     i->cur_speed=i->i_speed;
		     i->state=CONST_SPD;
		   }
		   else
		   {
		     i->state=DECEL;
		   }
		}
	  break;
	/*********************************************************/
	/* Constant Speed case.                                  */
	/* Put i_speed into cur_speed each time                  */
	/*********************************************************/
	case CONST_SPD:
		  if(i->dist>=(i->accel_dist+i->const_speed_dist))
	 	  {
		     i->state=DECEL;
		  }		
	 break;
	/*********************************************************/
	/* Decelerating case.                                    */
	/* cur_speed is decremented by i_accel each time         */
    /* when decel_ticks = 0 go back to IDLE case             */
	/*********************************************************/
	case DECEL:
		if(i->dist<i->total_dist)
		{
		   if(i->cur_speed >i->s_speed)
		   {
		     i->cur_speed -=i->i_decel;
		     if(i->cur_speed<i->s_speed)
		     {
		        i->cur_speed=i->s_speed;
		     } 
		   }
		   else
		   {
		     i->cur_speed=i->s_speed; 
		   }
		}
		else
		{
		   i->cur_speed=i->s_speed;
		   i->state=IDLE;
		}		
	 break;
	/*********************************************************/
	/* default case.                                         */
	/* Invalid case, set to IDLE                             */
	/*********************************************************/
	default:
	  break;
    }
  }


void Index_reset(INDEX *i)
{
  Uint32 tmp_dist;
  Uint32 tmp1,tmp2;
  int32 p1,p2;
  if(i->set_pos!=i->p_pos)
  {  
    if(i->dir==DIR_CW)
    {
       if(i->set_pos<i->cur_pos.whole)
       {
           p1=i->set_pos+800;
       }
	else
	{
           p1=i->set_pos;
	}
	p2=i->cur_pos.whole;	
    }
    else
    {
        if(i->set_pos>i->cur_pos.whole)
       {
           p2=i->set_pos-800;
       }
	else
	{
           p2=i->set_pos;
	}
	p1=i->cur_pos.whole;  
    }
    if(p1-p2>=460)
    {
       p1-=800;
    }	
    DINT;	
    switch(i->state)
    {
	  case ACCEL:
	    if(p1>p2)
	    {
		  tmp_dist=p1-p2;
		  if(tmp_dist>=i->dist)
		  {
		     tmp1=(i->i_speed-i->s_speed-1)/i->i_accel+1;
                   tmp2=(i->i_speed-i->s_speed-1)/i->i_decel+1; 
		     if(tmp_dist>(tmp1+tmp2-i->dist))
		     { 
                        i->accel_dist=tmp1;
			   i->const_speed_dist=tmp_dist+i->dist-tmp1-tmp2;
			   i->total_dist=tmp_dist+i->dist;
		      }
		       else
		      {
			  i->accel_dist=(tmp_dist+i->dist)*i->i_decel/(i->i_accel+i->i_decel);
                        i->const_speed_dist=0;
			  i->total_dist=tmp_dist+i->dist;
                        i->state=ACCEL;					
			}
		  }
		  else
		  {
		      i->accel_dist=i->dist;
                    i->const_speed_dist=0;
		      i->total_dist=i->dist+i->dist*i->i_accel/i->i_decel; 
		      i->state=DECEL;
                    i->reindex_flag=1;				  
		  }
	    }
	    else
	    {
		  i->accel_dist=i->dist;
                i->const_speed_dist=0;
                i->total_dist=i->dist+i->dist*i->i_accel/i->i_decel;
                i->state=DECEL;
                i->reindex_flag=1;					
	    } 
	     break;
	  case CONST_SPD:
	    tmp1=(i->i_speed-i->s_speed-1)/i->i_decel+1;
	    if(p1>p2)
	    {
		  tmp_dist=p1-p2;
		  if(tmp_dist>=tmp1)
		  {
                   i->const_speed_dist=i->dist+tmp_dist-i->accel_dist-tmp1;
                   i->total_dist=tmp_dist+i->dist;			   
		  }
		  else
		  {
		    i->const_speed_dist=i->dist-i->accel_dist;
		    i->total_dist=i->dist+tmp1;
                  i->state=DECEL;
                  i->reindex_flag=1;		    
		  }
	    }
	    else
	    {
		    i->const_speed_dist=i->dist-i->accel_dist;
		    i->total_dist=i->dist+tmp1;
                  i->state=DECEL;	
		    i->reindex_flag=1;
	    } 	    
	  break;
	  case DECEL:
	    if(p1>p2)
	    {
		  tmp_dist=p1-p2;
		  if(tmp_dist>=(i->total_dist-i->dist))
		  {
		    tmp1=(i->i_speed-i->s_speed-1)/i->i_accel+1;
                  tmp2=(i->i_speed-i->s_speed-1)/i->i_decel+1;
		    if(tmp_dist>(tmp1+tmp2+i->dist-i->total_dist))
		    {
			  i->dist=i->total_dist-i->dist;
                        i->accel_dist=tmp1;
                        i->total_dist=tmp_dist+i->dist;
                        i->const_speed_dist=i->total_dist-tmp1-tmp2; 
                        i->state=ACCEL;					
		     }
		     else
		    {
			  i->dist=i->total_dist-i->dist;
                        i->total_dist=tmp_dist+i->dist;
                        i->accel_dist=i->total_dist*i->i_decel/(i->i_accel+i->i_decel);
                        i->const_speed_dist=0; 
                        i->state=ACCEL;			  
		     }
		  }
		  else
		  {
		    i->reindex_flag=1;
		  }
	    }
	    else
	    {
		  i->reindex_flag=1;
	    }	     
	  break;
      case IDLE:
         i->reindex_flag=1;	  	
	  break;
      default:
	  break;
	}
	i->steps=0;
	i->p_pos=i->set_pos;
	EINT;
    }
}


