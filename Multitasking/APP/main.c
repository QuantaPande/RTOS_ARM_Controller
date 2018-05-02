#include "config.h"
#include "uart.h"
#include "lcd.h"
#define	TaskStkLength	100			      //Define the Task0 stack length 
OS_STK TaskStk0 [TaskStkLength];		//Define the Task0 stack 
OS_STK TaskStk1 [TaskStkLength];		//Define the Task1 stack
OS_STK TaskStk2 [TaskStkLength];		//Define the Task2 stack
void Task0(void *pdata);					//Task0 
void Task1(void *pdata);					//Task1
void Task2(void *pdata);					//Task2
int main (void)
{
	TargetInit ();
	PINSEL0 = 0x00000055;
	IO0DIR = 0x00FF0000;
	PINSEL1 = 0x00000000;
	IO1DIR = 0x07FF0000; 
	UART_Init(9600);
	OSInit();
	OSTaskCreate(Task0, (void*)0, &TaskStk0[TaskStkLength-1], 0);
	OSTaskCreate(Task1, (void*)0, &TaskStk1[TaskStkLength-1], 1);
	OSTaskCreate(Task2, (void*)0, &TaskStk2[TaskStkLength-1], 2);
	OSStart();
	return 0;															
}
/*********************************************************************************************************
**                            Task0 
********************************************************************************************************/

void Task0	(void *pdata)
{
	unsigned char i;
	while(1)
	{
		for(i=0;i<8;i++)
		{
			IO0SET = 0xFF<<16;
			OSTimeDly(10);
			IO0CLR = 0xFF<<16;
			OSTimeDly(10);
		}
		IO0CLR = 0xFF<<16;
		OSTimeDly(10);
	}
}

/*********************************************************************************************************
**                            Task1 
********************************************************************************************************/

void Task1(void *pdata)
{
	unsigned char u1[] = "Hello World!\r\n";
	unsigned char u2[] = "Bye Bye World!\r\n";
	while(1)
	{
		UART_transmit_string(u1);
		OSTimeDly(100);
		UART_transmit_string(u2);
		OSTimeDly(100);
	}
}

/*********************************************************************************************************
**                            Task2
********************************************************************************************************/

void Task2(void *pdata)
{
	unsigned char u1[] = "Hello World!\r\n";
	unsigned char u2[] = "Bye Bye World!\r\n";
	LCD_init();
	while(1)
	{
		LCD_string(u1);
		OSTimeDly(100);
		LCD_command(0xC0);
		LCD_string(u2);
		OSTimeDly(100);
	}
}

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
