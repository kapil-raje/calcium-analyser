#include"UART.h"
#include <avr/io.h>


void UART_Init( unsigned int baud )
{
  /* Set baud rate */
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	//UCSRC = (1<<USBS)|(3<<UCSZ0);
	
	/* Set frame format: 8data, 1stop bit, No Parity */
	UCSR0C= 0x06;     //(0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02);           //
	
	//UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0) ;
	
}

void UART_Transmit( char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

char UART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void DisplayString(char * str)
{
	while(*str != '\0')
	{
		UART_Transmit(*str);
		str++;
	}
	
}
