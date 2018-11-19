#ifndef _SIO_H
#define _SIO_H

#define BUFFERLEN 16
//Константы скорости
#define S9600   0xFD
#define S4800   0xFA
#define S2400   0xF4
#define S1200   0xE8

void Type(char * Str);
void InitSIO(char speed, char sdouble);
void WSio(unsigned char c);
unsigned char RSio(void);
char RSioStat(void);

#endif //_SIO_H
