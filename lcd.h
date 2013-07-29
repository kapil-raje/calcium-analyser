


#define LCD_DATA PORTB
#define LCD_DATA_READ PINB
#define LCD_DATA_DIR DDRB

#define LCD_CTRL_DIR DDRD
#define LCD_RS  	PD6	
#define LCD_RW		PD5	
#define LCD_EN		PD4	
#define LCD_CTRL PORTD

void lcd16x2_write (unsigned char writethis,unsigned char command) ;
void lcd16x2_write_string (char * str );
unsigned char lcd16x2_get_ready (void) ;
void lcd16x2_cursor_to (unsigned char line, unsigned char location) ;
void lcd16x2_init (void) ;
void lcd16x2_delay (void) ;
void lcd16x2_clear (void);
	
