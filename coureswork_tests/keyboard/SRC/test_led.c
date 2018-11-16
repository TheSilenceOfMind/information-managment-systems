/****************************************************************************

    test_led.c - ���� �������� ����������������� ������
                 ��� �������� ������ SDK-1.1

    (C) test_led.c, ������ �.�.  2007 �.

��� ��������� ���������; �� ������ �������� �������������� �� �/���
�������������� �� � ������������ � ������������� ������������
��������� GNU, �������������� ������ ���������� ��; ���� ������ 2,
���� (�� ������ ������) ����� ����� ������� ������.

��� ��������� ���������������� � �������, ��� ��� ����� ��������,
�� ��� �����-���� ��������; ���� ��� ��������������� ��������
������������ �������� ��� ����������� ��� ���������� ����.  ���
��������� ��������� �������� �������� ������������� ������������
�������� GNU.

�� ������ ���� �������� ����� ������������� ������������ ��������
GNU ������ � ���� ����������; ���� ���, �������� �� ������: Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA

----------------------------------------------------------------------------
������, �����-���������, ������� �������������� ������� ���������
e-mail: kluchev@d1.ifmo.ru

****************************************************************************/
#include "aduc812.h"
#include "led.h"
#include "dip.h"
#include "sio.h"
#include "kb.h"


// �������� �� �������� ���������� ��
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


// http://embedded.ifmo.ru/sdk/sdk11/soft/examples/keil/demo_sdk11.zip
//           in order to see source code
void main( void )
{
    unsigned char ch;

    InitSIO(S9600, 0);
    Type("Hello!\r\n");

    Type("\r\nReading keyboard\r\n");
    while (1)
    {
        if( ScanKBOnce(&ch) ) // Kirill: start
        {
            WSio(ch);
        }
    }
}
