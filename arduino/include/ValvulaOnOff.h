#ifndef VALVULAONOFF_H
#define VALVULAONOFF_H

#include <Arduino.h>
#include <stdint.h>

class ValvulaOnOff
{
private:
    uint8_t _pinOpen;

public:
    uint8_t posicion = 0;

    ValvulaOnOff(uint8_t pinOpen);
    void begin();
    void abrir();
    void cerrar();
};

#endif