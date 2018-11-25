#include "aduc812.h"
#include "timer1.h"

unsigned long __systime = 0;
unsigned short type = 0, count = 0;


static void SetVector(unsigned char __xdata * Address, void * Vector)  {
	unsigned char __xdata * TmpVector; 
	*Address = 0x02;
	TmpVector = (unsigned char __xdata *) (Address + 1);
	*TmpVector = (unsigned char) ((unsigned short)Vector >> 8);
	++TmpVector;
	*TmpVector = (unsigned char) Vector;
}

void init_count_timer_1( void * handler, unsigned short typef, unsigned short countf )  {	
	type = typef;
	count = countf;
    
	ET1 = 1;

	TR1 = 0; //Выключение таймера
	TMOD = TMOD & 0x0F; // Сброс TMOD
	// настройка таймера 1 в зависимости от выбранного режима
	if (typef == 1) {
		TMOD = TMOD | 0x50; 
		SetVector(TF1_VECTOR_N, (void *)handler);
	}
	else {
		TMOD = TMOD | 0x10; 
		SetVector(TF1_VECTOR_N, (void *)handler);
	}

	TH1 = count >> 8;
	TL1 = count; 

	TR1 = 1; 
}

unsigned long get_counter( void )  {
	unsigned long res;
	ET1 = 0;
	if(type == 1) {
		res = 0xFFFF - count;
	}
	else {
		res = __systime;
	}
	ET1 = 1;
	return res;
}

unsigned long d_time_ms( unsigned long t2 )  {
	if(type == 0) {
		unsigned long t1 = ( unsigned long )get_counter();
		return t1 - t2;
	}
	return 0;
}

void delay_ms( unsigned long ms )  {
	if(type == 0) {
		unsigned long t1 = ( unsigned long )get_counter();
		while ( 1 )  {
			if ( d_time_ms( t1 ) > ms ) 
				break;
		}
	}
}