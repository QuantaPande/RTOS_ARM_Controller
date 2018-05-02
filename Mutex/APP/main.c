#include "config.h"
#include "uart.h"
//#include "lcd.h"
#define	TaskStkLength	100						//Define the Task0 stack length 
OS_STK TaskStk0 [TaskStkLength];		//Define the Task0 stack 
OS_STK TaskStk1 [TaskStkLength];		//Define the Task1 stack
OS_STK TaskStk2 [TaskStkLength];		//Define the Task2 stack
OS_EVENT *p;												//Define Mutex1 pointer
void Task0(void *pdata);						//Task0 
void Task1(void *pdata);						//Task1
void Task2(void *pdata);						//Task3
int main (void)
{
	INT8U errp;
	TargetInit ();
	//PINSEL0 = 0x00000055;
	IO0DIR = IO0DIR | 0xFF<<16;
	UART_Init(9600);
	//LCD_init();
	OSInit();
	p = OSMutexCreate(0, &errp);
	OSTaskCreate(Task0, (void *)0, &TaskStk0[TaskStkLength - 1], 1);
	OSTaskCreate(Task1, (void *)0, &TaskStk1[TaskStkLength - 1], 2);
	OSTaskCreate(Task2, (void *)0, &TaskStk2[TaskStkLength - 1], 3);
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
	unsigned char s[]="Waiting for Mutex (Task 1)\r\n";
	unsigned char t[]="Mutex acquired (Task 1)\r\n";
	unsigned char u[]="Mutex released (Task 1)\r\n";
	while(1)
	{
		UART_transmit_string(s);
		OSMutexPend(p, 0, &err);
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
		OSMutexPost(p);
	}
}

/*******************************************************************************************************                          
																							Task1
********************************************************************************************************/

void Task1 (void *pdata)
{
	int i;
	unsigned char s[]="Waiting for Mutex (Task 2)\r\n";
	unsigned char t[]="Mutex acquired (Task 2)\r\n";
	unsigned char u[]="Mutex released (Task 2)\r\n";
	INT8U err;
	while(1)
	{
		UART_transmit_string(s);
		OSMutexPend(p, 0, &err);
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
		OSMutexPost(p);
	}
}

/*******************************************************************************************************                          
																							Task2
********************************************************************************************************/

void Task2 (void *pdata)
{
	int i;
	unsigned char s[]="Waiting for Mutex (Task 3)\r\n";
	unsigned char t[]="Mutex acquired (Task 3)\r\n";
	unsigned char u[]="Mutex released (Task 3)\r\n";
	INT8U err;
	while(1)
	{
		UART_transmit_string(s);
		OSMutexPend(p, 0, &err);
		UART_transmit_string(t);
		for(i=0;i<8;i++)
		{
			IO0SET = i<<16+i;
			OSTimeDly(10);
			IO0CLR = i<<16+i;
			OSTimeDly(10);
		}
		IO0CLR = 0xFF<<16;
		for(i=0;i<8;i++)
		{
			IO0SET = i<<23-i;
			OSTimeDly(10);
			IO0CLR = i<<23-i;
			OSTimeDly(10);
		}
		IO0CLR = 0xFF<<16;
		UART_transmit_string(u);
		OSTimeDly(10);
		OSMutexPost(p);
	}
}


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
