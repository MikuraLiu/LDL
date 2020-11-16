#include "led.h"
#include "SysTick.h"
#include "mat_keys.h"
#include "system.h"
#include "usart.h"
#include "timer.h"
#include "myiic.h"
#include "lcd.h"
#include "DLVR.h"
#include "pid.h"
#include "pid_timer.h"
#include "beep.h"
#include "algorithm.h"
#include "max30102_2.h"
#include "max301002.h"
#include "myiic2.h"
#include "dht11.h"
#include "pwm.h"

 static int flag=3;
 u8 y=0;//全局变量，用于TIME3中断中
 u8 kaishi=0;
 
 #define MAX_BRIGHTNESS 255
uint32_t aun_ir_buffer[150]; //infrared LED sensor data
uint32_t aun_red_buffer[150];  //red LED sensor data
int32_t n_ir_buffer_length; //data length
int32_t n_spo2;  //SPO2 value
int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate; //heart rate value
int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;

int32_t hr_buf[16];
int32_t hrSum;
int32_t hrAvg;
int32_t spo2_buf[16];
int32_t spo2Sum;
int32_t spo2Avg;
int32_t spo2BuffFilled;
int32_t hrBuffFilled;
int32_t hrValidCnt = 0;
int32_t spo2ValidCnt = 0;
int32_t hrThrowOutSamp = 0;
int32_t spo2ThrowOutSamp = 0;
int32_t spo2Timeout = 0;
int32_t hrTimeout = 0;

void loop(void);	
void Send_To_PC( int rate, int spo2 );

 void DHT10()   //温度湿度
 {
    u8 temperature,humidity,t; 
	   if(t%10==0)			//每100ms读取一次
		{									  
			
		DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值
//		LCD_ShowFontHZ(3,1,"温度：");
//	  LCD_ShowFontHZ(4,1,"湿度：");	
//		Lcd_Num_Display(0x8B,temperature);
//		Lcd_Num_Display(0x9B,humidity);

			//LCD_WriteData(0x01);
			LCD_ShowFontHZ(3,1,"湿度");
			LCD_ShowFontHZ(3,5,"温度");
			
		LCD_ShowString(30,130,200,16,16,"temperature");
		LCD_ShowString(30,110,200,16,16,"humidity");
		}				   
	 	delay_ms(10);
		t++;
				if(t==20)
		{
			t=0;
			led1=!led1;
		}
 }
 int main(void)
 {		

	//u8 y=0;//全局变量，用于TIME3中断中
	u8 key;	
	u8  flag1=0;
	u16 Adjustable=1200;//可调变量 	
  u16 led0pwmval=1000,ya=0;
  extern float Pout; 
  int d=0;	 
	USART1_Init(115200);	 //串口初始化为115200
	SysTick_Init(168);	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//  BEEP_Init();
	TFTLCD_Init();
	LCD_ShowString(20,10,tftlcd_data.width,tftlcd_data.height,24,"asdfsdfsdfsdfdsf");
//	 LCD_Clear(GREEN);
//	LCD_Display_Dir(0);
  //Lcd_Start_Init();
  PIDInit();
/* 	LED_Init();//LED端口初始化
  DLVR_Init(); //IIC初始?
	InitKeyBoard();//键盘初始化
//	TIM3_PWM_Init(10000,71);
//	TIM4_Int_Init(10000,71);//2000,719
//	TIM5_Int_Init(10000,719);//0.1s
	myiic_Init();        //模拟iic初始化
  maxim_max30102_reset();
	maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_dummy);
  maxim_max30102_init(); 
	beep=1;
  DHT11_Init();
	LCD_Clear(GREEN);*/
	while(1);
  while(1)
	 {	  
		    //TIM_Cmd(TIM4, ENABLE);
		    key=MatKeysScan();//按键扫描
		 		 		if(key==0xEB)//按键上的C 进入手动模式
				{
					delay_ms(100);
				  if(key==0xEB)
              flag1=1;
				}
			 if(key==0xE7)//按键上的D
				{
					delay_ms(100);
				  if(key==0xE7)
           flag1=2;					
					}
				
			if(flag1==1)	
			{	
		  if(key==0xEE)
				{
					delay_ms(100);
				  if(key==0xEE)
		       {
						 TIM_Cmd(TIM3, DISABLE);
					   flag=flag+1;
						 LCD_Clear(GREEN);//清屏
				   }
			  }
				if(key==0xDE)
				{
					delay_ms(100);
				  if(key==0xDE)
				  {
						  
						  TIM_Cmd(TIM3, ENABLE);
			        flag=flag-1;
						  LCD_Clear(GREEN);
				  }	
	      }
				
									if(key==0xDD) //按键上的6
				{
					delay_ms(100);
				  if(key==0xDD)
					 Adjustable+=50;
				}
					
					if(key==0xDB) //按键上的9
				{
					delay_ms(100);
				  if(key==0xDB)
					 Adjustable-=50;
				}
				if(flag%2==1)
				 {
					 //TIM_Cmd(TIM4, ENABLE);
				    Bratio();
					  beep=0;
				 }
				if(flag%2==0)
				 {   
            kaishi=1;
		        ya=pressure();
		        led0pwmval=Adjustable+Pout;
    		    LCD_ShowFontHZ(1,1,"风速值：");
				    LCD_ShowxNum(244,240,led0pwmval,4,16,0);	
		        TIM_SetCompare2(TIM3,led0pwmval);
					  //LCD_WriteData(0x01);
            loop();
		        DHT10();
					 
				 }
			 }
				if(flag1==2)
				{
						    if(key==0xEE)
									
				{
					delay_ms(100);
				  if(key==0xEE)
		       {
						 TIM_Cmd(TIM3, DISABLE);
					   flag=flag+1; 
						 LCD_Clear(GREEN);//清屏
				   }
			  }
				if(key==0xDE)
				{
					delay_ms(100);
				  if(key==0xDE)
				  {
						  
						  TIM_Cmd(TIM3, ENABLE);
			        flag=flag-1;
						  LCD_WriteData(0x01);
				  }	
	      }
				if(flag%2==1)
				 {
					 // TIM_Cmd(TIM4, ENABLE);
				    Bratio();
					  beep=0;
				 }
				if(flag%2==0)
				 {
				    kaishi=1;
		        ya=pressure();
		        led0pwmval=1450+Pout;
    		    LCD_ShowFontHZ(1,1,"风速值：");
				    LCD_ShowxNum(234,230,led0pwmval,4,16,0);
		        TIM_SetCompare2(TIM3,led0pwmval);
					  //LCD_WriteData(0x01);
            loop();
		        DHT10();
				 }
				
				}					
       	
  }
}
void loop()    //心率血氧
{
	 uint32_t un_min, un_max, un_prev_data, un_brightness;  
   int32_t i;
   float f_temp;
   un_brightness = 0;
   un_min = 0x3FFFF;
   un_max = 0;

    n_ir_buffer_length = 150; 

    for(i = 0; i < n_ir_buffer_length; i++)
    {
        
        maxim_max30102_read_fifo((aun_red_buffer + i), (aun_ir_buffer + i)); 

        if(un_min > aun_red_buffer[i])
            un_min = aun_red_buffer[i]; //update signal min
        if(un_max < aun_red_buffer[i])
            un_max = aun_red_buffer[i]; //update signal max
    
    }
    un_prev_data = aun_red_buffer[i];
    //calculate heart rate and SpO2 after first 150 samples (first 3 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

    //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
 
  
        i = 0;
        un_min = 0x3FFFF;
        un_max = 0;

        //dumping the first 50 sets of samples in the memory and shift the last 100 sets of samples to the top
        for(i = 50; i < 150; i++)
        {
            aun_red_buffer[i - 50] = aun_red_buffer[i];
            aun_ir_buffer[i - 50] = aun_ir_buffer[i];

            //update the signal min and max
            if(un_min > aun_red_buffer[i])
                un_min = aun_red_buffer[i];
            if(un_max < aun_red_buffer[i])
                un_max = aun_red_buffer[i];
        }

        //take 50 sets of samples before calculating the heart rate.
        for(i = 100; i < 150; i++)
        {
            un_prev_data = aun_red_buffer[i - 1];
            //while(KEY0 == 1);
            maxim_max30102_read_fifo((aun_red_buffer + i), (aun_ir_buffer + i));

            //calculate the brightness of the LED
            if(aun_red_buffer[i] > un_prev_data)
            {
                f_temp = aun_red_buffer[i] - un_prev_data;
                f_temp /= (un_max - un_min);
                f_temp *= MAX_BRIGHTNESS;
                f_temp = un_brightness - f_temp;
                if(f_temp < 0)
                    un_brightness = 0;
                else
                    un_brightness = (int)f_temp;
            }
            else
            {
                f_temp = un_prev_data - aun_red_buffer[i];
                f_temp /= (un_max - un_min);
                f_temp *= MAX_BRIGHTNESS;
                un_brightness += (int)f_temp;
                if(un_brightness > MAX_BRIGHTNESS)
                    un_brightness = MAX_BRIGHTNESS;
            }
	
        }
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

        if ((ch_hr_valid == 1) && (n_heart_rate < 190) && (n_heart_rate > 40))
        {
            hrTimeout = 0;

          // Throw out up to 1 out of every 5 valid samples if wacky
            if (hrValidCnt == 4)
            {
                hrThrowOutSamp = 1;
                hrValidCnt = 0;
                for (i = 12; i < 16; i++)
                {
                    if (n_heart_rate < hr_buf[i] + 10)
                    {
                        hrThrowOutSamp = 0;
                        hrValidCnt   = 4;
                    }
                }
            }
            else
            {
                hrValidCnt = hrValidCnt + 1;
            }

            if (hrThrowOutSamp == 0)
            {

                // Shift New Sample into buffer
                for(i = 0; i < 15; i++)
                {
                    hr_buf[i] = hr_buf[i + 1];
                }
                hr_buf[15] = n_heart_rate;

                // Update buffer fill value
                if (hrBuffFilled < 16)
                {
                    hrBuffFilled = hrBuffFilled + 1;
                }

                // Take moving average
                hrSum = 0;
                if (hrBuffFilled < 2)
                {
                    hrAvg = 0;
                }
                else if (hrBuffFilled < 4)
                {
                    for(i = 14; i < 16; i++)
                    {
                        hrSum = hrSum + hr_buf[i];
                    }
                    hrAvg = hrSum >> 1;
                }
                else if (hrBuffFilled < 8)
                {
                    for(i = 12; i < 16; i++)
                    {
                        hrSum = hrSum + hr_buf[i];
                    }
                    hrAvg = hrSum >> 2;
                }
                else if (hrBuffFilled < 16)
                {
                    for(i = 8; i < 16; i++)
                    {
                        hrSum = hrSum + hr_buf[i];
                    }
                    hrAvg = hrSum >> 3;
                }
                else
                {
                    for(i = 0; i < 16; i++)
                    {
                        hrSum = hrSum + hr_buf[i];
                    }
                    hrAvg = hrSum >> 4;
                }
            }
            hrThrowOutSamp = 0;
        }
        else
        {
            hrValidCnt = 0;
            if (hrTimeout == 4)
            {
                hrAvg = 0;
                hrBuffFilled = 0;
            }
            else
            {
                hrTimeout++;
            }
        }

        if ((ch_spo2_valid == 1) && (n_spo2 > 59))
        {
            spo2Timeout = 0;
            if (spo2ValidCnt == 4)
            {
                spo2ThrowOutSamp = 1;
                spo2ValidCnt = 0;
                for (i = 12; i < 16; i++)
                {
                    if (n_spo2 > spo2_buf[i] - 10)
                    {
                        spo2ThrowOutSamp = 0;
                        spo2ValidCnt   = 4;
                    }
                }
            }
            else
            {
                spo2ValidCnt = spo2ValidCnt + 1;
            }

            if (spo2ThrowOutSamp == 0)
            {

                // Shift New Sample into buffer
                for(i = 0; i < 15; i++)
                {
                    spo2_buf[i] = spo2_buf[i + 1];
                }
                spo2_buf[15] = n_spo2;

                // Update buffer fill value
                if (spo2BuffFilled < 16)
                {
                    spo2BuffFilled = spo2BuffFilled + 1;
                }

                // Take moving average
                spo2Sum = 0;
                if (spo2BuffFilled < 2)
                {
                    spo2Avg = 0;
                }
                else if (spo2BuffFilled < 4)
                {
                    for(i = 14; i < 16; i++)
                    {
                        spo2Sum = spo2Sum + spo2_buf[i];
                    }
                    spo2Avg = spo2Sum >> 1;
                }
                else if (spo2BuffFilled < 8)
                {
                    for(i = 12; i < 16; i++)
                    {
                        spo2Sum = spo2Sum + spo2_buf[i];
                    }
                    spo2Avg = spo2Sum >> 2;
                }
                else if (spo2BuffFilled < 16)
                {
                    for(i = 8; i < 16; i++)
                    {
                        spo2Sum = spo2Sum + spo2_buf[i];
                    }
                    spo2Avg = spo2Sum >> 3;
                }
                else
                {
                    for(i = 0; i < 16; i++)
                    {
                        spo2Sum = spo2Sum + spo2_buf[i];
                    }
                    spo2Avg = spo2Sum >> 4;
                }
            }
            spo2ThrowOutSamp = 0;
        }
        else
        {
            spo2ValidCnt = 0;
            if (spo2Timeout == 4)
            {
                spo2Avg = 0;
                spo2BuffFilled = 0;
            }
            else
            {
                spo2Timeout++;
            }
        }

        Send_To_PC(hrAvg, spo2Avg);    
}

	 void Send_To_PC( int rate, int spo2 )
{  
	
		LCD_ShowFontHZ(2,1,"心率");
	  LCD_ShowFontHZ(2,5,"血氧");
	  LCD_ShowxNum(134,130,rate,4,16,0);
	  LCD_ShowxNum(144,140,spo2,4,16,0);	

//	  LCD_ShowFontHZ(1,1,"心率：");
//	  LCD_ShowFontHZ(2,1,"血氧：");
//	  Lcd_Num_Display(0x83,rate);
//	  Lcd_Num_Display(0x93,spo2);			
}		 
