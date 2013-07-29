#include "lcd16x2.h"
#include <avr/io.h>
#include <util/delay.h>

#define KBD_PORT_DIR DDRA
#define KBD_PORT_IN PINA
#define KBD_PORT_OUT PORTA

unsigned char getkey(void);

void main(void)
{
  unsigned char temp;
	LCD_CTRL_DIR = (1<<LCD_EN|1<<LCD_RW|1<<LCD_RS);
	LCD_CTRL &= ~(1<<LCD_EN);

	lcd16x2_init() ;
	lcd16x2_cursor_to (1,0) ;	//Set DDRAM Address.
	
	for (temp = 0x41; temp < 0x51; temp++)
		lcd16x2_write (temp,0x82) ;
	lcd16x2_cursor_to (2,0) ;
	for (temp = 0x51;temp < 0x5B;temp++)
		lcd16x2_write (temp,0x82) ; 

	while(1)
	{
		lcd16x2_cursor_to(1,0);	
		lcd16x2_write(getkey(),0x82);
	}
}


void lcd16x2_write (unsigned char writethis,unsigned char command)
{
	switch (command & 0x03)
	{
		case 1:			//Write to command register
			LCD_CTRL &= ~(1<<LCD_RS);	//Select command register
		break ;

		case 2:			//Write to data register
			LCD_CTRL |= (1<<LCD_RS);
		break ;
	}
	LCD_DATA_DIR = 0xFF;
	_delay_us(1);
	LCD_DATA = writethis;	//Data on data port
	_delay_us(1);	
	LCD_CTRL &= ~(1<<LCD_RW);	//RW is low
	_delay_us(2);
	LCD_CTRL |= (1<<LCD_EN); //Enable high
	_delay_us(2);
	LCD_CTRL &= ~(1<<LCD_EN);
	_delay_us(1);
	LCD_CTRL |= (1<<LCD_RW);
	
	if ((command & 0x80) == 0x80)
		while (lcd16x2_get_ready() != 0) ;
	else
		lcd16x2_delay() ;
}

//--------------------------------------------------------------------------

unsigned char lcd16x2_get_ready (void)
{
	volatile unsigned char l_temp,l_temp2 ;
	unsigned int l_temp1 = 0 ;
	LCD_CTRL &= ~(1<<LCD_RS);					
	LCD_CTRL |= (1<<LCD_RW);	
	LCD_DATA_DIR = 0x00;
	while (l_temp1 != 65535)
	{

		LCD_CTRL |= (1<<LCD_EN);
		for(l_temp2 = 0; l_temp2 < 200; l_temp2++)
			l_temp2++;
		l_temp = LCD_DATA_READ ;		
		LCD_CTRL &= ~(1<<LCD_EN);
		if ((l_temp & 0x80) != 0x80)return (0) ;
		l_temp1++ ;
	}
	return (1) ;		//LCD is not ready
}

//--------------------------------------------------------------------------

void lcd16x2_cursor_to (unsigned char line, unsigned char location)
{
	if (location > 0x8F)location &= 0xcF ;
	else location &= 0x8F ;

	if (line == 1)
		lcd16x2_write (0x80+location,0x81) ;
	else
		lcd16x2_write (0xC0+location,0x81) ;
}

//--------------------------------------------------------------------------

void lcd16x2_delay (void)
{
	volatile unsigned int l_temp ;
	l_temp = 0 ;
	while (l_temp != 65535)l_temp++ ;
	l_temp = 0 ;
	while (l_temp != 65535)l_temp++ ;
	
}
//--------------------------------------------------------------------------
void lcd16x2_init (void)
{
	unsigned char l_temp ;

	for (l_temp = 0; l_temp < 8; l_temp++)
		lcd16x2_delay() ;

	for (l_temp = 0; l_temp < 4; l_temp++)
	{
		lcd16x2_write (0x30,0x01) ;
		lcd16x2_delay() ;
		lcd16x2_delay() ;
	}
	lcd16x2_write (0x38,0x81) ;	//2 lines,5x8 dot font
	lcd16x2_write (0x08,0x81) ;//Display off
	lcd16x2_write (0x01,0x81) ; //LCD clear
	lcd16x2_write (0x80,0x81) ;	//DDRAM Address set to 0.
	lcd16x2_write (0x0E,0x81) ; //Display on, cursor on,blink oFF
}

//--------------------------------------------------------------------------

unsigned char getkey(void)
{
	unsigned char kbd_lookup[4][3] = {
										{'0','1','2'},
									    {'3','4','5'},
										{'6','7','8'},
										{'9','.','E'}
	};
							
	volatile unsigned char row,column,temp;
	
	KBD_PORT_OUT = 0x87;			//PA6,PC5,PC4,PC3 are rows (are low)		
	_delay_us(1);
	KBD_PORT_DIR = 0x78;			//PC6,PC5,PC4,PC3 are rows (are outputs)	
	_delay_ms(2);
	column = KBD_PORT_IN;
	
	while((column & 0x07) != 0x07)		//Wait till all keys are released
		column = KBD_PORT_IN;
	
	while(1)
	{
		column = KBD_PORT_IN;		
		while((column & 0x07) == 0x07)
			column = KBD_PORT_IN;
		
		_delay_ms(20);				//debounce
		column &= 0x07;
		temp = KBD_PORT_IN;
		temp &= 0x07;
		if(temp == column)
			break;
	}
	column &= 0x07;
	column ^= 0x07;
	
	KBD_PORT_OUT = 0x78;		//PC2,PC1,PC0 are column (are low)	
	_delay_us(1);	
	KBD_PORT_DIR = 0x87;		//PC2,PC1,PC0 are column (are output)	
	_delay_ms(2);
	
	row = KBD_PORT_IN;
	row &= 0x78;
	row ^= 0x78;
	row >>= 3;			//KBD_PORT_IN.0 is unconnected
	temp = row;
	row = 0xFF;
	while(temp != 0)
	{
		temp >>= 1;
		row++;
	}
	temp = column;
	column = 0xFF;
	while(temp != 0)
	{
		temp >>= 1;
		column++;
	}
	
	return (kbd_lookup[row][column]);
}
