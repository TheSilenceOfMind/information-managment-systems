/*
2. Контроллер SDK-1.1 циклически проигрывает восходящую гамму нот первой октавы
(длительность каждой ноты – 1 секунда) и на линейку светодиодов выводит количество
замыканий входа T0 (счетный вход таймера 0 на рис. 5). Подсчет количества замыканий
входа должен быть реализован с помощью таймера-счетчика 0. В результате выполнения
работы должны быть разработаны драйверы системного таймера, таймера-счетчика,
звукового излучателя, светодиодных индикаторов.
*/
#include "helper.h"
#include "timer.h"
#include "speaker.h"

void Init()
{
    InitSpeaker();
    //InitTimer();
    EA = 1;
}

void main()
{
    Init();
    while (1)
        ;
}
