#ifndef _KB_H
#define _KB_H

#define KB_BUFFER_SIZE 16

void TIMER_KB(void) __interrupt( 2 );
char ScanKBOnce(void);
void init_keyborad(void);
char read_keyboard(unsigned char* symbol);

// just to test
extern unsigned char keyboard_buff[KB_BUFFER_SIZE]; // Буффер, в который записываются
extern unsigned char keyboard_len;
// end just to test

#endif //_KB_H
