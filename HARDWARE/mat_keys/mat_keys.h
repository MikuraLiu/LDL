#ifndef __MAT_KEYS_H__
#define	__MAT_KEYS_H__


#include "stm32f4xx.h"

void InitKeyBoard(void);
u8 MatKeysScan(void);
void Display_Key(u8 num);
void put_kbbuff(u8 c);
u8  get_char(void);
u8 Get_buffcnt(void);


#endif
