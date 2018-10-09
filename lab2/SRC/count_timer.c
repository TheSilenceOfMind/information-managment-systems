
#include "aduc812.h"
#include "count_timer.h"

unsigned char __count = 0;

unsigned char get_count () {
	return __count;
}

void T1_ISR (void) __interrupt (1) {
	__count++;
}

// инициализация таймера-счетчика (таймер 1)
void init_timer1 ( void ) {
	// задаем обработчик события прерывания от таймера 1	
	//SetVector(0x201B, (void *)T1_ISR );

	TL1 = 0xFE;
	TH1 = 0xFE ;
	
	// устанавливаем работу таймера 1 в режим "8-битный автоперезагружаемый таймер/счетчик"
	// + счетчик работает от внеш прерываний (стр. 77 методы)
	TMOD |= 0x60; 
	
	// стр. 79 метОды - включаем таймер 1
	TCON |= 0x40;
	
	// разрешаем прерывания от таймера 1
	ET1 = 1; 
}


