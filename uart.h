

void UART_Init( unsigned int baud );  		// To initialize UART
void UART_Transmit( char data );		// To Transmit single char to Terminal
char UART_Receive( void );						// To receive text from PC keyboard
void DisplayString(char * str);					//  To Display a string or values on Terminal
