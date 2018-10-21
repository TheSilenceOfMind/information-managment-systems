#include "aduc812.h"
#include "max.h"

static unsigned char old_led = 0;

void led( unsigned char n, unsigned char on ) {
    unsigned char c;
    unsigned char mask = 1;
    if( n > 7 ) return;
    c = old_led;
    mask <<= n;
    if( on )
        c |= mask;
    else
        c &= ~mask;         
    write_max( SV, c );     
    old_led = c;
}

void leds( unsigned char on ) {
    write_max( SV, on );     
    old_led = on;
}


//////////////////////// display_leds //////////////////////////
// Функция, зажигающая линейку светодиодов с заданной яркостью 
// каждого светодиода
// Вход: ms - длительность зажигания,
// leds_brights - массив, задающий маски для диодов согласно их
// яркости.
// Выход: нет.
// Результат: нет.
//////////////////////////////////////////////////////////////

/*  соответствие номера диода в линейке диодов и его маске в общей линейке диодов

Чем больше порядковый номер в массиве, тем больше яркость того диода, который задается маской - элементом массива
(индекс в массиве) - (процент яркости)
0 - 20 
1 - 40
2 - 60
3 - 80
4 - 100 */

void display_leds (unsigned long ms, unsigned char leds_brights[5])  {
    volatile unsigned long j;   
    volatile unsigned long rem;
    int t = 20; // период одного сигнала, необходим для изменения скважности сигнала
    
    /* разбиваем период на 5 промежутков, чтобы подавать разные уровни сигнала в разные интервалы 
       периода на различные диоды, то бишь изменяем скважности сигналов, подаваемых на диоды */
    int b1, b2, b3, b4;
    b1 = t/5;
    b2 = 2*b1;
    b3 = 3*b1;
    b4 = 4*b1;

    leds (0);   
    for (j = 0; j < ms; j++) {
        rem = j % t;
        if (rem < b1) {  
            // самый первый промежуток периода => подадим высокий уровень сигнала на все светящиеся диоды
            leds (leds_brights[0] | leds_brights[1] | leds_brights[2] | leds_brights[3] | leds_brights[4]);
        } else if (rem < b2) {
             // подадим 0 на менее яркий диод, остальные продолжают получать высокий уровень сигнала
            leds (leds_brights[1] | leds_brights[2] | leds_brights[3] | leds_brights[4]);
        } else if (rem < b3) {
            leds (leds_brights[2] | leds_brights[3] | leds_brights[4]);
        } else if (rem < b4) {
            leds (leds_brights[3] | leds_brights[4]);
        } else {
            //  диоды со стопроцентной яркостью => получают высокий уровень сигнала весь период
            leds (leds_brights[4]);
        }
    }
}




