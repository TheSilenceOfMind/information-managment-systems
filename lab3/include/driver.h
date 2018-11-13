#ifndef __DRIVER__H
#define __DRIVER__H

#define FIFO_SIZE 10

void init_UART( void );

unsigned char read_UART( unsigned char * symbol );
void write_UART( unsigned char c );

void enable_interrupt( void );
void disable_interrupt( void );

#endif //__DRIVER_H