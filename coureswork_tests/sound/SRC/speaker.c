#include "helper.h"
#include "speaker.h"
#include "aduc812.h"
#include "max.h"
#include "led.h" // to test

extern uint8_t note_delay[2];
extern const uint16_t notes[];

unsigned char current_ena;
unsigned char cur_note_ind = 0;
unsigned char volume = 0x1C; // максимальная громкость воспроизведения сигналов - все 4 бита в 1. 
uint32_t cur_note_time; // время воспроизведения текущей ноты, используется как счетчик: от определенного значения до 0, после чего тональность ноты изменяется, и этот параметр снова инициализируется.
unsigned char c = 0; // to test
char end_of_melody = 0; // 1 - дорожка музыки закончилась, 0 - иначе.

const uint16_t notes[] = {
        392, 392, 392, 311, 466, 392, 311, 466, 392,
		587, 587, 587, 622, 466, 369, 311, 466, 392,
		784, 392, 392, 784, 739, 698, 659, 622, 659,
		415, 554, 523, 493, 466, 440, 466,
		311, 369, 311, 466, 392
};

uint8_t note_delay[2];

// нормировочная функция для калибровки времени воспроизведения нот с разной частотой к какой-то нормали
unsigned int get_scaled_delay(uint16_t note) {
	float koef = note/(float)482; // 482 - среднее значение частоты, посчитанное ручками, для конкретной мелодии.
	return (unsigned int)(500 * koef); // 300 - среднее время воспроизведения одной ноты (в единицах цикла). Находится подгонианом))0)
}

void set_next_note(){	
	compute_note_delay(cur_note_ind);
	cur_note_time = get_scaled_delay(notes[cur_note_ind]);
	
	cur_note_ind++;
	cur_note_ind = cur_note_ind % 4; // 39 - кол-во элементов notes[]
	if (cur_note_ind == 0)
		end_of_melody = 1;
	
	// to test
	++c;
	leds(c);
	
	RCAP2H = note_delay[1];
	RCAP2L = note_delay[0];
}

void note_handler() __interrupt(5)
{
    TF2 = 0;                    // Like the Serial Interrupt, Timer 2 interrupt does not clear the interrupt flag that triggered the interrupt.
    EXF2 = 0;                   // Thus, be sure to always clear TF2 and EXF2 in your Timer 2 Interrupt.
	
    current_ena ^= volume; // Временная отрубка: инверсия битов, отвечающих за громкость. Не спрашивайте, почему, еобходимо отрубать-врубать, чтобы был звук.
	write_max(ENA, current_ena);
	
	if (end_of_melody == 1)
		T2CON = 0x00; // Выключение таймера 2
	if (--cur_note_time == 0) 
		set_next_note();
}

void set_volume(unsigned char v) {
	current_ena = read_max(ENA);
    current_ena |= v; // установить громкость
	write_max(ENA, current_ena);
}

void InitSpeaker()
{	
	T2CON = 0x00; // Выключение таймера 2
	
	end_of_melody = 0;
	set_volume(volume);
    set_next_note();
    set_vector(0x202B, (void *)note_handler);
	
    T2CON = 0x04; // Включение таймера 2
    ET2 = 1; // разрешаем прерывания от таймера 2
}