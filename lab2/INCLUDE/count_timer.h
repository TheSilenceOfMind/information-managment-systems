#ifndef __COUNT_T__H
#define __COUNT_T__H

unsigned char get_count();
void T1_ISR( void ) __interrupt ( 1 );
void init_timer1 (void);

#endif