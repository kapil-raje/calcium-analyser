



#define F_CPU 8000000UL

// Baud Rate to Connect with HyperTerminal 4800 , Parity none , Flow control none.

#define MYUBRR  12                             //FOSC/16/BAUD-1            // 51


#define KBD_PORT_DIR  DDRA
#define KBD_PORT_IN		PINA
#define KBD_PORT_OUT	PORTA


#define UART_DISPLAY
#define LCD_DISPLAY


