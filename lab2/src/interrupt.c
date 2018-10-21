#include "aduc812.h"
#include "max.h"
#include "interrupt.h"

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


void init_interrupt(int int_num, void *handler) {
    if (int_num == 0) {
        EX0 = 1;
        IT0 = 1;
        SetVector(IE0_VECTOR_N, (void *)handler);
    }
    else {
        EX1 = 1;
        IT1 = 1;
        SetVector(IE1_VECTOR_N, (void *)handler);
    }
}
