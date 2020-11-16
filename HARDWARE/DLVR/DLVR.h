#ifndef __DLVR_H
#define __DLVR_H
#include "myiic.h"   

u8   DLVR_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void DLVR_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void DLVR_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32  DLVR_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void DLVR_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void DLVR_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据
//void EXTIX_Init5();


void DLVR_Init(void); //初始化IIC
u16  DLVR_ReadOneByte2();

int pressure();  //压力测量
void Bratio(); //呼吸比测量
#endif
