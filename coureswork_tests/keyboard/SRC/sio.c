/******************************************************************************
                (�) ��� "���", �����-������, �����, 2002.
                http://lmt.cs.ifmo.ru, tel.: +7-812-233-3096,
                mailto: lmt@d1.ifmo.ru

        ����� 䠩� ����  ᢮�����  �����࠭塞�  �ਬ�஬  ��
    �몥  ��  (���������  Keil  C  ���  KEIL  ELEKTRONIK  GmbH)  �
    �।�����祭 ��� �������樨 ࠡ��� � ��᫥����⥫�� ����䥩ᮬ
    RS232 �⥭�� SDK-1.1.
        �����  �ணࠬ���   �����   ��    �����࣠���   ��⥫쭮��
    ���஢����  �  �����   ᮤ�ঠ��   �訡��.   �����   ��   �����
    �⢥��⢥����� �� �����  ���ଠ樨  �  ���  ��������  �।��
    ��᫥��⢨� �ᯮ�짮����� ������� �ணࠬ�����  �����  �  ��⠢�
    �ணࠬ���� �஥�⮢.
        ����� 䠩� ����� �����࣠���� ��� ���������, ��  ��  �����
    �����࠭����� � �������᪨� 楫��  �  �  ��⠢�  �������᪨�
    �ணࠬ���� �த�⮢.


����:       sio.c
�����:     1.0.0
����:      LAN
���ᠭ��:   ����� ��������樮����� �ணࠬ����� ���ᯥ祭�� ��� �⥭��
            SDK-1.1. �ਬ�� ࠡ��� � ��᫥����⥫�� ������� RS232.
            ����� "�� �����".

���������:
-------------------------------------------------------------------------------
N ���     �����   ����               ���ᠭ��
-------------------------------------------------------------------------------
1 10.04.02  1.0.0   LAN     ������
******************************************************************************/
#include "aduc812.h"
#include "sio.h"

/*----------------------------------------------------------------------------
                    �㭪樨
 -----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------
                        RSioStat()
-------------------------------------------------------------------------------
�����頥� ���㫥��� ���祭��, �᫨ ���� �ਥ�� �� ����

�室:       ���
��室:      ���
�������:  0 - ���� �ਥ�� ����;
            1 - �� �ਭ�� ᨬ���
----------------------------------------------------------------------------- */
bit RSioStat(void)
{
    return RI;
}


/**----------------------------------------------------------------------------
                        WSio()
-------------------------------------------------------------------------------
��ࠢ��� ᨬ��� �� ��᫥����⥫쭮�� ������

�室:       uchar Sym - ᨬ���, ����� �㦭� ��ࠢ���
��室:      ���
�������:  ���
----------------------------------------------------------------------------- */
void WSio(unsigned char Sym)
{
    SBUF=Sym;
    TI=0;
    while(!TI);
}

/**----------------------------------------------------------------------------
                        RSio()
-------------------------------------------------------------------------------
���������� �ਥ�� ᨬ���� �� ��᫥����⥫쭮�� ������ � �����頥� ���

�室:       ���
��室:      ���
�������:  �ਭ��� ᨬ���
----------------------------------------------------------------------------- */
unsigned char RSio(void)
{
    while(!RI);
    RI=0;
    return SBUF;
}


/**----------------------------------------------------------------------------
                        RSio()
-------------------------------------------------------------------------------
�뢮��� ASCIIZ-��ப� � ��᫥����⥫�� �����

�室:       char *Str - 㪠��⥫� �� ��ப�
��室:      ���
�������:  ���
----------------------------------------------------------------------------- */
void Type(char * Str)
{
    while(*Str)
        WSio(*Str++);
}


/**----------------------------------------------------------------------------
                        InitSIO()
-------------------------------------------------------------------------------
���樠������� ��᫥����⥫�� ����� �� �������� ᪮���. �ᯮ���� ⠩��� 1

�室:       char speed - ᪮����. �������� ����⠭⠬�, ���ᠭ�묨 �
                ��������筮� 䠩�� sio.h
            bit sdouble - �㡫�஢���� ᪮���: 0 - �� �㡫�஢��� ᪮����,
                �������� ��㬥�⮬ speed; 1 - �㡫�஢���.
��室:      ���
�������:  ���
----------------------------------------------------------------------------- */
void InitSIO(char speed, bit sdouble)
{
    TH1      = speed;
    TMOD    |= 0x20; //������ 1 �㤥� ࠡ���� � ०��� autoreload

    if(sdouble)
        PCON|=0x80;//�᫨ sdouble==1, � ᪮���� 㤢��������
    else
        PCON&=~0x80;

    TCON    |= 0x40; //����� ⠩��� 1
    SCON     = 0x50; //����ன�� ��᫥����⥫쭮�� ������
    ES       = 0;    //����饭�� ���뢠��� �� �ਥ����।��稪�
}