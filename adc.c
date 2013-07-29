#include"ADC.h"
#include <avr/io.h>




void ADC_Init(void)
{
  ADCSRA |= 1<<ADPS2 ;      // pre scalar value is selected as 16
	//ADCSRA |= (1<<ADPS1) | (1<<ADPS0);   // pre scalar value is selected as 8
	
	ADMUX  |= 1<<ADLAR ;
	ADMUX  |= 1<<REFS0 ;
	ADMUX  |= 1<<REFS1 ;
	
	//To select ADC 7
	ADMUX  |= 1<<MUX0  ;		
	ADMUX  |= 1<<MUX1  ;
	ADMUX  |= 1<<MUX2  ;
	
	//ADCSRA |= 1<<ADIE  ;
	ADCSRA |= 1<<ADEN  ;

    DIDR0 = 1<<ADC7D ; 
}


