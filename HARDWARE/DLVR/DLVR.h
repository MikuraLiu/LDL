#ifndef __DLVR_H
#define __DLVR_H
#include "myiic.h"   

u8   DLVR_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void DLVR_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
void DLVR_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
u32  DLVR_ReadLenByte(u16 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ����������
void DLVR_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void DLVR_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����
//void EXTIX_Init5();


void DLVR_Init(void); //��ʼ��IIC
u16  DLVR_ReadOneByte2();

int pressure();  //ѹ������
void Bratio(); //�����Ȳ���
#endif
