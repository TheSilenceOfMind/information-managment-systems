#include "aduc812.h"
#include "led.h"
#include "max.h"

static unsigned short mode = 0;

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


void main( void )
{
	while (1) {
		DisplayLeds(5000);
		leds(0);
		delay2(500);
	}
	/*
	unsigned char light = 1;
	InitSystimer0();
	// Установка вектора в пользовательской таблице
	SetVector( 0x200B, (void *)T0_ISR );
	// Разрешение прерываний от таймера 0
	ET0 = 1; EA = 1;
	while( 1 )
	{
		leds( light );
		if( light == 0xFF ) light = 1;
		else light |= light << 1;
		DelayMs( 300 );
	}
	*/
}   