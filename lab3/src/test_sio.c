#include "led.h"
#include "driver.h"

#define NEWLINE_R 	0x0D
#define NEWLINE_W	0x0A
#define DEFAULT_VAL	0xFF

/*
    symbols -   при чтении равна кол-ву считанных символов,
                при записи - нужна для записи десятков и перевода строки в SBUF

    results -   здесь накапливается считанное число в 10-ой СС
*/
unsigned char result, symbols;

/**----------------------------------------------------------------------------
                        reset()
-------------------------------------------------------------------------------
Вход:		нет
Выход:		нет
Описание:	Сбрасывает результат конвертации
----------------------------------------------------------------------------- */
void reset() {
	result = 0;
	symbols = 0;
}

/**----------------------------------------------------------------------------
                        mistake()
-------------------------------------------------------------------------------
Вход:		нет
Выход:		нет
Описание:	Вывод в терминал сообщения об ошибке а также
			сигнализирование об этом путём зажигания всех диодов.
----------------------------------------------------------------------------- */
void mistake( void )
{
	disable_interrupt();
	type ( "Mistake has happend\n" );
	leds( DEFAULT_VAL );
	reset();
	enable_interrupt();
}

void write_number_to_UART(unsigned char number)
{
	// Send hundreds
	if ( 100 <= number )
	{
		write_UART((number / 100) + '0');
		number = number % 100;
	}
	// Send tens
	if ( 10 <= number )
	{
		write_UART((number / 10) + '0');
		number = number % 10;
	}
	// Send ones and line break
	write_UART(number + '0');
	write_UART(NEWLINE_W);
}

/**----------------------------------------------------------------------------
                        parse_received_symbol
-------------------------------------------------------------------------------
Вход:		считанный символ
Выход:		нет
Описание:	Обновление result, проверка кол-ва введенных символов
----------------------------------------------------------------------------- */
void parse_received_symbol(unsigned char number)
{
	unsigned char digit = (number < 'A') ? (number - '0') : (number - 'A' + 10);
	symbols++;

	if ( symbols <= 2 ) // If input no more than 2 digits
		result = (result << 4) + digit;
	else
		mistake();
}

void main( void )
{
	init_UART();
	reset();

	while( 1 )
	{
		// If position of DIP-switches equals 0xFF, then work without interrupts
		if (DEFAULT_VAL == readdip() )
		{
			// Запрещаем прерывания
			disable_interrupt();

			if( rsiostat() )
			{
				c = rsio();

				if ('a' <= c && c <= 'z')
				{
					char i;

					wsio(c);
					for (i = c - 1; i >= 'a'; i--)
						wsio(i - 32);
					wsio(NEWLINE_W);
				}
			}
		}
		else
		{
			unsigned char symbol;
			enable_interrupt();

			if ( 0 < read_UART(&symbol) ){ // reading from buffer and analyzing
				if ( '0' <= symbol && symbol <= '9' || 'A' <= symbol && symbol <= 'F')
					parse_received_symbol(symbol);
				else if ( symbol == NEWLINE_R )
					// Line break after input digits
					if ( symbols )
						write_number_to_UART(result);
					else
						mistake();
				else // Receive uncorrect character
					mistake();
			}
		}
	}
}
