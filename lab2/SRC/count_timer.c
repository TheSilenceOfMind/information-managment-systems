#include "aduc812.h"
#include "count_timer.h"

int __count = 0;

void SetVector(unsigned char __xdata * Address, void * Vector)  {
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

int get_count () {
	return __count;
}

// инициализация таймера-счетчика (таймер 1)
void init_timer1 (void) {
	
	// задаем обработчик события прерывания от таймера 1 (стр. 
	SetVector(0x201B, (void*) T1_ISR);
	
	TL1 = 0xFE;
	TH1 = 0xFE ;
	
	// устанавливаем работу таймера 1 в режим "8-битный автоперезагружаемый таймер/счетчик"
	// + счетчик работает от внеш прерываний (стр. 77 методы)
	TMOD |= 0x60; 
	
	// стр. 79 методы - тупо включаем таймер 1
	TCON |= 0x40;
	
	// разрешаем прерывания в принципе
	ET1 = 1; EA = 1 ;
}

void T1_ISR( void ) __interrupt (1) {
	__count++;
}

