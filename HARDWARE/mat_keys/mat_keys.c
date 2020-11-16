#include "mat_keys.h"
#include "SysTick.h"
#include "lcd.h"
//#include "timer.h"
#define BUFF_SIZE  20
u8 kb_buffer[BUFF_SIZE];
u8 *inpt, *outpt;
u8 buffcnt;					//缓冲区键值计数
void MatKeysConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
/**********************电平反转法，占用连续八个IO口***************************************************/	
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	//该四位控制行扫描
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//??????
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
//	GPIO_InitStructure.GPIO_Mode = GPIO_Out_PP;  //设置推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //该四位检测列输入
	//GPIO_InitStructure.GPIO_Mode = GPIO_IPU;//无外挂上拉电阻，需要设置为上拉输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	
}
/*将新键值放入键盘缓冲区inpt所指处*/
void put_kbbuff(u8 c) 
{
    if (buffcnt<BUFF_SIZE)                        // If buffer not full
    {
        *inpt = c;                                // Put character into buffer
        inpt++;                                    // Increment pointer
        buffcnt++;
        if (inpt >= kb_buffer + BUFF_SIZE)        // Pointer wrapping
            inpt = kb_buffer;
    }
	else{}	//buffer full ring to alarm
//	lcd_buff[0][0]=c;	lcd_buff[1][0]=c;
}

/*返回键盘缓冲区outpt所指的键值*/
u8  get_char(void) 
{
    u8 byte,C;
    if(buffcnt)
		{
	    byte = *outpt;                                 // Get byte
	    outpt++;                                       // Increment pointer
	    if (outpt >= kb_buffer + BUFF_SIZE)            // Pointer wrapping
	        {outpt = kb_buffer;}
	    buffcnt--;                                     // Decrement buffer count
      switch(byte)
			{    
				   case 0xEE:C=1;break;
				   case 0xED:C=4;break;
				   case 0xEB:C=7;break;
           case 0xE7:C=15;break;					 
				   case 0xDE:C=2;break;
				   case 0xDD:C=5;break;
				   case 0xDB:C=8;break;
           case 0xD7:C=0;break;
	         case 0xBE:C=3;break;
				   case 0xBD:C=6;break;
			     case 0xBB:C=9;break;
           case 0xB7:C=14;break;
           case 0x7E:C=10;break;
				   case 0x7D:C=11;break;
				   case 0x7B:C=12;break;
           case 0x77:C=13;break; 
					 default:C=16;break;
	  }
	  
				return C;
	}
	else{
		
		return 16;
	    }
}
/*初始化键盘*/
void InitKeyBoard(void)
{
    inpt =  kb_buffer;                        // Initialize buffer
    outpt = kb_buffer;
    buffcnt = 0;
	  //ScanKeysFlag=1;//关闭键盘扫描
	  MatKeysConfig();
}	
/*获取缓冲区中键值的个数*/
u8 Get_buffcnt(void)
{
   return buffcnt;
}
/************************/
/**
 *描述：扫描4*4矩阵键盘返回编码值
 *输入：无
 *输出：返回按键十六进制编码值
				EE ED EB E7
				DE DD DB D7
				BE BD BB B7
				7E 7D 7B 77
 *注意：无
 **/
u8 MatKeysScan(void)
{	

/**********************电平反转法，占用连续八个IO口********************************************************/	
	u8 sccode=0,recode=0;
  GPIO_Write(GPIOC,0xf0);
  if((GPIO_ReadInputData(GPIOC)&0xf0)!=0xf0)
	  {
      //Delay_1ms(20);
	if((GPIO_ReadInputData(GPIOC)&0xf0)!=0xf0)
		{
		  sccode=0xfe;
		 while((sccode&0x10)!=0)
		 {
			 GPIO_Write(GPIOC,sccode);
		  if((GPIO_ReadInputData(GPIOC)&0xf0)!=0xf0)
		  {
		   recode=(GPIO_ReadInputData(GPIOC)&0xf0);
		   sccode=(sccode&0x0f);
        while((GPIO_ReadInputData(GPIOC)&0xf0)!=0xf0){};//等待按键释放
		    return((sccode)+(recode));
		  }
		  else
		   sccode=(sccode<<1)|0x01;
		  }
		 
		 }
		 }
	 return 0;
 }
