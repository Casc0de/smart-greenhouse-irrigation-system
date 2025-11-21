#ifndef CONTROLADORTIEMPO_H
#define CONTROLADORTIEMPO_H

#include "Horario.h"
#include <Wire.h>
#include <RTClib.h>

class ControladorTiempo
{
public:
    Horario horarios[7]; // Arreglo de horarios para cada día de la semana
    RTC_DS3231 rtc;      // Objeto Rtc para manejar el tiempo real

    ControladorTiempo(); // constructor
    void begin();
};

#endif