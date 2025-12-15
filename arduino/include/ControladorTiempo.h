#ifndef CONTROLADORTIEMPO_H
#define CONTROLADORTIEMPO_H

#include "Horario.h"
#include <Wire.h>
#include <RTClib.h>

class ControladorTiempo
{
public:
    Horario horarios[7]; // Arreglo de horarios para cada día de la semana
    RTC_DS3231 rtc;      // Objeto RTC para manejar el tiempo real

    ControladorTiempo(); // constructor
    void begin();

    // Devuelve true SOLO la primera vez que se cumple la hora
    // de riego programada del día actual.
    bool esMomentoDeRiego();

private:
    int _ultimoDiaRTC = -1; // 0..6 (0=Domingo según RTClib)
    bool _yaDisparoHoy = false;

    // Convierte dayOfTheWeek() de RTClib (0..6) a tu codificación 'U','M','T','W','R','F','S'
    char diaRTCToChar(int dow);
};

#endif
