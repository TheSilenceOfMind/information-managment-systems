#include "aduc812.h"
#include "led.h"
#include "max.h"
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


char set_time( unsigned char program_number );
char set_temperature( unsigned char program_number, unsigned char time );
char cooking_process( unsigned char program_number, unsigned char time, unsigned char temp );
char dish_is_cooked( void );


char read_key(unsigned char* symbol)
{
	EA = 1;
	return read_keyboard(symbol);
}

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

void delay(unsigned char ms)
{
	volatile int i, j;

    for( j = 0; j < ms; j++ )
        for( i = 0; i < 25; i++ );
}

void wait_sec(unsigned char * min, unsigned char * sec)
{
    delay(1000);

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
	unsigned char msg[14] = "set program 1";
	unsigned char program_number = 0; 
	unsigned char symbol;

	print_string_lcd(msg, 13);
	for ( ;; )
	{
		if (0 == read_key(&symbol))
			continue;	

		switch (symbol)
		{
			case NEXT_SIGN:
				if (set_time(program_number)) return;
				break;
			case MORE_SIGN:
				program_number = (program_number + 1) % MAX_PROGRAM_NUMBER;
				break;
			case LESS_SIGN:
				program_number = (MAX_PROGRAM_NUMBER + program_number - 1) % MAX_PROGRAM_NUMBER;
				break;
		}

		write_number_to_str(msg, 12, program_number + 1, 1);
		print_string_lcd(msg, 13);
	}
}

char set_time( unsigned char program_number )
{
	unsigned char msg[17] = "set time  30 min";
	unsigned char time = 30; 
	unsigned char symbol;

	print_string_lcd(msg, 16);
	for ( ;; )
	{
		if (0 == read_key(&symbol))
			continue;	

		switch (symbol)
		{
			case PREV_SIGN:
				return 0;
			case NEXT_SIGN:
				if (set_temperature(program_number, time)) return 1;
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
		print_string_lcd(msg, 16);
	}
}

char set_temperature( unsigned char program_number, unsigned char time )
{
	unsigned char msg[20] = "set t =  90 degrees";
	unsigned char temp = 90;
	unsigned char symbol;

	print_string_lcd(msg, 19);
	for ( ;; )
	{
		if (0 == read_key(&symbol))
			continue;

		switch (symbol)
		{
			case PREV_SIGN:
				return 0;
			case NEXT_SIGN:
				if (cooking_process(program_number, time, temp)) return 1;
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
		print_string_lcd(msg, 19);
	}
}

char cooking_process( unsigned char program_number, unsigned char minutes, unsigned char temp )
{
	unsigned char msg[32] = "before cooking  time 100 m 60 s";
	unsigned int cur_time, base_time = minutes * 60;
	unsigned char seconds = 60;
	unsigned char symbol;

	--minutes;
	write_time_to_str(msg, 21, minutes, 27, seconds);
	print_string_lcd(msg, 31);

	for ( ;; )
	{
		// leds
		unsigned int luminous_leds;
		cur_time = base_time - (minutes * 60 + seconds);
		luminous_leds = (cur_time << 3) / base_time;
		leds(~(255 >> luminous_leds));
		// 
		wait_sec(&minutes, &seconds);
		write_time_to_str(msg, 21, minutes, 27, seconds);
		print_string_lcd(msg, 31);

		if (0 == minutes && 0 == seconds)
			if (dish_is_cooked()) return 1;

		if (0 != read_key(&symbol) && PREV_SIGN == symbol)
			return 0;
	}
}

char dish_is_cooked( void )
{
	char i;
	unsigned char msg[16] = "Dish is cooked!";
	print_string_lcd(msg, 15);

	for (i = 0; i < 3; ++i)
	{
		leds(0);
		delay(300);
		leds(255);
		delay(300);
	}
	
	return 1;
}

void main( void )
{
	init_keyboard((void*)ScanKBOnce);
	for ( ;; )
	{
		leds(0);
		set_program();
	}
}