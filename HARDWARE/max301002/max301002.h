#ifndef _MAX301002_H
#define _MAX301002_H
#include "stm32f4xx.h"
#define max30102_WR_address 0xAE
u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data);
u8 max30102_Bus_Read(u8 Register_Address);
void max30102_FIFO_Read(u8 Register_Address,u16  Word_Data[][2],u8 count);
void max30102_init(void);







#endif

