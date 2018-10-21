#include "aduc812.h"
#include "timer0.h"

unsigned long __systime = 0;
unsigned short type = 0, count = 0;
void * handler;

static void SetVector(unsigned char __xdata * Address, void * Vector)  {
	unsigned char __xdata * TmpVector; // Временная переменная
	// Первым байтом по указанному адресу записывается
	// код команды передачи управления ljmp, равный 02h
	*Address = 0x02;
	// Далее записывается адрес перехода Vector
	TmpVector = (unsigned char __xdata *) (Address + 1);
	*TmpVector = (unsigned char) ((unsigned short)Vector >> 8);
	++TmpVector;
	*TmpVector = (unsigned char) Vector;
	// Таким образом, по адресу Address теперь
	// располагается инструкция ljmp Vector
}

//work == выпоняемая функция //typef == режим работы 
void init_sys_timer_0( void * handlerf, unsigned short typef, unsigned short countf )  {
    //option
	handler = handlerf;
	type = typef;
	count = countf;

	EA = 1;
	ET = 1;

	TR0 = 0; //Выключение таймера
	TMOD = TMOD & 0xF0; //Сброс TMOD
	if (typef == 1) {  // Выбор режима работы 16-разрядный счётчик 
		TMOD = TMOD | 0x05; 
		SetVector(TF0_VECTOR_N, (void *)T_ISR);
	}
	else {  // Выбор режима работы 16-разрядный таймер 
		TMOD = TMOD | 0x01; 
		SetVector(TF1_VECTOR_N, (void *)T_ISR);
	}

	TH0 = count >> 8; // настройка на частоту работы 
	TL0 = count; 

	TR0 = 1; // Включение таймера 0
}

unsigned long get_counter( void )  {
	unsigned long res;
	ET0 = 0;
	if(type == 1) {
		res = 0xFFFF - count;
	}
	else {
		res = __systime;
	}
	ET0 = 1;
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

void T_ISR( void ) __interrupt ( 1 )  {
	handler();
	__systime ++;
	TH0 = count >> 8; 
	TL0 = count; 
	
}