#ifndef __keyboard_h_
#define __keyboard_h_
#define KeyPort P1 //�������ӿ�

sbit SPK=P3^0;    //�������ȶ˿�

unsigned char KeyScan(void);
unsigned char KeyPro(void);
void UpperLower(void);
void MinAMax();
void Int0_init(); 
void bee(bit b);
void THcompare(void);
#endif
