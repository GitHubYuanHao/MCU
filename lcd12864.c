#include<reg52.h>
#include"lcd12864.h"
#include"delay.h"
extern bit f;
extern int temp,humi;
extern unsigned char tmh,tml;
unsigned char code pic1[]={0x00,0x00,0x00,0x00,0x18,0x00,0x24,0xF0,0x25,0x08,0x1A,0x04,0x02,0x00,0x02,0x00,
									0x02,0x00,0x02,0x00,0x02,0x04,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,};	  //摄氏度
unsigned char code pic2[]={0x00,0x00,0x00,0x00,0x18,0x04,0x24,0x08,0x24,0x10,0x18,0x20,0x00,0x40,0x00,0x80,
						            0x01,0x00,0x02,0x00,0x04,0x18,0x08,0x24,0x10,0x24,0x20,0x18,0x00,0x00,0x00,0x00,};	  //百分号
unsigned char code pic3[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x10,0xA9,0x2B,0xA8,0x2B,0xA9,0x28,
									0xA9,0x28,0xA9,0x2B,0xA9,0x2B,0xA9,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};	 //min
unsigned char code pic4[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x28,0xAA,0xAB,0xAA,0xAB,0xAA,0x90,
									0xAB,0x90,0xAA,0xAB,0xAA,0xAB,0xAA,0xA8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};	 //max																				
/*------------------------------------------------
                    检测忙位
------------------------------------------------*/
void Check_Busy()
{  
    RS=0;
    RW=1;
    E=1;
    DataPort=0xff;
    while((DataPort&0x80)==0x80);//忙则等待
    E=0;
}
/*------------------------------------------------
                   写命令
------------------------------------------------*/
void Write_Cmd(unsigned char Cmd)
{
	Check_Busy();
	RS=0;
	RW=0;
	E=1;
	DataPort=Cmd;
	DelayUs2x(5);
	E=0;
	DelayUs2x(5);
}
/*------------------------------------------------
                    写数据
------------------------------------------------*/
void Write_Data(unsigned char Data)
{
	Check_Busy();
	RS=1;
	RW=0;
	E=1;
	DataPort=Data;
	DelayUs2x(5);
	E=0;
	DelayUs2x(5);
}

/*------------------------------------------------
                   液晶屏初始化
------------------------------------------------*/
void Init_ST7920()
{  
   DelayMs(40);           //大于40MS的延时程序
   PSB=1;                 //设置为8BIT并口工作模式
   DelayMs(1);            //延时
   RES=0;                 //复位
   DelayMs(1);            //延时
   RES=1;                 //复位置高
   DelayMs(10);
   Write_Cmd(0x30);       //选择基本指令集
   DelayUs2x(50);         //延时大于100us
   Write_Cmd(0x30);       //选择8bit数据流
   DelayUs2x(20);         //延时大于37us
   Write_Cmd(0x0c);       //开显示(无游标、不反白)
   DelayUs2x(50);         //延时大于100us
   Write_Cmd(0x01);       //清除显示，并且设定地址指针为00H
   DelayMs(15);           //延时大于10ms
   Write_Cmd(0x06);       //指定在资料的读取及写入时，设定游标的移动方向及指定显示的移位，光标从右向左加1位移动
   DelayUs2x(50);         //延时大于100us
}
 
/*------------------------------------------------
                 用户自定义字符
------------------------------------------------*/
void CGRAM1()
{ 
     int i;
     Write_Cmd(0x30); 
     Write_Cmd(0x40);
     for(i=0;i<16;i++)
		{
	    Write_Data(pic1[i*2]);
      Write_Data(pic1[i*2+1]);
		}
} 

void CGRAM2()
{ 
     int i;
     Write_Cmd(0x30); 
     Write_Cmd(0x50);
     for(i=0;i<16;i++)
		{
	    Write_Data(pic2[i*2]);
      Write_Data(pic2[i*2+1]);
		}
}

void CGRAM3()
{ 
     int i;
     Write_Cmd(0x30); 
     Write_Cmd(0x60);
     for(i=0;i<16;i++)
		{
	    Write_Data(pic3[i*2]);
      Write_Data(pic3[i*2+1]);
		}
} 

void CGRAM4()
{ 
     int i;
     Write_Cmd(0x30); 
     Write_Cmd(0x70);
     for(i=0;i<16;i++)
		{
	    Write_Data(pic4[i*2]);
      Write_Data(pic4[i*2+1]);
		}
}   	
/*------------------------------------------------
             显示用户自定义字符
------------------------------------------------*/
void DisplayCGRAM(unsigned char x,unsigned char y,unsigned char z)
{ 
 switch(y)
     {
	  case 1: Write_Cmd(0x80+x);break;
	  case 2: Write_Cmd(0x90+x);break;
	  case 3: Write_Cmd(0x88+x);break;
	  case 4: Write_Cmd(0x98+x);break;
      default:break;
	 } 
   Write_Data(0x00);
    switch(z)
     {
	  case 1: Write_Data(0x00);break;
	  case 2: Write_Data(0x02);break;
	  case 3: Write_Data(0x04);break;
	  case 4: Write_Data(0x06);break;
      default:break;
	 }		
}  
/*------------------------------------------------
                   显示字符串
x:横坐标值，范围0~7
y:纵坐标值，范围1~4
------------------------------------------------*/
void LCD_PutString(unsigned char x,unsigned char y,unsigned char code *s)
{ 
 switch(y)
     {
	  case 1: Write_Cmd(0x80+x);break;
	  case 2: Write_Cmd(0x90+x);break;
	  case 3: Write_Cmd(0x88+x);break;
	  case 4: Write_Cmd(0x98+x);break;
      default:break;
	 }
 while(*s>0)
   { 
      Write_Data(*s);
      s++;
      DelayUs2x(50);
   }
}
/*------------------------------------------------
               显示静态的字符
------------------------------------------------*/


void StaticCharacters()
{ 
   CGRAM1();
	 CGRAM2();
	 CGRAM3();
	 CGRAM4();
	 LCD_PutString(0,1,"  温湿度控制器  ");		  //在第一行输出字符串
	 DisplayCGRAM(3,2,1);
	 DisplayCGRAM(3,3,1);
	 DisplayCGRAM(3,4,1);
	 DisplayCGRAM(7,2,2);
	 DisplayCGRAM(7,3,2);						  //显示符号℃,%,min和max
	 DisplayCGRAM(7,4,2);
	 DisplayCGRAM(0,3,3);
	 DisplayCGRAM(4,3,3);
	 DisplayCGRAM(0,4,4);
	 DisplayCGRAM(4,4,4);
} 

/*------------------------------------------------
              显示AM2320所测的数据
------------------------------------------------*/
void TempAndHumi()
{ 
 	  if(f!=1)LCD_PutString(0,2," +");			 
	  else	 
	   { 
		LCD_PutString(0,2," -");				 //输出实时温度前的符号
		temp=tmh&0x7f;
  		temp<<=8;								 //若温度为负值则修正即时温度temp值
   		temp|=tml;
      }
	  Write_Cmd(0x91);
	  Write_Data(0x30+temp/100);
	  Write_Data(0x30+temp%100/10);				 //显示当前温度
	  Write_Data('.');
	  Write_Data(0x30+temp%10);
	  Write_Cmd(0x95);
	  Write_Data(0x30+humi/100);
	  Write_Data(0x30+humi%100/10);				 //显示当前湿度
	  Write_Data('.');
	  Write_Data(0x30+humi%10);
}