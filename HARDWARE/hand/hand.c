#include "hand.h"
#include "system.h"
#include "pwm.h"
#include "SysTick.h"
#include "DLVR.h"
#include "lcd.h"


void hand_Init()
{
	TIM14_CH1_PWM_Init(10000,71);	 //����Ƶ��PWMƵ��=72000000/900=20Khz  //Ƶ����50Hz���ȸ�1000�������ٸ�1050����200
}

//void hand_control()
//{    
//	      u16 led0pwmval=1000,dir=0
//        if(dir==0)
//				  {
//            led0pwmval=led0pwmval+50;
//						delay_ms(10000);
//				  }
//				if(dir==1)
//				  {
//				    led0pwmval=led0pwmval-50;
//						delay_ms(10000);
//				  }
//				if(led0pwmval>1800)dir=1;
//				if(led0pwmval<1050)dir=0;
//				TIM_SetCompare2(TIM3,led0pwmval);
//				LCD_ShowFontHZ(1,1,"����ֵ��");
//				Lcd_Num_Display(0x84,led0pwmval);	
//	  
//}

