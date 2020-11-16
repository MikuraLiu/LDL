#ifndef __PID_H__
#define __PID_H__

//#include <project.h>


typedef struct{
   float err;           //误差
   float err_last;      //最前一次偏差
   float err_next;      //上一个偏差值
   float Kp,Ki,Kd;      //KP,KI,KD
   float pout;          //P 输出
   float iout;          //I 输出
   float dout;          //D 输出
   float out ;          // 总输出
   float integral;      //积分
}PID_Typedef;


//extern void PID_Init(void);
//extern void PID_Init(PID_Typedef *PID_1);

float PID_Adjust(PID_Typedef *PID, float set, float act, int Mode);
extern void PIDInit(void);

//void test(MPU6050_XYZType angle);
#endif
