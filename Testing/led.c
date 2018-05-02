#include<lpc21xx.h>
void delay(unsigned char time)
{
	int i, j;
	for(i=0;i<time;i++)
		for(j=0;j<8002;j++);
}

int main (void)
{
	int i;
	PINSEL0 = 0x00000000;
	IO0DIR = 0x00FF0000;
	while(1)
	{
		for(i=0;i<8;i++)
		{
			IO0SET = 1<<23-i;
			delay(100);
			IO0CLR = 1<<23-i;
			delay(100);
		}
	}
	return 0;
}
