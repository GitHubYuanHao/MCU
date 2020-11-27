#ifndef __DELAY_H__
#define __DELAY_H__
/*------------------------------------------------
 uS延时函数，大致延时T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t);
/*------------------------------------------------
 mS延时函数t是无符号字符变量，其值的范围是
 0~255 这里使用晶振12M
------------------------------------------------*/
void DelayMs(unsigned char t);
void delay10us();
void delay10ms(unsigned char t);
#endif