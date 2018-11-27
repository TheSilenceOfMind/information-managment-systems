#include "helper.h"
#include "timer.h"
#include "speaker.h"
#include "aduc812.h"

uint8_t current_ena;
extern uint8_t note_delay[2];
extern const uint16_t notes[];
int counter = 0;
unsigned long last_note_swap;
uint8_t current_note;

const uint16_t notes[] =
    {
        261,
        293,
        329,
        349,
        391,
        440,
        493};

uint8_t note_delay[2];

void change_frequency(){    
	compute_note_delay(current_note++);
	
	current_note = current_note % 7;

	RCAP2H = note_delay[1];
	RCAP2L = note_delay[0];    
}

void note_handler() __interrupt(5)
{
	
    TF2 = 0;                    // Like the Serial Interrupt, Timer 2 interrupt does not clear the interrupt flag that triggered the interrupt.
    EXF2 = 0;                   // Thus, be sure to always clear TF2 and EXF2 in your Timer 2 Interrupt.
    current_ena = ~current_ena; // Временная отрубка
    set_ena(current_ena);
	
	++counter;
	if (counter == 1000) {
		counter = 0;
		change_frequency();
	}
}

void InitSpeaker()
{
    current_ena = 0x18;
    compute_note_delay(0);
    set_vector(0x202B, (void *)note_handler);
    T2CON = 0x00; // Выключение таймера 2
    TH2 = note_delay[1];
    TL2 = note_delay[0];
    RCAP2H = note_delay[1];
    RCAP2L = note_delay[0];
    T2CON = 0x04; // Включение таймера 2

    ET2 = 1; // разрешаем прерывания от таймера 2
}