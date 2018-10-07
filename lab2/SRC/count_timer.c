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
	return count;
}

void init_timer1 (void) {
	
}

void T1_ISR( void ) __interrupt (1) {
	count++;
}

