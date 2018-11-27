#ifndef __TIMER0__H
#define __TIMER0__H

void init_timer0 (void);
unsigned long get_ms_counter(void);
unsigned long dtime_ms(unsigned long t2);
void delay_ms( unsigned long ms );
void set_vector(unsigned char xdata * address, void * vector);

#endif