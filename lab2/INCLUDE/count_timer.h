#ifndef __COUNT_T__H
#define __COUNT_T__H

void init_timer1 (void);
int get_count();
void T1_ISR( void ) __interrupt ( 1 );

#endif