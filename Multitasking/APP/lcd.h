//Include Controller specific header file
#include <lpc21xx.h> 

//Function to generate software delay
//Calibrated to 1ms
void  delay_ms(unsigned char time)	   
{  
 unsigned int  i, j;
 for (j=0; j<time; j++)
		for(i=0; i<8002; i++);
}

void LCD_command(unsigned char command)
{
	IO1CLR = 0xFF<<19;	// Clear LCD Data lines
	IO1CLR=1<<16;		 		// RS=0 for command
	IO1CLR=1<<17;		 		// RW=0 for write
	IO1SET=command<<19;	// put command on data line
	IO1SET=(1<<18);			// en=1 
	delay_ms(10) ;			// delay
	IO1CLR=(1<<18);	 		// en=0
}

void LCD_data(unsigned char data)
{
	IO1CLR = 0xFF<<19;	// Clear LCD Data lines
	IO1SET=1<<16;		 		// RS=1 for data
	IO1CLR=1<<17;		 		// RW=0 for write
	IO1SET= data<<19; 	// put command on data line
	IO1SET=(1<<18);			//en=1 
	delay_ms(10) ;	  	//delay
	IO1CLR=(1<<18);			//en=0
 }

void LCD_init()
{
	LCD_command(0x38);	//8bit mode and 5x8 dotes (function set)
	delay_ms(10) ;			// delay
	LCD_command(0x0c);	//display on, cursor off, cursor char blinking off(display on/off)
	delay_ms(10) ;			// delay
	LCD_command(0x06);  //cursor increament and display shift(entry mode set)
	delay_ms(10) ;			// delay
	LCD_command(0x01); 	//clear lcd(clear command)
	delay_ms(10) ;			// delay
	LCD_command(0x80); 	//set cursor to 0th location	1st lne
	
}
 
void LCD_string(unsigned char *string)
{
  while(*string)			//Check for End of String
  LCD_data(*string++); 	//sending data on LCD byte by byte
}
