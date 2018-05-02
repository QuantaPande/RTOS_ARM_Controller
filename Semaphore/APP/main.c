#include "config.h"
#include "uart.h"
//#include "lcd.h"
#define	TaskStkLength	100						//Define the Task0 stack length 
OS_STK TaskStk0 [TaskStkLength];		//Define the Task0 stack 
OS_STK TaskStk1 [TaskStkLength];		//Define the Task1 stack
OS_EVENT *p;												//Define Semaphore pointer
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
	p = OSSemCreate(1);
	OSStart();
	return 0;															
}

/*******************************************************************************************************
																							Task0
********************************************************************************************************/
void Task0	(void *pdata)
{
	int i;
	INT8U err;
	unsigned char s[]="Waiting for Semaphore (Task 2)\r\n";
	unsigned char t[]="Semaphore acquired (Task 2)\r\n";
	unsigned char u[]="Semaphore released (Task 2)\r\n";
	while(1)
	{
		UART_transmit_string(s);
		OSSemPend(p, 0, &err);
		UART_transmit_string(t);
		for(i=0;i<20;i++)
		{
			IO0SET = 0xFF<<16;
			OSTimeDly(10);
			IO0CLR = 0xFF<<16;
			OSTimeDly(10);
		}
		IO0CLR = 0xFF<<16;
		UART_transmit_string(u);
		OSTimeDly(10);
		OSSemPost(p);
	}
}

/*******************************************************************************************************                          
																							Task1
********************************************************************************************************/

void Task1 (void *pdata)
{
	int i;
	unsigned char s[]="Waiting for Semaphore (Task 1)\r\n";
	unsigned char t[]="Semaphore acquired (Task 1)\r\n";
	unsigned char u[]="Semaphore released (Task 1)\r\n";
	INT8U err;
	while(1)
	{
		UART_transmit_string(s);
		OSSemPend(p, 0, &err);
		UART_transmit_string(t);
		for(i=0;i<16;i++)
		{
			IO0SET = i<<16;
			OSTimeDly(10);
			IO0CLR = i<<16;
			OSTimeDly(10);
		}
		IO0CLR = 0xFF<<16;
		UART_transmit_string(u);
		OSTimeDly(10);
		OSSemPost(p);
	}
}


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
