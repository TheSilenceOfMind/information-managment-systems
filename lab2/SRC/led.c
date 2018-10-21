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
// �������, ���������� ������� ����������� � �������� �������� 
// ������� ����������
// ����: ms - ������������ ���������,
// leds_brights - ������, �������� ����� ��� ������ �������� ��
// �������.
// �����: ���.
// ���������: ���.
//////////////////////////////////////////////////////////////

/*  ������������ ������ ����� � ������� ������ � ��� ����� � ����� ������� ������
1 - 1  (00000001)
2 - 2  (00000010)
3 - 4  (00000100)
4 - 8
5 - 16
6 - 32
7 - 64
8 - 128

��� ������ ���������� ����� � �������, ��� ������ ������� ���� �����, ������� �������� ������ - ��������� �������
(������ � �������) - (������� �������)
0 - 20 
1 - 40
2 - 60
3 - 80
4 - 100
*/

void display_leds (unsigned long ms, unsigned char leds_brights[5])  {
    volatile unsigned long j;   
    volatile unsigned long rem;
    int t = 20; // ������ ������ �������, ��������� ��� ��������� ���������� �������
    
    /* ��������� ������ �� 5 �����������, ����� �������� ������ ������ ������� � ������ ��������� 
       ������� �� ��������� �����, �� ���� �������� ���������� ��������, ���������� �� ����� */
    int b1, b2, b3, b4;
    b1 = t/5;
    b2 = 2*b1;
    b3 = 3*b1;
    b4 = 4*b1;

    leds (0);   
    for (j = 0; j < ms; j++) {
        rem = j % t;
        if (rem < b1) {  
            // ����� ������ ���������� ������� => ������� ������� ������� ������� �� ��� ���������� �����
            leds (leds_brights[1] | leds_brights[2] | leds_brights[3] | leds_brights[4]);
        } else if (rem < b2) {
            // ������� 0 �� ����� ����� ����, ��������� ������ ���������� �������� ������� ������� �������
            leds (leds_brights[2] | leds_brights[3] | leds_brights[4]);
        } else if (rem < b3) {
            leds (leds_brights[4] | leds_brights[4]);
        } else if (rem < b4) {
            leds (leds_brights[4]);
        } else {
            //  ����� �� ������������� �������� => �������� ������� ������� ������� ���� ������
            leds (0);
        }
    }
}




