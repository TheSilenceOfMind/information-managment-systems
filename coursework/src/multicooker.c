//#include "led.h"
//#include "max.h"
#include "lcd.h"
#include "keyboard.h"

#define NEXT_SIGN '#'
#define PREV_SIGN '*'
#define MORE_SIGN 'A'
#define LESS_SIGN 'B'

#define MAX_PROGRAM_NUMBER 5
#define MIN_TIME           10
#define MAX_TIME           240
#define MIN_TEMPERATURE    60
#define MAX_TEMPERATURE    240


void set_time( unsigned char program_number );
void set_temperature( unsigned char program_number, unsigned char time );
void cooking_process( unsigned char program_number, unsigned char time, unsigned char temp );
void wish_is_cooked( void );


void write_number_to_str(unsigned char* str, unsigned char pos, unsigned char number, char line_break)
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
		*(str + pos++) = '0' + (number % 10);
	}

	if (line_break)
		*(str + pos++) = '\0';
}

void write_time_to_str(unsigned char* str,
	unsigned char pos_min, unsigned char min,
	unsigned char pos_sec, unsigned char sec)
{
	if (10 > min){
		*(str + pos_min) = ' ';
		*(str + pos_min + 1) = ' ';
		write_number_to_str(str, pos_min + 2, min, 0);
	}
	else if (100 > min){
		*(str + pos_min) = ' ';
		write_number_to_str(str, pos_min + 1, min, 0);
	}
	else
		write_number_to_str(str, pos_min, min, 0);

	if (10 > sec){
		*(str + pos_sec) = ' ';
		write_number_to_str(str, pos_sec + 1, sec, 0);
	}
	else
		write_number_to_str(str, pos_sec, sec, 0);
}

void wait_sec(unsigned char * min, unsigned char * sec)
{
    volatile unsigned long i, j;

    // Delay
    for( j = 0; j < 1000; j++ )
    {
        for( i = 0; i < 50; i++ );
    }

	// Decrease time
	if (0 < *sec)
	{
		*sec = *sec - 1;

		if (0 == *sec && 0 < *min)
		{
			*min = *min - 1;
			*sec = 60;
		}
	}
}

void set_program( void )
{
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

		write_number_to_str(msg, 12, program_number + 1, 1);
		PrintStringLCD(msg);
	}
}

void set_time( unsigned char program_number )
{
	unsigned char msg[17] = "set time  30 min\0"; 
	unsigned char time = 30; 
	unsigned char symbol;

	PrintStringLCD(msg);
	for ( ;; )
	{
		if (0 == read_keyboard(&symbol))
			continue;	

		switch (symbol)
		{
			case PREV_SIGN:
				return;
			case NEXT_SIGN:
				set_temperature(program_number, time);
				break;
			case MORE_SIGN:
				time = (time + 5 > MAX_TIME) ? MIN_TIME : time + 5;
				break;
			case LESS_SIGN:
				time = (time - 5 < MIN_TIME) ? MAX_TIME : time - 5;
				break;
		}

		if (time < 100)
		{
			msg[9] = ' ';
			write_number_to_str(msg, 10, time, 0);
		}
		else
		{
			write_number_to_str(msg, 9, time, 0);
		}
		PrintStringLCD(msg);
	}
}

void set_temperature( unsigned char program_number, unsigned char time )
{
	unsigned char msg[20] = "set t =  90°С\0";
	unsigned char temp = 90;
	unsigned char symbol;

	PrintStringLCD(msg);
	for ( ;; )
	{
		if (0 == read_keyboard(&symbol))
			continue;

		switch (symbol)
		{
			case PREV_SIGN:
				return;
			case NEXT_SIGN:
				cooking_process(program_number, time, temp);
				break;
			case MORE_SIGN:
				temp = (temp + 10 > MAX_TEMPERATURE) ? MIN_TEMPERATURE : temp + 10;
				break;
			case LESS_SIGN:
				temp = (temp - 10 < MIN_TEMPERATURE) ? MAX_TEMPERATURE : temp - 10;
				break;
		}

		if (temp < 100)
		{
			msg[8] = ' ';
			write_number_to_str(msg, 9, temp, 0);
		}
		else
		{
			write_number_to_str(msg, 8, temp, 0);
		}
		PrintStringLCD(msg);
	}
}

void cooking_process( unsigned char program_number, unsigned char minutes, unsigned char temp )
{
	unsigned char msg[32] = "before cooking time 100 m 60 s\0";
	unsigned char seconds = 60;
	unsigned char symbol;

	--minutes;
	write_time_to_str(msg, 20, minutes, 26, seconds);
	PrintStringLCD(msg);

	for ( ;; )
	{
		wait_sec(&minutes, &seconds);
		write_time_to_str(msg, 20, minutes, 26, seconds);
		PrintStringLCD(msg);

		if (0 == minutes && 0 == seconds)
			wish_is_cooked();

		if (0 != read_keyboard(&symbol) && PREV_SIGN == symbol)
			set_program();
	}
}

void wish_is_cooked( void )
{
	unsigned char msg[16] = "Wish is cooked!\0";
	PrintStringLCD(msg);

	set_program();
}

void main( void )
{
	init_keyboard();	
	set_program();
}