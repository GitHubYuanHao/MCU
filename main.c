#include<reg52.h>
#include"lcd12864.h"
#include"am2320.h"
#include"keyboard.h"
void main()
{	
	Int0_init();												//�ⲿ�ж�0
	Init_ST7920();											//LCD12864��ʼ��
	StaticCharacters();									//��ʾ�̶��ַ�
	MinAMax(); 													//��ʾ��ʪ����������ֵ											 
   	while(1)													 
		{  	
		amreaddata();									    //��ȡ����
	 	TempAndHumi();										//��ʾ��ǰ��ʪ��
		THcompare();										 	//�Ƚ�����
		delay10ms(240);										//AM2320ÿ�β������ݼ������2s
		} 
}