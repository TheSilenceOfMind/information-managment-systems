#include "aduc812.h"
#include "helper.h"
#include "max.h"

extern unsigned char note_delay[];
extern const unsigned short notes[];
extern const unsigned short ok_btn_notes[];
extern const unsigned short err_btn_notes[];

void set_vector(unsigned char __xdata *address, void *vector)
{
    unsigned char __xdata *tmp;

    *address = 0x02;

    tmp = (unsigned char __xdata *)(address + 1);
    *tmp = (unsigned char)((unsigned short)vector >> 8);
    ++tmp;

    *tmp = (unsigned char)vector;
}

void compute_note_delay(unsigned char mode, unsigned char index)
{
	unsigned short freq = 1;
	unsigned short delay;
	unsigned char *ptr;
	
	switch (mode) {
		case 0: 
			freq = ok_btn_notes[index];
			break;
		case 1:
			freq = err_btn_notes[index];
			break;
		case 2:
			freq = notes[index];
			break;
	};
    delay = 0xffff - 460791 / freq;
    ptr = (unsigned char *)&delay;
    note_delay[0] = ptr[0];
    note_delay[1] = ptr[1];
}