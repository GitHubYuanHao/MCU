#include<reg52.h> 
#include "delay.h"
#include "lcd12864.h"
#include"keyboard.h"
unsigned char code JianPanMa[]={  0x31,0x32,0x33,0x61,						// 1, 2, 3,tl
																	0x34,0x35,0x36,0x62,						// 4, 5, 6,th
																	0x37,0x38,0x39,0x63,						// 7, 8, 9,hl
																	0x30,0x2B,0x2D,0x64,					  // 0, +, -,hh
								};																								// ��Ÿ�������ʵ��ascllֵ
bit b;							    //�����������ж�λ
extern int temp,humi;
unsigned char num,i;
int Tmin,Tmax,Hmin,Hmax;
unsigned char cord_h,cord_l;//����ֵ�м����

unsigned char tl[2]={0x31,0x35}; //15
unsigned char th[2]={0x32,0x35}; //25
unsigned char hl[2]={0x35,0x35}; //55
unsigned char hh[2]={0x37,0x35}; //75

unsigned char fhtl[2]={0x20,0x2B}; //+��-��λ��
unsigned char fhth[2]={0x20,0x2B}; //+��-��λ��
unsigned char fhhl[2]={0x20,0x20}; //+��-��λ��
unsigned char fhhh[2]={0x20,0x20}; //+��-��λ��
unsigned char ls[4]={0x36,0x2B,0x37,0x30}; //��ʪ���м�ֵ�������� 


/*------------------------------------------------
          ��ʾ��ʪ�������޵ľ�����ֵ
------------------------------------------------*/
void MinAMax()						  
{
		    Write_Cmd(0x89);//��ʾ�¶�min������
	  	  Write_Data(fhtl[0]);	  
	  		Write_Data(fhtl[1]);
				Write_Cmd(0x8A);//��ʾ�¶�min
	  		Write_Data(tl[0]);		  
	  		Write_Data(tl[1]);
				Write_Cmd(0x99);//��ʾ�¶�max������
	  	  Write_Data(fhth[0]);	  
	  		Write_Data(fhth[1]);
	  		Write_Cmd(0x9A);//��ʾ�¶�max
	  		Write_Data(th[0]);		  
	  		Write_Data(th[1]);
				Write_Cmd(0x8D);//��ʾʪ��min������
	  	  Write_Data(fhhl[0]);	  
	  		Write_Data(fhhl[1]);
				Write_Cmd(0x8E);//��ʾʪ��min
	  		Write_Data(hl[0]);	      
	  		Write_Data(hl[1]);
				Write_Cmd(0x9D);//��ʾʪ�ȶ�max�ķ���
	  	  Write_Data(fhhh[0]);	  
	  		Write_Data(fhhh[1]);
				Write_Cmd(0x9E);//��ʾʪ��max
	  		Write_Data(hh[0]);		  
	  		Write_Data(hh[1]);
}
/*------------------------------------------------
          ����ɨ�躯��������ɨ���ֵ
------------------------------------------------*/
unsigned char KeyScan(void)  //����ɨ�躯����ʹ�����з�תɨ�跨
{
 
 KeyPort=0x0f;           	//�������ȫΪ0
 cord_h=KeyPort&0x0f;    	//��������ֵ
 if(cord_h!=0x0f)    	 	//�ȼ�����ް�������
 {
  DelayMs(10);        //ȥ��
  if((KeyPort&0x0f)!=0x0f)
  {
    cord_h=KeyPort&0x0f;  	//��������ֵ
    KeyPort=cord_h|0xf0;  	//�����ǰ����ֵ
    cord_l=KeyPort&0xf0;  	//��������ֵ

    while((KeyPort&0xf0)!=0xf0);//�ȴ��ɿ������

    return(cord_h+cord_l);		//������������ֵ
   }
  }return(0xff);     //���ظ�ֵ
}
/*------------------------------------------------
              ����ֵ������������ɨ��ֵ
------------------------------------------------*/
unsigned char KeyPro(void)
{
 switch(KeyScan())
 {
  case 0x7e:return 0;break;//1 
  case 0x7d:return 1;break;//2
  case 0x7b:return 2;break;//3
  case 0x77:return 3;break;//�¶�min
  case 0xbe:return 4;break;//4
  case 0xbd:return 5;break;//5
  case 0xbb:return 6;break;//6
  case 0xb7:return 7;break;//�¶�max
  case 0xde:return 8;break;//7
  case 0xdd:return 9;break;//8
  case 0xdb:return 10;break;//9
  case 0xd7:return 11;break;//ʪ��min
  case 0xee:return 12;break;//0
  case 0xed:return 13;break;//+
  case 0xeb:return 14;break;//-
  case 0xe7:return 15;break;//ʪ��max
  default:return 0xff;break;
 }
}

/*------------------------------------------------
 		��ʪ�������޵�������ʾ����
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
  		  			case 0x61:fhtl[0]=0x20;fhtl[1]=0x2A;tl[0]=0x2A;tl[1]=0x2A;break;//ָ��λ��***
							case 0x62:fhth[0]=0x20;fhth[1]=0x2A;th[0]=0x2A;th[1]=0x2A;break;    
          		case 0x63:fhhl[0]=0x20;fhhl[1]=0x2A;hl[0]=0x2A;hl[1]=0x2A;break;
							case 0x64:fhhh[0]=0x20;fhhh[1]=0x2A;hh[0]=0x2A;hh[1]=0x2A;break;
		  			}
				if(ls[1]!=0x2D)ls[1]=0x2B;//ֻҪ�¶�λ�ķ�������Ĳ��ǡ�-������Ϊ��+��
				if(i==2)
					switch(ls[0])
		  			{
  		  			case 0x61:fhtl[1]=ls[1];break;//�¶�min
							case 0x62:fhth[1]=ls[1];break;//�¶�max    ����ʪ�ȸ�λ�Լ�����Ϊ��ֵ
          		case 0x63:fhhl[1]=0x20; break;//ʪ��min	   ʪ�ȷ���Ϊ�㶨Ϊ�ո�0x20
							case 0x64:fhhh[1]=0x20; break;//ʪ��max
		  			}	
				if(i==3)
					switch(ls[0])
		  			{
  		  			case 0x61:tl[0]=ls[2];break;//�¶�min
							case 0x62:th[0]=ls[2];break;//�¶�max  ����ʪ�ȸ�λ�Լ�����Ϊ��ֵ
          		case 0x63:hl[0]=ls[2];break;//ʪ��min	 ʪ�ȷ���Ϊ�㶨Ϊ�ո�0x20
							case 0x64:hh[0]=ls[2];break;//ʪ��max
		  			}			
				if(i==4)//ԭ��Ӧ��Ϊ3ִ����i++���4
       				{ 
	     			ls[i-1]=JianPanMa[num];
	      				switch(ls[0])
		  				{
  		  				case 0x61:tl[1]=ls[3];i++;break;//�¶�min
								case 0x62:th[1]=ls[3];i++;break;//�¶�max  ����ʪ�ȸ�λ�Լ�����Ϊ��ֵ
          			case 0x63:hl[1]=ls[3];i++;break;//ʪ��min	 ʪ�ȷ���Ϊ�㶨Ϊ�ո�0x20
								case 0x64:hh[1]=ls[3];i++;break;//ʪ��max
		  				}				   
	  				}												  
				MinAMax();//��ʾ��ʪ�������޵���ֵ
					}
			num=KeyPro();//����ɨ��
		}
MinAMax();//��ʾ��ʪ�������޵���ֵ
i=0;
}

/*------------------------------------------------
          �ⲿ�ж�0��ʼ������
------------------------------------------------*/
void Int0_init()                    //�ⲿ�ж�0��ʼ������
{
	EA  = 1;//�����ж�
	IT0 = 1;//ITx = 0;�͵�ƽ����  ITx = 1;�½��ش���
	EX0 = 1;//���ⲿ�ж�
}
/*------------------------------------------------
          �ⲿ�ж�0������
------------------------------------------------*/
void Int0_isr(void) interrupt 0   
{
	EX0 = 0;//���ⲿ�ж�0
	DelayMs(10);
	if(0 == key1)
	{
		while(!key1);//ȥ��
		DelayMs(10);

           UpperLower();//������ʪ��������
    }
	EX0 = 1;//�ر��ⲿ�ж�
}
/*------------------------------------------------
      ��ʪ����������ʵʱֵ�Ƚϴ�����
------------------------------------------------*/
void THcompare(void)
{			  
Tmin=100*(tl[0]-0x30)+10*(tl[1]-0x30);//�¶�min
Tmax=100*(th[0]-0x30)+10*(th[1]-0x30);//�¶�min
Hmin=100*(hl[0]-0x30)+10*(hl[1]-0x30);//�¶�min
Hmax=100*(hh[0]-0x30)+10*(hh[1]-0x30);//�¶�min
		  		
if(fhtl[1]==0x2D)Tmin=-Tmin;//�������ķ���Ϊ-
if(fhth[1]==0x2D)Tmax=-Tmax;//���¶ȶ�Ӧ������ȡ��ֵ

if(temp<Tmin)led1=0;
		else led1=1;
if(temp>Tmax)led2=0;
		else led2=1;		 //�̵�����led�ж�
if(humi<Hmin)led3=0;	
		else led3=1;
if(humi>Hmax)led4=0;	
		else led4=1;					 
b=led1&led2&led3&led4;//�������ж�
if(!b)SPK=0;
else SPK=1;
}