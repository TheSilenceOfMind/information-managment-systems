#include "aduc812.h"
#include "max.h"
#include "ext_int.h"
#include "led.h"

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

void INT0_ISR( void ) __interrupt (2) {
	// пока для проверки чтоб просто понять, что прерывание сработало
	leds(0x01);
}

void InitInt0 () {
	EA = 1;		// разрешить прерывание
	EX0 = 1;	// разрешить внешнее прерывание int0
    IT0 = 1;	// сделать его по фронту, а не по уровню
    SetVector(0x2003, (void*) INT0_ISR);
}

