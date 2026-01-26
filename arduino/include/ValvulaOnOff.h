#ifndef VALVULAONOFF_H
#define VALVULAONOFF_H

#include <Arduino.h>
#include <stdint.h>

class ValvulaOnOff
{
private:
    uint8_t _pinOpen;

public:
    ValvulaOnOff(uint8_t pinOpen);
    void begin();
    void abrir();
    void cerrar();
};

#endif