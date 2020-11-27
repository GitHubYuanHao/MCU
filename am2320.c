#include"am2320.h"
#include"delay.h"
unsigned char tmh,tml,hmh,hml,crc;
int temp,humi;
bit f;

void amstart()
{
   unsigned char i;
   amport=0;
   i=120;
   while(i--);
   amport=1;
   while(amport);
   while(!amport);
   while(amport);
}
unsigned char amread()
{
   unsigned char n,dat;
   for(n=0;n<8;n++)
   {
    while(!amport);
	  delay10us();
	  delay10us();
	  delay10us();
	  delay10us();
	  dat<<=1;
	  if(amport)
	  {
	     dat|=0x01;
		 while(amport);
	  }
   }
   return dat;
}
void amreaddata()
{
   amstart();
   hmh=amread();
   hml=amread();
   tmh=amread();
   tml=amread();
   crc=amread();
   f=((tmh&0x80)==0x80)?1:0;
   temp=tmh;
   temp<<=8;
   temp|=tml;
   humi=hmh;
   humi<<=8;
   humi|=hml;
}
