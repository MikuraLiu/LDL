#ifndef _MYIIC_H
#define _MYIIC_H
#include "stm32f4xx.h"
#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

void myiic_Init(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);

#endif
