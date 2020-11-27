#ifndef __AM2320_H_
#define __AM2320_H_
#include<reg52.h>
#include<intrins.h>

sbit amport=P2^0;
void amstart();
unsigned char amread();
void amreaddata();
void delay10us();
void delay10ms(unsigned char t);
#endif