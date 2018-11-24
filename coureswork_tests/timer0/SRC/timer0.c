#include "aduc812.h"
#include "timer0.h"
#include "led.h"

static unsigned long __systime = 0;

void set_vector(unsigned char xdata * address, void * vector){
    unsigned char xdata * tmp_vector; // Временная переменная
    // Первым байтом по указанному адресу записывается
    // код команды передачи управления ljmp, равный 02h
    *address = 0x02;
    // Далее записывается адрес перехода vector
    tmp_vector = (unsigned char xdata *) (address + 1);
    *tmp_vector = (unsigned char) ((unsigned short)vector >> 8);
    ++tmp_vector;
    *tmp_vector = (unsigned char) vector;
    // Таким образом, по адресу address теперь
    // располагается инструкция ljmp vector
}
//////////////////////// T0_ISR //////////////////////////////
// Обработчик прерывания от таймера 0.
//////////////////////////////////////////////////////////////
void T0_ISR( void ) __interrupt ( 1 ) {
    leds(0xFF);

    // Время в милисекундах
    ///__systime++;

    //char tmp;
    //tmp = ScanKBOnce();
    //leds(tmp);
    //TH0 = 0x4B;     // Инициализация таймера 0:
    //TL0 = 0xFF;     // 1/20 секунды 
    TF0 = 0;
}
///////////////////////////////////////////////////////////////////////////
// Чтение милисекундного счетчика
///////////////////////////////////////////////////////////////////////////
unsigned long get_ms_counter(void) {
    unsigned long res;
    ET0 = 0;
    res = __systime;
    ET0 = 1;
    return res;
}
///////////////////////////////////////////////////////////////////////////
// Инициализация Таймера 0 (1000Гц)
///////////////////////////////////////////////////////////////////////////
void init_timer0(void) {
    TCON = TCON & 0xF0;  // Выключение таймера 0 
    TH0 = 0x4B;         // Инициализация таймера 0:
    TL0 = 0xFF;         // 1/20 секунды 
    TMOD = 0x01;        // Выбор режима работы 16-разрядный таймер
    TCON = TCON & 0x10 ;  // Включение таймера 0

    set_vector(0x200B, (void *)T0_ISR);
    ET0 = 1;
    EA = 1;
}
///////////////////////////////////////////////////////////////////////////
// Возвращает прошедшее время (от момента замера)
///////////////////////////////////////////////////////////////////////////
unsigned long dtime_ms(unsigned long t2) {
    unsigned long t1 = (unsigned long)get_ms_counter();
    return t1 - t2;
}
///////////////////////////////////////////////////////////////////////////
// Задержка в милисекундах
///////////////////////////////////////////////////////////////////////////
void delay_ms(unsigned long ms) {
    unsigned long t1 = ( unsigned long )get_ms_counter();
    while (1) {
        if (dtime_ms(t1) > ms) break;
    }
}

