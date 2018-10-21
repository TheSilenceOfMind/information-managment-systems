#include "aduc812.h"
#include "sio.h"

#define NEWLINE ('\n')

void main( void )
{
unsigned char c;

    init_sio( S9600 );

    type("’Ґбв ¤а ©ўҐа  SIO ¤«п бвҐ­¤  SDK-1.1\r\n");
    type("Ќ ¦Ё¬ ©вҐ Є­®ЄЁ ¤«п вҐбвЁа®ў ­Ёп... \r\n");

    while( 1 )
    {
        if( rsiostat() )
        {
            c = rsio();

            if ('a' <= c && c <= 'z')
            {
				char i;
				
				wsio(c);
				for (i = c - 1; i >= 'a'; i--)
					wsio(i - 32);
				wsio(NEWLINE);
            }
        }
    }
}