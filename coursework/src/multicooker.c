#include "aduc812.h"
#include "led.h"
#include "max.h"
#include "lcd.h"
#include "keyboard.h"
#include "speaker.h"

#define NEXT_SIGN '#'
#define PREV_SIGN '*'
#define MORE_SIGN 'A'
#define LESS_SIGN 'B'

#define MAX_PROGRAM_NUMBER 5
#define MIN_TIME           5
#define MAX_TIME           240
#define MIN_TEMPERATURE    60
#define MAX_TEMPERATURE    240

unsigned char program_number;
unsigned char time;
unsigned char temp;

__xdata    unsigned char symbol;
__xdata unsigned char set_program_msg[14] = "set program ";
__xdata unsigned char set_time_msg[17] = "set time  30 min";
__xdata unsigned char cooking_process_msg[32] = "before cooking  time 100 m 60 s";
__xdata unsigned char set_temperature_msg[32] = "set t =  90     degrees Celsius";
__xdata unsigned char dish_is_cooked_msg[16] = "Dish is cooked!";

char set_time( void );
char set_temperature( void );
char cooking_process( void );
char dish_is_cooked( void );

char read_key(unsigned char* symbol)
{
    EX0 = 1;
    IP |= 0x20;

    return read_keyboard(symbol);
}

void print_int_to_lcd(unsigned char number, unsigned char x, unsigned char y)
{
    if (10 > number){
        print_char_at_xy(' ', x++, y);
        print_char_at_xy(' ', x++, y);
        print_char_at_xy('0' + number, x, y);
    }
    else if (100 > number){
        print_char_at_xy(' ', x++, y);
        print_char_at_xy('0' + number / 10, x++, y);
        print_char_at_xy('0' + number % 10, x, y);
    }
    else
    {
        print_char_at_xy('0' + number / 100, x++, y);
        print_char_at_xy('0' + (number / 10) % 10, x++, y);
        print_char_at_xy('0' + number % 10, x, y);
    }
}

void delay(unsigned char ms)
{
    volatile int i, j;

    for( j = 0; j < ms; j++ )
        for( i = 0; i < 152; i++ );
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
    program_number = 0;
    print_string_lcd(set_program_msg, 13);
    print_int_to_lcd(program_number + 1, 12, 0);
    for ( ;; )
    {
        if (0 == read_key(&symbol))
            continue;

        switch (symbol)
        {
            case NEXT_SIGN:
                make_sound(0);
                if (set_time()) return;
                program_number = 0;
                print_string_lcd(set_program_msg, 13);
                break;
            case MORE_SIGN:
                make_sound(0);
                program_number = (program_number + 1) % MAX_PROGRAM_NUMBER;
                break;
            case LESS_SIGN:
                make_sound(0);
                program_number = (MAX_PROGRAM_NUMBER + program_number - 1) % MAX_PROGRAM_NUMBER;
                break;
            default:
                continue;
        }

        print_int_to_lcd(program_number + 1, 12, 0);
    }
}

char set_time( void )
{
    time = 30;
    print_string_lcd(set_time_msg, 16);
    print_int_to_lcd(time, 9, 0);
    for ( ;; )
    {
        if (0 == read_key(&symbol))
            continue;

        switch (symbol)
        {
            case PREV_SIGN:
                make_sound(0);
                return 0;
            case NEXT_SIGN:
                make_sound(0);
                if (set_temperature()) return 1;
                time = 30;
                print_string_lcd(set_time_msg, 16);
                break;
            case MORE_SIGN:
                make_sound(0);
                time = (time + 5 > MAX_TIME) ? MIN_TIME : time + 5;
                break;
            case LESS_SIGN:
                make_sound(0);
                time = (time - 5 < MIN_TIME) ? MAX_TIME : time - 5;
                break;
            default:
                continue;
        }

        print_int_to_lcd(time, 9, 0);
    }
}

char set_temperature( void )
{
    temp = 90;
    print_string_lcd(set_temperature_msg, 31);
    print_int_to_lcd(temp, 8, 0);
    for ( ;; )
    {
        if (0 == read_key(&symbol))
            continue;

        switch (symbol)
        {
            case PREV_SIGN:
                make_sound(0);
                return 0;
            case NEXT_SIGN:
                make_sound(0);
                if (cooking_process()) return 1;
                temp = 90;
                print_string_lcd(set_temperature_msg, 31);
                break;
            case MORE_SIGN:
                make_sound(0);
                temp = (temp + 10 > MAX_TEMPERATURE) ? MIN_TEMPERATURE : temp + 10;
                break;
            case LESS_SIGN:
                make_sound(0);
                temp = (temp - 10 < MIN_TEMPERATURE) ? MAX_TEMPERATURE : temp - 10;
                break;
            default:
                continue;
        }

        print_int_to_lcd(temp, 8, 0);
    }
}

char cooking_process( void )
{
    unsigned int cur_time, base_time = time * 60;
    unsigned char seconds = 60;

    --time;
    print_string_lcd(cooking_process_msg, 31);
    print_int_to_lcd(time, 5, 1);
    print_int_to_lcd(seconds, 10, 1);

    for ( ;; )
    {
        // leds
        unsigned int luminous_leds;
        cur_time = base_time - (time * 60 + seconds);
        luminous_leds = (cur_time << 3) / base_time;
        leds(~(255 >> luminous_leds));
        // decrease time and print to LCD
        wait_sec(&time, &seconds);
        print_int_to_lcd(time, 5, 1);
        print_int_to_lcd(seconds, 10, 1);

        if (0 == time && 0 == seconds)
            if (dish_is_cooked()) return 1;

        if (0 != read_key(&symbol) && PREV_SIGN == symbol){
            make_sound(0);
            return 0;
        }
    }
}

char dish_is_cooked( void )
{
    print_string_lcd(dish_is_cooked_msg, 15);
    make_sound(2);

    for ( ;; )
    {
        leds(0);
        delay(300);
        leds(255);
        delay(300);

        if (read_key(&symbol) > 0)
            if (PREV_SIGN == symbol || NEXT_SIGN == symbol ||
                LESS_SIGN == symbol || MORE_SIGN == symbol)
            {
                make_sound(0);
                break;
            }
    }

    return 1;
}

void main( void )
{
    init_keyboard((void*)ScanKBOnce);
    init_sound();

    for ( ;; )
    {
        leds(0);
        set_program();
    }
}