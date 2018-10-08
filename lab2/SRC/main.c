
#include "max.h"
#include "led.h"
#include "ext_int.h"
#include "count_timer.h"

static unsigned short mode = 1;

void change_mode () {
	mode = !mode;
}

void delay ( unsigned long ms )
{
    volatile unsigned long i, j;

    for( j = 0; j < ms; j++ )
    {
        for( i = 0; i < 50; i++ );
    }
}


void main( void ) {
	unsigned char arr[8] = {1, 2, 4, 8, 16, 32, 64, 128};
	
	init_timer1();
	InitInt0();
	leds(0);
		
	while (1) {
		if (mode) {
			display_leds(5000, arr);
			leds(0);
			delay(500);
		} else {
			leds(0);
		}		
	}
	
}   