#include "config.h"
#include "uart.h"
//#include "lcd.h"
#define	TaskStkLength	100						//Define the Task0 stack length 
OS_STK TaskStk0 [TaskStkLength];		//Define the Task0 stack 
OS_STK TaskStk1 [TaskStkLength];		//Define the Task1 stack
OS_EVENT *p;												//Define Mailbox pointer
void Task0(void *pdata);						//Task0 
void Task1(void *pdata);						//Task1

int main (void)
{
	TargetInit ();
	//PINSEL0 = 0x00000055;
	IO0DIR = IO0DIR | 0xFF<<16;
	UART_Init(9600);
	//LCD_init();
	OSInit();
	OSTaskCreate(Task0, (void *)0, &TaskStk0[TaskStkLength - 1], 0);
	OSTaskCreate(Task1, (void *)0, &TaskStk1[TaskStkLength - 1], 1);
	p = OSMboxCreate(0);
	OSStart();
	return 0;															
}
/*******************************************************************************************************                          
																							Task0 
********************************************************************************************************/

void Task0 (void *pdata)
{
	unsigned char s[]="Executing Task 0)\r\n";
	unsigned char t[]="Posting the message\r\n";
	unsigned char u[]="Hello World!\r\n";
	INT8U err;
	while(1)
	{
		UART_transmit_string(s);
		UART_transmit_string(t);
		OSMboxPost(p, u);
		OSTimeDly(1000);
	}
}

/*******************************************************************************************************
																							Task1 
********************************************************************************************************/
void Task1	(void *pdata)
{
	INT8U err;
	unsigned char s[]="Executing the Task 1\r\n";
	unsigned char t[]="Reading Message from Mailbox\r\n";
	unsigned char r[]="Message is : ";
	unsigned char* u;
	while(1)
	{
		UART_transmit_string(s);
		u = OSMboxPend(p, 0, &err);
		UART_transmit_string(t);
		UART_transmit_string(r);
		UART_transmit_string(u);
		OSTimeDly(1000);
	}
}


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
