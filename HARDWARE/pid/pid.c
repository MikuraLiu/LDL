

#include "math.h"
#include "pid.h"


PID_Typedef PID_1,PID_2,PID_3;
PID_Typedef PID[3];
//void rt6(void)
//{
//
//	PID_1->Kp = 100;//0.032  0.2-0.25  0.23
//	PID_1->Ki = 0;      // ;0.00037.095
//	PID_1->Kd = 0 ;     //0.151 1  0.75  1.05
//
//}

/* 
 * ??  : err = set - Actual
 * ???:
 *  u(k)-u(k-1)=kp*(e(k)-e(k-1))+ki*e(k)+kd(e(k)-2e(k-1)+e(k-2))
 *  ???
 *  u(k)=kp*e(k)+ki*(e(k)++)+kd*(e(k)-e(k-1))
 */
 
float PID_Adjust(PID_Typedef *PID, float set, float act,int Mode)
{      
    
    //PID->err =abs(set - act);
      PID->err =set - act;
    
        ////  ???
    if(Mode == 0)
    {
        PID->integral +=PID->err;

        if(PID->integral>65536)PID->integral=65536;
        PID->out = PID->Kp*PID->err + PID->Ki*PID->integral + PID->Kd*(PID->err - PID->err_last);
        
        PID->err_last = PID->err;    
    }
    
    ///???    
    if(Mode == 1)
    {
        PID->pout    = PID->Kp * (PID->err - PID->err_next);
        PID->iout    = PID->Ki * PID->err;
        PID->dout    = PID->Kd * (PID->err-2*PID->err_next+PID->err_last);  
        
        PID->err_last=PID->err_next;
        PID->err_next=PID->err;
        PID->out     =PID->out + PID->pout + PID->iout + PID->dout;    
    }
		
    if(PID->out>400) PID->out=400;
    return PID->out;
}


void PIDInit(void)
{

    PID[0].Kp = 0.80;   // 0.8;
    PID[0].Ki = 0.05;  //0.06;
    //PID[0].Kd = 0.04;
        
    PID[1].Kp = 0;
    PID[1].Ki = 0;
    PID[1].Kd = 0;
        
    PID[2].Kp = 0;
    PID[2].Ki = 0;
    PID[2].Kd = 0;
}

