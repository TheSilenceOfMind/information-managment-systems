#include "aduc812.h"
#include "sys_timer.h"

unsigned long __systime = 0;


void T0_ISR (void) __interrupt (1)
{	
	__systime++;// Время в милисекундах
	TH0 = 0xFC; // Инициализация таймера 0:
	TL0 = 0x67; // настройка на частоту работы 1000 Гц
}

void init_systimer0 (void )
{
	TCON = TCON | 0x00; 	// Выключение таймера 0 (и таймера 1)
	TMOD = TMOD | 0x01; 	// Выбор режима работы 16-разрядный таймер
	TH0 = 0xFC; 	// Инициализация таймера 0:
	TL0 = 0x67; 	// настройка на частоту работы 1000 Гц (чуть больше)
	TCON = TCON | 0x10; 	// Включение таймера 0

	SetVector(0x200B, (void *)T0_ISR );
}

unsigned long GetMsCounter (void)
{
	unsigned long res;
	ET0 = 0;	// запрет на прерывание таймера 0
	res = __systime;
	ET0 = 1;
	return res;
} 

unsigned long DTimeMs (unsigned long t2)
{
	unsigned long t1 = ( unsigned long )GetMsCounter();
	return t1 - t2;
}

void DelayMs (unsigned long ms)
{
	unsigned long t1 = (unsigned long)GetMsCounter();
	while (1)
	{
		if (DTimeMs(t1) > ms) break;
	}
}


