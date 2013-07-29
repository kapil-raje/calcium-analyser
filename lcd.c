#include "HardwareProfile.h"
#include "LCD.h"

#include <avr/io.h>
#include <util/delay.h>





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
void lcd16x2_write_string (char * str )
{
	while(*str != '\0')
	{
		lcd16x2_write (*str,0x82) ;
		str++;
	}
	
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
void lcd16x2_clear (void)
{
	lcd16x2_write (0x01,0x81) ; //LCD clear
}
//--------------------------------------------------------------------------
