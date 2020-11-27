#include<reg52.h>
#include"lcd12864.h"
#include"am2320.h"
#include"keyboard.h"
void main()
{	
	Int0_init();												//外部中断0
	Init_ST7920();											//LCD12864初始化
	StaticCharacters();									//显示固定字符
	MinAMax(); 													//显示温湿度上下限数值											 
   	while(1)													 
		{  	
		amreaddata();									    //读取数据
	 	TempAndHumi();										//显示当前温湿度
		THcompare();										 	//比较数据
		delay10ms(240);										//AM2320每次测量数据间隔至少2s
		} 
}