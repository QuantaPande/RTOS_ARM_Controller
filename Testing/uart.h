#include<lpc21xx.h>
#define PCLK 15000000
void UART_Init(int baudrate)
{
	unsigned int regi;
	PINSEL0 = PINSEL0 | 0x00000055;
	U0LCR = 0x83;
	regi = PCLK / (16*baudrate);
	if(regi>256)
	{
		U0DLL = 255;
		U0DLM = (regi - 255)/256;
	}
	else
	{
		U0DLL = regi;
		U0DLM = 0;
	}
	U0LCR = 0x03;
}

void UART_transmit(unsigned char data)
{
	U0THR = data;
	while ((U0LSR & 0x40) == 0);
}

unsigned char UART_receive()
{
	while((U0LSR & 0x01) == 0);
	return(U0RBR);
}

void UART_transmit_string(unsigned char* data)
{
	int i;
	i=0;
	while(*(data+i)!='\0')
	{
		UART_transmit(*(data+i));
		i++;
	}
}

