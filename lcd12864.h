#ifndef __lcd_h_
#define __lcd_h_
#define uchar unsigned char
#define DataPort P0
sbit PSB=P2^1;
sbit RES=P2^3;
sbit RS=P2^4;
sbit RW=P2^5;
sbit E=P2^6;
sbit key1=P3^2;
sbit led1=P3^4;
sbit led2=P3^5;
sbit led3=P3^6;
sbit led4=P3^7;
void CGRAM1();
void CGRAM2();
void CGRAM3();
void CGRAM4();
void Check_Busy();
void Init_ST7920();
void TempAndHumi();
void StaticCharacters();
void Write_Cmd(unsigned char Cmd);
void Write_Data(unsigned char Data);
void DisplayCGRAM(unsigned char x,unsigned char y,unsigned char z);
void LCD_PutString(unsigned char x,unsigned char y,unsigned char code *s);
#endif