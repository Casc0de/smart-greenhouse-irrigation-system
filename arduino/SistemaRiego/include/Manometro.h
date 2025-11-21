#ifndef MANOMETRO_H
#define MANOMETRO_H

#include <Arduino.h>
#include <stdint.h>

class Manometro
{
private:
    uint8_t _pinIn;

public:
    uint8_t presion = 0;

    Manometro(uint8_t pinIn);
    void begin();
    uint8_t medirPresion();
};

#endif