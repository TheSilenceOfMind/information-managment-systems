#include "aduc812.h"
#include "helper.h"

extern unsigned char note_delay[];
extern const uint16_t notes[];

void set_vector(unsigned char __xdata *address, void *vector)
{
    unsigned char __xdata *tmp;

    *address = 0x02;

    tmp = (unsigned char __xdata *)(address + 1);
    *tmp = (unsigned char)((unsigned short)vector >> 8);
    ++tmp;

    *tmp = (unsigned char)vector;
}

void write_max(unsigned char __xdata *regnum, unsigned char value)
{
    unsigned char old_dpp = DPP;

    DPP = 8;
    *regnum = value;
    DPP = old_dpp;
}

void set_led(unsigned char value)
{
    write_max(7, value);
}

void set_ena(unsigned char value)
{
    write_max(4, value);
}
void compute_note_delay(unsigned char index)
{
    uint16_t delay = 0xffff - 460791 / notes[index];
    unsigned char *ptr = (unsigned char *)&delay;
    note_delay[0] = ptr[0];
    note_delay[1] = ptr[1];
}