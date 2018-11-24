#include "aduc812.h"
#include "timer0.h"
#include "max.h"

void main( void ) {
	init_timer0();
	
	/*unsigned char light = 1;
	init_timer0();
	// Установка вектора в пользовательской таблице
	set_vector( 0x200B, (void *)T0_ISR );
	// Разрешение прерываний от таймера 0
	ET0 = 1; EA = 1;
	while( 1 )
	{
		leds( light );
		if(light == 0xFF) light = 1;
		else light |= light << 1;
		delay_ms( 300 );
	}*/
}
