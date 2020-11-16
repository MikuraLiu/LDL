#ifndef __PID_H__
#define __PID_H__

//#include <project.h>


typedef struct{
   float err;           //���
   float err_last;      //��ǰһ��ƫ��
   float err_next;      //��һ��ƫ��ֵ
   float Kp,Ki,Kd;      //KP,KI,KD
   float pout;          //P ���
   float iout;          //I ���
   float dout;          //D ���
   float out ;          // �����
   float integral;      //����
}PID_Typedef;


//extern void PID_Init(void);
//extern void PID_Init(PID_Typedef *PID_1);

float PID_Adjust(PID_Typedef *PID, float set, float act, int Mode);
extern void PIDInit(void);

//void test(MPU6050_XYZType angle);
#endif
