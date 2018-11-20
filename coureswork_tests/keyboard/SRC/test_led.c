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
#include "sio.h"
#include "keyboard.h"

// �������� �� �������� ���������� ��
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
    unsigned char __xdata * TmpVector; // ��������� ����������
    // ������ ������ �� ���������� ������ ������������ // ��� ������� �������� ���������� ljmp, ������ 02h
	*Address = 0x02;
    // ����� ������������ ����� �������� Vector
	TmpVector = (unsigned char __xdata *) (Address + 1);
	*TmpVector = (unsigned char) ((unsigned short)Vector >> 8);
	++TmpVector;
	*TmpVector = (unsigned char) Vector;
    // ����� �������, �� ������ Address ������
    // ������������� ���������� ljmp Vector
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
    // ��������� ���������� ���������� (TF0) �� ������������ �������/�������� 0
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
