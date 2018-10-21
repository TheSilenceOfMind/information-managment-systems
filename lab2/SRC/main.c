#include "aduc812.h"
#include "led.h"
#include "max.h"
#include "timer1.h"
#include "interrupt.h"

static unsigned char arr [5] = {0, 0, 0, 0, 1};

void delay ( unsigned long ms )  {
    volatile unsigned long i, j;
    for( j = 0; j < ms; j++ )  {
        for( i = 0; i < 50; i++ );
    }
}

static int counter = 0;
static int mode = 0;
static int direction_counter = 0;


static void count() __interrupt (2)  {
    counter++;
    
    TH1 = 0xFFFF >> 8; // настройка частоты работы таймера 
	TL1 = 0xFFFF; 
}

void change_mode() __interrupt (1)  {
    if (mode == 0)
        mode = 1;
    else
        mode = 0;
}

void swap_array() {
	int i = 0;	
    if (direction_counter == 0) {
        arr[0] = 0;
		arr[1] = 0;
		arr[2] = 0;
		arr[3] = 0;
		arr[4] = 1;
    }
    if (direction_counter == 10) {
        arr[0] = 0;
		arr[1] = 0;
		arr[2] = 0;
		arr[3] = 0;
		arr[4] = 128;
    }

    for (i = 0; i < 4; ++i) {
        arr[i] = arr[i+1];
    }

    if (direction_counter < 10) {
        arr[4] *= 2;
    } else if (direction_counter < 20) {
        arr[4] /= 2;		
    } else {
        direction_counter = -1;
    }
	
	++direction_counter;
} 

void main( void )  {
    write_max(ENA, 0x20);    
    init_count_timer_1((void*)count, 1, 0xFFFF);
    init_interrupt(0, (void*)change_mode);
    
    EA = 1;
    
    while (1) {
        if (mode == 0) {
            swap_array();
            display_leds(300, arr);
        } else {
            leds(counter);
        }
    }
}   