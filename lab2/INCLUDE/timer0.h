#ifndef __TIMER0__H
#define __TIMER0__H

void init_sys_timer_0( void * workf, unsigned short typef, unsigned short countf);
unsigned long get_counter( void );
unsigned long d_time_ms(unsigned long t2);
void delay_ms( unsigned long ms );
void T_ISR( void ) __interrupt (1); 

#endif