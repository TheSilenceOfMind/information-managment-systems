
#include "aduc812.h"
#include "max.h"
#include "led.h"
#include "ext_int.h"
#include "count_timer.h"

static unsigned short mode = 1;
static unsigned char arr[8] = {1 | 2 | 4, 8, 16, 32, 64, 128};

void change_mode () __interrupt (2) {
	mode = !mode;
}

void delay (unsigned long ms)
{
    volatile unsigned long i, j;

    for(j = 0; j < ms; j++)
    {
        for(i = 0; i < 50; i++);
    }
}

void modify_arr (unsigned char direction) {
	volatile unsigned int i;

	if (direction == 0) {
		arr[0] |= arr[1]; 
		for (i = 1; i < 5; ++i)
		{	
			arr[i] = (arr[i] * 2 >= 128) ? 0 : arr[i] * 2 ;		
		}
		arr[5] = 128;
	} else {
		arr[5] /= 2;
		for (i = 1; i < 5; ++i)
		{
			arr[i] = (arr[i-1] / 2 < 20) ? 0 : arr[i-1] / 2;
		}
		arr[0] <<= 1; 
	}	
}


/*void modify_arr (unsigned char direction) {
	volatile unsigned int i;

	if (direction == 0) {
	} else {

	}
}*/


void main( void ) {
	volatile unsigned int i;
	
	init_timer1();
	init_int0((void*) change_mode);
	EA = 1;		// разрешить прерывания

	leds(0);
		
	while (1) {
		if (mode) {
			for (i = 0; i < 5; ++i)
			{
				display_leds(5000, arr);
				modify_arr(0);
			}
			for (i = 0; i < 5; ++i)
			{
				display_leds(5000, arr);
				modify_arr(1);
			}
		} else {
			leds(get_count());
		}		
	}	
}   

//////////////////////// SetVector //////////////////////////
// Функция, устанавливающая вектор прерывания в пользовательской таблице
// прерываний.
// Вход: Vector - адрес обработчика прерывания,
// Address - вектор пользовательской таблицы прерываний.
// Выход: нет.
// Результат: нет.
//////////////////////////////////////////////////////////////

void SetVector(unsigned char __xdata * Address, void * Vector)
{
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