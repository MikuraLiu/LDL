#include "DLVR.h"
#include "SysTick.h"
#include "lcd.h"
#include "beep.h"
extern u8 y;
extern u8 kaishi;
void DLVR_Init(void)
{
	IIC_Init();
	BEEP_Init();
}

u16 DLVR_ReadOneByte2()
{	
	static u16 temp=0;
	static u16 tempcunchu;	
	u16  temp1=0;	
  u16  temp2=0;
  u8  tempbiaozhi;	
  IIC_Start();
	IIC_Send_Byte(0x51);   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();
  tempcunchu=IIC_Read_Byte();
	IIC_Ack();
	temp2=IIC_Read_Byte();
  IIC_NAck();
	IIC_Stop();
	tempbiaozhi=(tempcunchu&0xc0)>>4;
//	Lcd_Num_Display1(0x80,tempbiaozhi);  //显示标志位
	temp1=tempcunchu&0x3F;
	temp=(temp1<<8)|(temp2&0x00ff);
	return temp;
}
 //处理数据显示压力
 int pressure()
 {
	 int h;
   int datatemp;  //IIC读出的数据
	 float yali,yalii;
 	 datatemp=DLVR_ReadOneByte2();
	 datatemp=(datatemp-8192)*1.25;
	 yali=datatemp*0.16;
   if(yali<0)
     {
	       yalii=-yali;
	       LCD_WriteCmdData(0x9C,yalii);
	   }
   else		 
	    LCD_WriteCmdData(0x9C,yali/100);//压力值转换为厘米水注/98
	    LCD_ShowFontHZ(4,1,"压力值：");
	    LCD_ShowFontHZ(4,7,"cmH2O");
     if(kaishi==1)
   {
	    h=yali/100;
			if(h==0)
				y++;
			else
			{
				y=0;
				beep=0;
			}
			if(y==100)//20
			{
			beep=1;
			y=0;
			}		
		}
 else
	 beep=0;
	    return yali;
 }
 //测量呼吸比
 void Bratio()
 {
	 int pressur;
   static float xtime=0,htime=0;   //吸气呼气时间长度	 
	 static float Bratio=0.00;
	 pressur=pressure();
	 if(pressur>0)
	 {
			htime=htime+1;
	 }
	 if(pressur<0)
	 {
			xtime=xtime+1;
	 }
	 Bratio=htime/xtime;
	 Bratio=Bratio*100;
	 LCD_ShowFontHZ(1,1,"呼气时长：");
	 LCD_ShowFontHZ(2,1,"吸气时长：");
	 LCD_ShowFontHZ(3,1,"呼吸比：");
	 LCD_WriteCmdData(0x85, htime);
	 LCD_WriteCmdData(0x95, xtime);
	 LCD_WriteCmdData(0x8c, Bratio);
	 LCD_ShowFontHZ(3,7,"：1");
 }	  
	     
	 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

//void EXTIX_Init5(void)
//{
// 
// 	EXTI_InitTypeDef EXTI_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
// 
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
//	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//KEY0-KEY2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOE2,3,4
//	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);

//  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;	//KEY2
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

//  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键KEY1所在的外部中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1 
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
//  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
// 
//}

//void EXTI1_IRQHandler(void)
//{  
//   u16 datatemp;  //IIC读出的数据
//	 float yali;
//	 if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
//   {		 
//	
// 	   datatemp=DLVR_ReadOneByte2();
//	   datatemp=(datatemp-8192)*1.25;
//	   yali=datatemp*0.16;  
//	   Lcd_Num_Display1(0x98,yali);
//	   LCD_ShowFontHZ(4,4,"Pa");
//		 EXTI_ClearITPendingBit(EXTI_Line1);  //清除LINE8上的中断标志位
//	 } 
//  
//}


