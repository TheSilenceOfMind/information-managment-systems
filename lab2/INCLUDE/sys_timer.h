#ifndef __SYS_T__H
#define __SYS_T__H

void init_systimer0 (void);
unsigned long GetMsCounter (void);
unsigned long DTimeMs (unsigned long t2);
void DelayMs (unsigned long ms);

#endif