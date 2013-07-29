/*
* ADC_Project.c
*
* Created: 26/03/2013 4:46:22 PM
*  Author: Kanchan


*/

#include "HardwareProfile.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "UART.h"
#include "ADC.h"
#include "LCD.h"



unsigned char getkey(void);

void FloatToStringNew(char *str, float f, char size);
void lcd16x2_write (unsigned char writethis,unsigned char command);
unsigned char lcd16x2_get_ready (void);
void lcd16x2_cursor_to (unsigned char line, unsigned char location);
void lcd16x2_delay (void);
void lcd16x2_init (void);
void lcd16x2_write_string (char * );


int main(void)
{
  uint8_t theLow=0 ;
	uint16_t tenBitValue=0,distWaterTrVol=0, reagBlankTrVol=0,stdTrVol=0,sampleTrVol=0;
	float reagBlankAbsorb=0.0,stdAbsorb=0.0,sampleAbsorb=0.0,calConc=0.0,theFactor = 0.0;
	float theResult=0.0;
	char myBuffer[80];
	
	
	UART_Init ( MYUBRR );
	ADC_Init();
	
	//**** Declerations for LCD  ****//
	
	LCD_CTRL_DIR = (1<<LCD_EN|1<<LCD_RW|1<<LCD_RS);
	LCD_CTRL &= ~(1<<LCD_EN);

	DisplayString("LCD Initializing\r\n");
	lcd16x2_init() ;
	DisplayString("LCD Initialized\r\n");
	lcd16x2_cursor_to (1,0) ;	
	lcd16x2_write_string ("Calcium Analyzer");
	_delay_ms(1000);
	
	while(1)
	{
		
#ifdef LCD_DISPLAY  		
		lcd16x2_clear();
		lcd16x2_cursor_to(1,0);
		lcd16x2_write_string ("1.Initial Process");
		lcd16x2_cursor_to(2,0);
		lcd16x2_write_string ("2.Take Sample");	
#endif	
	
#ifdef UART_DISPLAY
		DisplayString("Menu ===>\r\n");
		DisplayString("\r\n 1.Initial Process\t2.Take Sample\r\n");
		DisplayString("\r\nPlease Make sure,You are Done with Connections Before each Key Press!!!!  \r\n");
#endif

#if 1	
	    switch( getkey() )
		{
			case 0x31:
#ifdef LCD_DISPLAY			
			lcd16x2_clear();
			lcd16x2_cursor_to(1,0);	
			lcd16x2_write_string ("1.DistWaterBlank");
#endif	

#ifdef UART_DISPLAY
			DisplayString("\r\n1.Distilled water Blank");
#endif					

			if(0x31 == getkey() )
			{
				//**** Start ADC Conversion  ****//
				ADC_Init();
				ADCSRA |= 1<<ADSC;
				_delay_ms(1000);
				theLow = ADCL ;
				tenBitValue = (ADCH<< 2) | (theLow >> 6) ;
				distWaterTrVol = tenBitValue ;
				
				sprintf(myBuffer,"=>%d",distWaterTrVol);
				
#ifdef LCD_DISPLAY				
				lcd16x2_cursor_to(2,0);
				lcd16x2_write_string(myBuffer);
#endif
#ifdef UART_DISPLAY
				DisplayString(myBuffer);
#endif			
				_delay_ms(5000);
				
#ifdef LCD_DISPLAY
				lcd16x2_clear();
				lcd16x2_cursor_to(1,0);	
				lcd16x2_write_string("2.ReagBlkReading");
#endif
#ifdef UART_DISPLAY
				DisplayString("\r\n2.Reagent Blank Reading");
#endif
				
				if(0x32 ==  getkey() )
				{
					//**** Start ADC Conversion  ****//
					ADC_Init();
					ADCSRA |= 1<<ADSC;
					_delay_ms(1000);
					theLow = ADCL ;
					tenBitValue = (ADCH<< 2) | (theLow >> 6) ;
					reagBlankTrVol = tenBitValue;
					theResult = (float)distWaterTrVol/(float)reagBlankTrVol ;
					
					//**** Calculate Reagent Blank Absorbance  ****//
					reagBlankAbsorb =(float)log10(theResult) ;
					
					sprintf(myBuffer,"=>%d",reagBlankTrVol);
#ifdef LCD_DISPLAY					
					lcd16x2_cursor_to(2,0);
					lcd16x2_write_string(myBuffer);
#endif					
#ifdef UART_DISPLAY					
					DisplayString(myBuffer);
#endif				
					_delay_ms(5000);
					
#ifdef LCD_DISPLAY				
					lcd16x2_clear();
					lcd16x2_cursor_to(1,0);
					lcd16x2_write_string("ReagBlkAbsrb");
#endif					
#ifdef UART_DISPLAY	
					DisplayString("\r\nReagentBlankAbsorb=");
					
#endif					
					memset(myBuffer,0,sizeof(myBuffer));
					FloatToStringNew(myBuffer,reagBlankAbsorb,6);
#ifdef LCD_DISPLAY					
					lcd16x2_cursor_to(2,0);
					lcd16x2_write_string(myBuffer);
#endif
#ifdef UART_DISPLAY					
					DisplayString(myBuffer);
#endif				
					memset(myBuffer,0,sizeof(myBuffer));
					_delay_ms(5000);
					
#ifdef LCD_DISPLAY					
					lcd16x2_clear();
					lcd16x2_cursor_to(1,0);
					lcd16x2_write_string("3.Std Reading");
#endif
#ifdef UART_DISPLAY
					DisplayString("\r\n3.Standard Reading");
#endif					
					
					if(0x33 == getkey() )
					{
						//**** Start ADC Conversion  ****//
						ADC_Init();
						ADCSRA |= 1<<ADSC;
						_delay_ms(1000);
						theLow = ADCL ;
						tenBitValue = (ADCH<< 2) | (theLow >> 6) ;
						stdTrVol = tenBitValue;
						theResult = (float)distWaterTrVol/(float)stdTrVol;
						
						//**** Calculate Standard ReaAbsorbance  ****//
						stdAbsorb = (float)log10( theResult );
						
						sprintf(myBuffer,"=>%d",stdTrVol);	
#ifdef LCD_DISPLAY						
						lcd16x2_cursor_to(2,0);
						lcd16x2_write_string(myBuffer);
#endif						
#ifdef UART_DISPLAY
						DisplayString(myBuffer);
#endif						
						
						_delay_ms(5000);
						
#ifdef LCD_DISPLAY						
						lcd16x2_clear();
						lcd16x2_cursor_to(1,0);
						lcd16x2_write_string("Std Absorbance");
#endif	

#ifdef UART_DISPLAY
						DisplayString("\r\nStandard Absorb=");
#endif					
						
						memset(myBuffer,0,sizeof(myBuffer));
						FloatToStringNew(myBuffer,stdAbsorb,6);
#ifdef LCD_DISPLAY						
						lcd16x2_cursor_to(2,0);
						lcd16x2_write_string(myBuffer);
#endif
#ifdef UART_DISPLAY						
						DisplayString(myBuffer);
#endif						
						
						theFactor = 10.0 / (stdAbsorb - reagBlankAbsorb) ;
						
						_delay_ms(5000);
						
						
#ifdef LCD_DISPLAY						
						lcd16x2_clear();
						lcd16x2_cursor_to(1,0);
						lcd16x2_write_string("Factor=");
#endif						
#ifdef UART_DISPLAY
						DisplayString("\r\nFactor=");
#endif						
						memset(myBuffer,0,sizeof(myBuffer));
						FloatToStringNew(myBuffer,theFactor,6);
#ifdef LCD_DISPLAY						
						lcd16x2_cursor_to(2,0);
						lcd16x2_write_string(myBuffer);
#endif
#ifdef UART_DISPLAY						
						DisplayString(myBuffer);
#endif						
						if(0 != getkey())
						break;
						
					}
					else
					{
						lcd16x2_clear();
						lcd16x2_cursor_to(2,0);
						lcd16x2_write_string("Wrong Choice");
						_delay_ms(300);
						break;
					}
				}
				else
				{
					lcd16x2_clear();
					lcd16x2_cursor_to(2,0);
					lcd16x2_write_string("Wrong Choice");
					_delay_ms(300);
					break;
				}
			}
			else
			{
				lcd16x2_clear();
				lcd16x2_cursor_to(2,0);
				lcd16x2_write_string("Wrong Choice");
				_delay_ms(300);
			}
			break;
			case 0x32:
			if( (theFactor != 0) && (distWaterTrVol != 0) )
			{
#ifdef LCD_DISPLAY
				lcd16x2_clear();
				lcd16x2_cursor_to(1,0);
				lcd16x2_write_string("1.ReagBlkReading");
#endif
#ifdef UART_DISPLAY				
				DisplayString("\r\n1.Reagent Blank Reading");
#endif
				
				if(0x31 == getkey() )
				{
					ADC_Init();
					ADCSRA |= 1<<ADSC;
					_delay_ms(1000);
					theLow = ADCL ;
					tenBitValue = (ADCH<< 2) | (theLow >> 6) ;
					reagBlankTrVol = tenBitValue;
					theResult = (float)distWaterTrVol/(float)reagBlankTrVol;
					reagBlankAbsorb = log10( theResult );
					sprintf(myBuffer,"=>%d",reagBlankTrVol);
#ifdef LCD_DISPLAY					
					lcd16x2_cursor_to(2,0);
					lcd16x2_write_string(myBuffer);
#endif				
	
#ifdef UART_DISPLAY
					DisplayString(myBuffer);
#endif					
					_delay_ms(5000);
#ifdef LCD_DISPLAY					
					lcd16x2_clear();
					lcd16x2_cursor_to(1,0);
					lcd16x2_write_string("ReagBlkAbsrb");
#endif					
#ifdef UART_DISPLAY
					DisplayString("\r\nReagentBlankAbsorbance=");
#endif					
					
					memset(myBuffer,0,sizeof(myBuffer));
					FloatToStringNew(myBuffer,reagBlankAbsorb,6);
#ifdef LCD_DISPLAY					
					lcd16x2_cursor_to(2,0);
					lcd16x2_write_string(myBuffer);
#endif
#ifdef UART_DISPLAY
					DisplayString(myBuffer);
#endif					
					_delay_ms(5000);
#ifdef LCD_DISPLAY					
					lcd16x2_clear();
					lcd16x2_cursor_to(1,0);
					lcd16x2_write_string("2.SampleReading");
#endif
#ifdef UART_DISPLAY
					DisplayString("\r\n2.Sample Reading");
#endif					
					if(0x32 == getkey() )
					{
						ADC_Init();	
						ADCSRA |= 1<<ADSC;
						_delay_ms(1000);
						theLow = ADCL ;
						tenBitValue = (ADCH<< 2) | (theLow >> 6) ;
						sampleTrVol = tenBitValue;
						theResult = (float)distWaterTrVol/(float)sampleTrVol;
						sampleAbsorb = log10( theResult );
						calConc =  (sampleAbsorb - reagBlankAbsorb)*theFactor;
						sprintf(myBuffer,"=>%d",sampleTrVol);
#ifdef LCD_DISPLAY						
						lcd16x2_cursor_to(2,0);
						lcd16x2_write_string(myBuffer);
#endif
#ifdef UART_DISPLAY
						DisplayString(myBuffer);
#endif									
						_delay_ms(5000);
						
#ifdef LCD_DISPLAY						
						lcd16x2_clear();
						lcd16x2_cursor_to(1,0);
						lcd16x2_write_string("SampleAbsrb");
#endif
#ifdef UART_DISPLAY
						DisplayString("\r\nSample Absorbance=");
#endif						
						memset(myBuffer,0,sizeof(myBuffer));
						FloatToStringNew(myBuffer,sampleAbsorb,6);

#ifdef LCD_DISPLAY						
						lcd16x2_cursor_to(2,0);
						lcd16x2_write_string(myBuffer);
#endif					
#ifdef UART_DISPLAY
						DisplayString(myBuffer);
#endif	
						
						_delay_ms(5000);
#ifdef LCD_DISPLAY						
						lcd16x2_clear();
						lcd16x2_cursor_to(1,0);
						lcd16x2_write_string("Calcium Conc=");
#endif
#ifdef UART_DISPLAY
						DisplayString("\r\nCalcium Concentration=");
#endif					
	
						memset(myBuffer,0,sizeof(myBuffer));
						FloatToStringNew(myBuffer,calConc,6);
#ifdef LCD_DISPLAY						
						lcd16x2_cursor_to(2,0);
						lcd16x2_write_string(myBuffer);
						lcd16x2_cursor_to(2,7);
						lcd16x2_write_string("mg/dl");
#endif
#ifdef UART_DISPLAY
						DisplayString(myBuffer);
						DisplayString("mg/dl");
#endif						
						
						if(0 != getkey())
						break;
					}
					else
					{
						lcd16x2_clear();
						lcd16x2_cursor_to(2,0);
						lcd16x2_write_string("Wrong Choice");
						_delay_ms(300);
					}
					
				}
				else
				{
					lcd16x2_clear();
					lcd16x2_cursor_to(2,0);
					lcd16x2_write_string("Wrong Choice");
					_delay_ms(300);	
					
				}
			}
			else
			{
				lcd16x2_clear();
				lcd16x2_cursor_to(2,0);
				lcd16x2_write_string("InitProcessFirst");
				_delay_ms(300);
			}
			
			break;
			default:
			
			lcd16x2_clear();
			lcd16x2_cursor_to(2,0);
			lcd16x2_write_string("wrong Choice");
			_delay_ms(300);
			
			break;
		}
		
 #endif		
	}
}

void FloatToStringNew(char *str, float f, char size)
{
	int pos;  // position in string
	char len;  // length of decimal part of result
	char* curr = NULL;  // temp holder for next digit
	int value;  // decimal digit(s) to convert
	pos = 0;  // initialize pos, just to be sure
	
	value = (int)f;  // truncate the floating point number
	itoa(value,str,10);  // this is kinda dangerous depending on the length of str
	// now str array has the digits before the decimal
	
	if (f < 0 )  // handle negative numbers
	{
		f *= -1;
		value *= -1;
	}
	
	len = strlen(str);  // find out how big the integer part was
	pos = len;  // position the pointer to the end of the integer part
	str[pos++] = '.';  // add decimal point to string
	
	while(pos < (size + len + 1) )  // process remaining digits
	{
		f = f - (float)value;  // hack off the whole part of the number
		f *= 10;  // move next digit over
		value = (int)f;  // get next digit
		itoa(value, curr,10); // convert digit to string
		str[pos++] = *curr; // add digit to result string and increment pointer
	}
}


unsigned char getkey(void)
{
	unsigned char kbd_lookup[4][3] = {
		{'8','4','0'},
		{'9','5','1'},
		{'.','6','2'},
		{'E','7','3'}
	};
	
	volatile unsigned char row,column,temp;
	
	KBD_PORT_OUT =	0x07;   // 0x87 //0x07;	(0<<PA6|0<<PA5|0<<PA4|0<<PA3)		//PA6,PC5,PC4,PC3 are rows (are low)
	_delay_us(1);
	KBD_PORT_DIR =  0x78;	// 0x78  // (1<<PA6|1<<PA5|1<<PA4|1<<PA3);			//PC6,PC5,PC4,PC3 are rows (are outputs)
	_delay_ms(2);
	column = KBD_PORT_IN;
	
	while((column & 0x07) != 0x07)		//Wait till all keys are released
	column = KBD_PORT_IN;
	
	while(1)
	{
		column = KBD_PORT_IN;
		while((column & 0x07) == 0x07)
		column = KBD_PORT_IN;
		
		_delay_ms(20);				//deb ounce
		column &= 0x07;
		temp = KBD_PORT_IN;
		temp &= 0x07;
		if(temp == column)
		break;
	}
	column &= 0x07;
	column ^= 0x07;
	
	KBD_PORT_OUT =	0x78;   	//0x78;		(0<<PA2|0<<PA1|0<<PA0)//PC2,PC1,PC0 are column (are low)
	_delay_us(1);
	KBD_PORT_DIR =	0x07;		//0x87;	//0x07;(1<<PA2|1<<PA1|1<<PA0)		//PC2,PC1,PC0 are column (are output)
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

  
  
  
  
  
