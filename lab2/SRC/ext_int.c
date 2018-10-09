#include "aduc812.h"
#include "max.h"
#include "ext_int.h"
#include "led.h"

void init_int0 (void* handler) {
	EX0 = 1;	// разрешить внешнее прерывание int0
    IT0 = 1;	// сделать его по фронту, а не по уровню
    SetVector(0x2003, (void*) handler);
}

