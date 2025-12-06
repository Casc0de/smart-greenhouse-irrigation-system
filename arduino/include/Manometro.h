#ifndef MANOMETRO_H
#define MANOMETRO_H

#include <Arduino.h>
#include <stdint.h>

class Manometro
{
private:
    uint8_t _pinIn;

public:
    uint8_t presion = 0; // presión redondeada (0–30 psi)

    Manometro(uint8_t pinIn);
    void begin();

    float medirPresion(); // <-- devuelve PSI real (float)
};

#endif
