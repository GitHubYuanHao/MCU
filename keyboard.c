#include<reg52.h> 
#include "delay.h"
#include "lcd12864.h"
#include"keyboard.h"
unsigned char code JianPanMa[]={  0x31,0x32,0x33,0x61,						// 1, 2, 3,tl
																	0x34,0x35,0x36,0x62,						// 4, 5, 6,th
																	0x37,0x38,0x39,0x63,						// 7, 8, 9,hl
																	0x30,0x2B,0x2D,0x64,					  // 0, +, -,hh
								};																								// 存放各按键的实际ascll值
bit b;							    //蜂鸣器启动判断位
extern int temp,humi;
unsigned char num,i;
int Tmin,Tmax,Hmin,Hmax;
unsigned char cord_h,cord_l;//行列值中间变量

unsigned char tl[2]={0x31,0x35}; //15
unsigned char th[2]={0x32,0x35}; //25
unsigned char hl[2]={0x35,0x35}; //55
unsigned char hh[2]={0x37,0x35}; //75

unsigned char fhtl[2]={0x20,0x2B}; //+和-的位置
unsigned char fhth[2]={0x20,0x2B}; //+和-的位置
unsigned char fhhl[2]={0x20,0x20}; //+和-的位置
unsigned char fhhh[2]={0x20,0x20}; //+和-的位置
unsigned char ls[4]={0x36,0x2B,0x37,0x30}; //温湿度中间值过渡数组 


/*------------------------------------------------
          显示温湿度上下限的具体数值
------------------------------------------------*/
void MinAMax()						  
{
		    Write_Cmd(0x89);//显示温度min的正负
	  	  Write_Data(fhtl[0]);	  
	  		Write_Data(fhtl[1]);
				Write_Cmd(0x8A);//显示温度min
	  		Write_Data(tl[0]);		  
	  		Write_Data(tl[1]);
				Write_Cmd(0x99);//显示温度max的正负
	  	  Write_Data(fhth[0]);	  
	  		Write_Data(fhth[1]);
	  		Write_Cmd(0x9A);//显示温度max
	  		Write_Data(th[0]);		  
	  		Write_Data(th[1]);
				Write_Cmd(0x8D);//显示湿度min的正负
	  	  Write_Data(fhhl[0]);	  
	  		Write_Data(fhhl[1]);
				Write_Cmd(0x8E);//显示湿度min
	  		Write_Data(hl[0]);	      
	  		Write_Data(hl[1]);
				Write_Cmd(0x9D);//显示湿度度max的符号
	  	  Write_Data(fhhh[0]);	  
	  		Write_Data(fhhh[1]);
				Write_Cmd(0x9E);//显示湿度max
	  		Write_Data(hh[0]);		  
	  		Write_Data(hh[1]);
}
/*------------------------------------------------
          按键扫描函数，返回扫描键值
------------------------------------------------*/
unsigned char KeyScan(void)  //键盘扫描函数，使用行列反转扫描法
{
 
 KeyPort=0x0f;           	//行线输出全为0
 cord_h=KeyPort&0x0f;    	//读入列线值
 if(cord_h!=0x0f)    	 	//先检测有无按键按下
 {
  DelayMs(10);        //去抖
  if((KeyPort&0x0f)!=0x0f)
  {
    cord_h=KeyPort&0x0f;  	//读入列线值
    KeyPort=cord_h|0xf0;  	//输出当前列线值
    cord_l=KeyPort&0xf0;  	//读入行线值

    while((KeyPort&0xf0)!=0xf0);//等待松开并输出

    return(cord_h+cord_l);		//键盘最后组合码值
   }
  }return(0xff);     //返回该值
}
/*------------------------------------------------
              按键值处理函数，返回扫键值
------------------------------------------------*/
unsigned char KeyPro(void)
{
 switch(KeyScan())
 {
  case 0x7e:return 0;break;//1 
  case 0x7d:return 1;break;//2
  case 0x7b:return 2;break;//3
  case 0x77:return 3;break;//温度min
  case 0xbe:return 4;break;//4
  case 0xbd:return 5;break;//5
  case 0xbb:return 6;break;//6
  case 0xb7:return 7;break;//温度max
  case 0xde:return 8;break;//7
  case 0xdd:return 9;break;//8
  case 0xdb:return 10;break;//9
  case 0xd7:return 11;break;//湿度min
  case 0xee:return 12;break;//0
  case 0xed:return 13;break;//+
  case 0xeb:return 14;break;//-
  case 0xe7:return 15;break;//湿度max
  default:return 0xff;break;
 }
}

/*------------------------------------------------
 		温湿度上下限调整并显示函数
------------------------------------------------*/
void UpperLower(void)
{
num=KeyPro();
		while(i<=4)		
	 	{
			if(num!=0xff)
			   {
    			ls[i++]=JianPanMa[num];
    			if(i==1)
					switch(ls[0])
		  			{
  		  			case 0x61:fhtl[0]=0x20;fhtl[1]=0x2A;tl[0]=0x2A;tl[1]=0x2A;break;//指定位置***
							case 0x62:fhth[0]=0x20;fhth[1]=0x2A;th[0]=0x2A;th[1]=0x2A;break;    
          		case 0x63:fhhl[0]=0x20;fhhl[1]=0x2A;hl[0]=0x2A;hl[1]=0x2A;break;
							case 0x64:fhhh[0]=0x20;fhhh[1]=0x2A;hh[0]=0x2A;hh[1]=0x2A;break;
		  			}
				if(ls[1]!=0x2D)ls[1]=0x2B;//只要温度位的符号输入的不是“-”就设为“+”
				if(i==2)
					switch(ls[0])
		  			{
  		  			case 0x61:fhtl[1]=ls[1];break;//温度min
							case 0x62:fhth[1]=ls[1];break;//温度max    给温湿度各位以及符号为赋值
          		case 0x63:fhhl[1]=0x20; break;//湿度min	   湿度符号为恒定为空格0x20
							case 0x64:fhhh[1]=0x20; break;//湿度max
		  			}	
				if(i==3)
					switch(ls[0])
		  			{
  		  			case 0x61:tl[0]=ls[2];break;//温度min
							case 0x62:th[0]=ls[2];break;//温度max  给温湿度各位以及符号为赋值
          		case 0x63:hl[0]=ls[2];break;//湿度min	 湿度符号为恒定为空格0x20
							case 0x64:hh[0]=ls[2];break;//湿度max
		  			}			
				if(i==4)//原本应该为3执行了i++变成4
       				{ 
	     			ls[i-1]=JianPanMa[num];
	      				switch(ls[0])
		  				{
  		  				case 0x61:tl[1]=ls[3];i++;break;//温度min
								case 0x62:th[1]=ls[3];i++;break;//温度max  给温湿度各位以及符号为赋值
          			case 0x63:hl[1]=ls[3];i++;break;//湿度min	 湿度符号为恒定为空格0x20
								case 0x64:hh[1]=ls[3];i++;break;//湿度max
		  				}				   
	  				}												  
				MinAMax();//显示温湿度上下限的数值
					}
			num=KeyPro();//键盘扫描
		}
MinAMax();//显示温湿度上下限的数值
i=0;
}

/*------------------------------------------------
          外部中断0初始化函数
------------------------------------------------*/
void Int0_init()                    //外部中断0初始化函数
{
	EA  = 1;//打开总中断
	IT0 = 1;//ITx = 0;低电平触发  ITx = 1;下降沿触发
	EX0 = 1;//打开外部中断
}
/*------------------------------------------------
          外部中断0服务函数
------------------------------------------------*/
void Int0_isr(void) interrupt 0   
{
	EX0 = 0;//打开外部中断0
	DelayMs(10);
	if(0 == key1)
	{
		while(!key1);//去抖
		DelayMs(10);

           UpperLower();//设置温湿度上下限
    }
	EX0 = 1;//关闭外部中断
}
/*------------------------------------------------
      温湿度上下限与实时值比较处理函数
------------------------------------------------*/
void THcompare(void)
{			  
Tmin=100*(tl[0]-0x30)+10*(tl[1]-0x30);//温度min
Tmax=100*(th[0]-0x30)+10*(th[1]-0x30);//温度min
Hmin=100*(hl[0]-0x30)+10*(hl[1]-0x30);//温度min
Hmax=100*(hh[0]-0x30)+10*(hh[1]-0x30);//温度min
		  		
if(fhtl[1]==0x2D)Tmin=-Tmin;//如果输入的符号为-
if(fhth[1]==0x2D)Tmax=-Tmax;//则将温度对应上下限取负值

if(temp<Tmin)led1=0;
		else led1=1;
if(temp>Tmax)led2=0;
		else led2=1;		 //继电器与led判断
if(humi<Hmin)led3=0;	
		else led3=1;
if(humi>Hmax)led4=0;	
		else led4=1;					 
b=led1&led2&led3&led4;//蜂鸣器判断
if(!b)SPK=0;
else SPK=1;
}