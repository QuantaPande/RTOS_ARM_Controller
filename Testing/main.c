#include<lpc21xx.h>
#include "uart.h"
int main(void)
{
	unsigned char s[] ="Testing 1....2....3....";
	UART_Init(9600);
	UART_transmit_string(s);
	return 0;
}