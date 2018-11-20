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
#include "sio.h"
#include "keyboard.h"

// Задержка на заданное количество мс
void delay ( unsigned long ms )
{
    volatile unsigned long i, j;

    for( j = 0; j < ms; j++ )
    {
        for( i = 0; i < 50; i++ );
    }
}

void SetVector(unsigned char __xdata * Address, void * Vector)
{
    unsigned char __xdata * TmpVector; // Временная переменная
    // Первым байтом по указанному адресу записывается // код команды передачи управления ljmp, равный 02h
	*Address = 0x02;
    // Далее записывается адрес перехода Vector
	TmpVector = (unsigned char __xdata *) (Address + 1);
	*TmpVector = (unsigned char) ((unsigned short)Vector >> 8);
	++TmpVector;
	*TmpVector = (unsigned char) Vector;
    // Таким образом, по адресу Address теперь
    // располагается инструкция ljmp Vector
}

// to test
void print_keyboard_buffer_to_sio(void)
{
    int i;

    for (i = 0; i < KB_BUFFER_SIZE; i++)
        WSio(keyboard_buff[i]);
}

// to test
void endless_dots_print(void)
{
    while (1)
    {
        WSio('.');
        delay(500);
    }
}

// http://embedded.ifmo.ru/sdk/sdk11/soft/examples/keil/demo_sdk11.zip
//           in order to see source code
void main( void )
{
    // назначить обработчик прерывания (TF0) по переполнению таймера/счетчика 0
    SetVector( 0x200B, (void *)TIMER_KB);
    InitSIO(S9600, 0);
    init_keyborad();

    print_keyboard_buffer_to_sio();
    endless_dots_print();

    // Type("Hello!\r\n");
    // Type("\r\nReading keyboard:\r\n");
    // while (1)
    // {
    //     unsigned char ch;
    //     if (read_keyboard(&ch) != 0)
    //         WSio(ch);
    // }
}
