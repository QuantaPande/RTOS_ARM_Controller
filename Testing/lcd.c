#include<lpc21xx.h>
#include"lcd.h"
int main(void)
{
	unsigned char u1[] = "Hello World";
	unsigned char u2[] = "Bye Bye World";
	PINSEL1 = 0x00000000;
	IODIR1 = 0x07FF0000;
	LCD_init();
	LCD_string(u1);
	LCD_command(0xC0);
	LCD_string(u2);
}
