#include "aduc812.h"
#include "sio.h"
#include "driver.h"

size_t rfifo_len, wfifo_len;
unsigned char rfifo[FIFO_SIZE];
unsigned char wfifo[FIFO_SIZE];

void init_UART( void )
{
	SetVector((unsigned char __xdata *)0x2023, (void *)SIO_ISR);
	init_sio(S9600);
}

unsigned char read_UART( unsigned char * symbol )
{	
	int i;

	if ( 0 < rfifo_len )
	{
		*symbol = rfifo[0];
		--rfifo_len;
		
		for (i = 0; i < rfifo_len; i++)
			rfifo[i] = rfifo[i + 1];
			
		return 1;
	}

	return 0;
}

void write_UART( unsigned char c )
{
	wfifo[wfifo_len % FIFO_SIZE] = c;
	
	if (wfifo_len < FIFO_SIZE)
		++wfifo_len;
}

void rfifo_push( unsigned char c )
{
	rfifo[rfifo_len % FIFO_SIZE] = c;
	
	if (rfifo_len < FIFO_SIZE)
		++rfifo_len;
}

unsigned char wfifo_pop( unsigned char * symbol )
{	
	int i;

	if ( 0 < wfifo_len )
	{
		*symbol = wfifo[0];
		--wfifo_len;
		
		for (i = 0; i < wfifo_len; i++)
			wfifo[i] = wfifo[i + 1];
			
		return 1;
	}

	return 0;
}

void SIO_ISR( void ) __interrupt ( 4 )
{
	// Interrrupt blog for sending data
	if( TI )
	{	
		wfifo_pop(SBUF);
		TI = 0;
	}

	// Interrrupt blog for receiving data
	if( RI )
	{
		rfifo_push(SBUF);
		RI = 0;
	}
}

/**----------------------------------------------------------------------------
                            enable_interrupt
-------------------------------------------------------------------------------
Описание:	Разрешает прерывания
-----------------------------------------------------------------------------*/
void enable_interrupt( void )
{
	EA = 1;
	ES = 1;
}

/**----------------------------------------------------------------------------
                            disable_interrupt
-------------------------------------------------------------------------------
Описание:	Запрещает прерывания
-----------------------------------------------------------------------------*/
void disable_interrupt( void )
{
	EA = 0;
	ES = 0;
}
