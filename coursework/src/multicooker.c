//#include "led.h"
//#include "max.h"
#include "lcd.h"
#include "keyboard.h"

#define MODE_COUNT  5
#define NEXT_SIGN '#'
#define PREV_SIGN '*'
#define MORE_SIGN 'A'
#define LESS_SIGN 'B'

#define MAX_PROGRAM_NUMBER 5


void set_time( unsigned char program_number );
void set_temperature( void );
void cooking_process( void );
void wish_is_cooked( void );


void write_number_to_str(unsigned char* str, unsigned char pos, unsigned char number)
{
	if (number < 10)
	{
		*(str + pos++) = '0' + number;
	}
	else if (number < 100)
	{
		*(str + pos++) = '0' + (number / 10);
		*(str + pos++) = '0' + (number % 10);
	}
	else
	{
		*(str + pos++) = '0' + (number / 100);
		*(str + pos++) = '0' + ((number / 10) % 10);
		*(str + pos++) = '0' + (number % 100);
	}

	*(str + pos++) = '\0';
}

void set_program( void ){
	unsigned char msg[14] = "set program 1\0"; 
	unsigned char program_number = 0; 
	unsigned char symbol;

	PrintStringLCD(msg);
	for ( ;; )
	{
		if (0 == read_keyboard(&symbol))
			continue;	

		switch (symbol)
		{
			case NEXT_SIGN:
				set_time(program_number);
				break;
			case MORE_SIGN:
				program_number = (program_number + 1) % MAX_PROGRAM_NUMBER;
				break;
			case LESS_SIGN:
				program_number = (MAX_PROGRAM_NUMBER + program_number - 1) % MAX_PROGRAM_NUMBER;
				break;
		}

		write_number_to_str(msg, 12, program_number + 1);
		PrintStringLCD(msg);
	}
}

void set_time( unsigned char program_number ){

}

void set_temperature( void ){

}

void cooking_process( void )
{

}

void wish_is_cooked( void )
{

}

void main( void )
{
	init_keyboard();	
	set_program();
}