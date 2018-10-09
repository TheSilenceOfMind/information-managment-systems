
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

void animation () {
	volatile unsigned int i, j;
	// первая часть анимации
	for (j = 0; j < 8; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			arr[i] = arr[i+1];
		}
		arr[4] *= 2;
		display_leds(500, arr);
	}

	delay(200);

	//вторая часть анимации
	arr[3] = 0;
	for (j = 0; j < 4; ++j) {
		for (int i = 0; i < 4; ++i)
		{
			arr[i] = arr[i+1];
		}
		display_leds(500, arr);
	}

	delay(200);

	// третья часть анимациии
	for (j = 0; j < 12; ++j){
		for (int i = 0; i < 4; ++i)
		{
			arr[i] = arr[i+1];
		}
		arr[4] /= 2;
		display_leds(500, arr);
	}

	delay(200);
}

void main( void ) {
	volatile unsigned int i;
	
	init_timer1();
	init_int0((void*) change_mode);
	EA = 1;		// разрешить прерывания

	leds(0);
		
	while (1) {
		if (mode) {
			animation();
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