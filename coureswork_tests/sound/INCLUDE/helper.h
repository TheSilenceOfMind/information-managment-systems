#ifndef __HELPER__H
#define __HELPER__H

#include "aduc812.h"
#include <stdint.h>

void set_vector(unsigned char __xdata *address, void *vector);
void write_max(unsigned char __xdata *regnum, unsigned char value);
void compute_note_delay(unsigned char mode, unsigned char index);

#endif //__HELPER__H
