#include "helper.h"
#include "speaker.h"
#include "aduc812.h"
#include "max.h"

extern unsigned char note_delay[2];
extern const unsigned short notes[];
extern const unsigned short ok_btn_notes[];
extern const unsigned short err_btn_notes[];

unsigned char cur_mode;

unsigned char current_ena;
unsigned char cur_note_ind;
unsigned char volume = 0x1C; // максимальная громкость воспроизведения сигналов - все 4 бита в 1. 
unsigned long cur_note_time; // время воспроизведения текущей ноты, используется как счетчик: от определенного значения до 0, после чего тональность ноты изменяется, и этот параметр снова инициализируется.
char end_of_melody = 0; // 1 - дорожка музыки закончилась, 0 - иначе.

const unsigned short notes[] = {
        392, 392, 392, 311, 466, 392, 311, 466, 392,
		587, 587, 587, 622, 466, 369, 311, 466, 392,
		784, 392, 392, 784, 739, 698, 659, 622, 659,
		415, 554, 523, 493, 466, 440, 466,
		311, 369, 311, 466, 392
};

const unsigned short ok_btn_notes[] = {
	200, 400, 600
};

const unsigned short err_btn_notes[] = {
	600, 400, 200
};

unsigned char note_delay[2];

// нормировочная функция для калибровки времени воспроизведения нот с разной частотой к какой-то нормали
unsigned int get_scaled_delay(unsigned short note) {
	float koef = note/(float)482; // 482 - среднее значение частоты, посчитанное ручками, для конкретной мелодии.
	return (unsigned int)(450 * koef); // 300 - среднее время воспроизведения одной ноты (в единицах цикла). Находится подгонианом))0)
}

void set_next_note(unsigned char mode){
	compute_note_delay(mode, cur_note_ind);
	if (mode == 2)
		cur_note_time = get_scaled_delay(notes[cur_note_ind]);
	else
		cur_note_time = 50;
	
	cur_note_ind++;
	if (mode == 2)
		cur_note_ind = cur_note_ind % 39; // 39 - кол-во элементов notes[]
	else 
		cur_note_ind = cur_note_ind % 3;
	
	if (cur_note_ind == 0)
		end_of_melody = 1;
		
	RCAP2H = note_delay[1];
	RCAP2L = note_delay[0];
}

void note_handler() __interrupt(5)
{
    TF2 = 0;                    // Like the Serial Interrupt, Timer 2 interrupt does not clear the interrupt flag that triggered the interrupt.
    EXF2 = 0;                   // Thus, be sure to always clear TF2 and EXF2 in your Timer 2 Interrupt.
	
    current_ena ^= volume; // Временная отрубка: инверсия битов, отвечающих за громкость. Не спрашивайте, почему, еобходимо отрубать-врубать, чтобы был звук.
	write_max((unsigned char __xdata *)ENA, current_ena);
	
	if (end_of_melody == 1)
		T2CON = 0x00; // Выключение таймера 2
	if (--cur_note_time == 0) 
		set_next_note(cur_mode);
}

void set_volume(unsigned char v) {
	current_ena = read_max((unsigned char __xdata *)ENA);
    current_ena |= v; // установить громкость
	write_max((unsigned char __xdata *)ENA, current_ena);
}

/* 
mode: 
	0 - success key press
	1 - failure key press
	2 - end of cook process
*/
void make_sound(unsigned char mode)
{	
	T2CON = 0x00; // Выключение таймера 2

	end_of_melody = 0;
	cur_note_ind = 0;
    set_next_note(mode);
	cur_mode = mode; // Необходимо для того, чтобы обработчик прерывания идентифицировал, какой режим сейчас работает.

    T2CON = 0x04; // Включение таймера 2
}

void init_sound(void)
{
    set_volume(volume);
    set_vector(0x202B, (void *)note_handler);
    IP |= 0x20;
    ET2 = 1; // Разрешаем прерывания от таймера 2
    EA = 1;  // Разрешаем все прерывания
}
