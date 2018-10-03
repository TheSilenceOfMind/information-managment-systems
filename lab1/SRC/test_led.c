/****************************************************************************

    test_led.c - тест драйвера последовательного канала
                 для учебного стенда SDK-1.1

    (C) test_led.c, Ключев А.О.  2007 г.

Это свободная программа; вы можете повторно распространять ее и/или
модифицировать ее в соответствии с Универсальной Общественной
Лицензией GNU, опубликованной Фондом Свободного ПО; либо версии 2,
либо (по вашему выбору) любой более поздней версии.

Эта программа распространяется в надежде, что она будет полезной,
но БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ; даже без подразумеваемых гарантий
КОММЕРЧЕСКОЙ ЦЕННОСТИ или ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ.  Для
получения подробных сведений смотрите Универсальную Общественную
Лицензию GNU.

Вы должны были получить копию Универсальной Общественной Лицензии
GNU вместе с этой программой; если нет, напишите по адресу: Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA

----------------------------------------------------------------------------
Россия, Санкт-Петербург, кафедра вычислительной техники СПбГУИТМО
e-mail: kluchev@d1.ifmo.ru

****************************************************************************/
#include "aduc812.h"
#include "led.h"
#include "dip.h"


// Задержка на заданное количество мс
void delay ( unsigned long ms )
{
    volatile unsigned long i, j;

    for( j = 0; j < ms; j++ )
    {
        for( i = 0; i < 50; i++ );
    }
}



// update state when lightning on lamps by one
void upd_state_on (unsigned char *animation_state)
{
    unsigned char state = *animation_state;
    if (state == 0x00)
        state = 0x81; // 1000 0001
    else
        state |= ((state & 0xF0) >> 1) | ((state & 0x0F) << 1);
    *animation_state = state;
}



// update state when lightning off lamps by one
void upd_state_off (unsigned char *animation_state)
{
    unsigned char state = *animation_state;
    state = ((state & 0xF0) << 1) | ((state & 0x0F) >> 1);
    *animation_state = state;
}



void main( void )
{
    unsigned char direction = 0;    // 0 if going to led all, 1 if turn off all
    unsigned char animation_state = 0x00;

    while (1)
    {
        unsigned char dip_value = ~get_dip_values();
        if (dip_value == 0x77)
        {
            if (direction == 0)
            {
                upd_state_on(&animation_state);
                leds(animation_state);
                if (animation_state == 0xFF)
                    direction = 1;
            }
            else if (direction == 1)
            {
                upd_state_off(&animation_state);
                leds(animation_state);
                if (animation_state == 0x00)
                    direction = 0;
            }
        }
        else // if wrong dip value -> display value from DIP.
            leds(dip_value);

        delay(300); // half of a second
    }
}
