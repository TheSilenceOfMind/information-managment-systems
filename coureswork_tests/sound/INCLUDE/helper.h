#ifndef __HELPER__H
#define __HELPER__H

#include "aduc812.h"
#include <stdint.h>
//�������� ॣ���஢
#define KB 0x0
#define DATA_IND 0x1
#define EXT_LO 0x2
#define EXT_HI 0x3
#define ENA 0x4
#define C_IND 0x6
#define SV 0x7

void set_vector(unsigned char __xdata *address, void *vector);
void write_max(unsigned char __xdata *regnum, unsigned char value);
void compute_note_delay(unsigned char index);
void set_led(unsigned char value);
void set_ena(unsigned char value);

#endif //__HELPER__H
