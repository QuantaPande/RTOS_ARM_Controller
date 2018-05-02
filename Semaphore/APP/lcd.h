#include<lpc21xx.h>
#define rs_en IO1SET = 1<<16
#define rs_dis IO1CLR = 1<<16
#define rw_en IO1SET = 1<<17
#define rw_dis IO1CLR = 1<<17
#define en_en IO1SET = 1<<18
#define en_dis IO1CLR = 1<<18
void delay(unsigned char time)
{
	int i, j;
	for(i=0;i<8002;i++)
		for(j=0;j<time;j++);
}
void lcd_cmd(unsigned char cmd)
{
	rs_dis;
	rw_dis;
	en_en;
	IO0CLR = 0xFF<<8;
	IO0SET = cmd<<8;
	delay(10);
	en_dis;
}

void lcd_data(unsigned char data)
{
	rs_en;
	rw_dis;
	en_en;
	IO0CLR = 0xFF<<8;
	IO0SET = data<<8;
	delay(10);
	en_dis;
}

void LCD_init()
{
	lcd_cmd(0x38);
	lcd_cmd(0x01);
	lcd_cmd(0x06);
	lcd_cmd(0x0E);
	lcd_cmd(0x80);
}

void LCD_string(unsigned char* a)
{
	int length, i;
	//Check length of string
	i=0;
	length=0;
	while(*(a+i)!='\0')
	{
		length++;
		i++;
	}
	if(length<16)
	{
		i=0;
		while(*(a+i)!='\0')
		{
			lcd_data(*(a+i));
			i++;
		}
	}
	else
	{
		i=0;
		for(i=0;i<16;i++)
			lcd_data(*(a+i));
		lcd_cmd(0xC0);
		while(*(a+i)!='\0')
		{
			lcd_data(*(a+i));
			i++;
		}
	}
}