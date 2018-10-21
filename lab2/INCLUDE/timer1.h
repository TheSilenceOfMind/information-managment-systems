#ifndef __TIMER1__H
#define __TIMER1__H

void init_count_timer_1( void * workf, unsigned short typef, unsigned short countf);
unsigned long get_counter( void );
unsigned long d_time_ms(unsigned long t2);
void delay_ms( unsigned long ms );

#endif