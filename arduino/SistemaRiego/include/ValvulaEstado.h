#ifndef VALVULAESTADO_H
#define VALVULAESTADO_H

#include <Arduino.h>
#include <stdint.h>

class ValvulaEstado
{
private:
    uint8_t _pinOpen;
    uint8_t _pinShut;

public:
    uint8_t posicion = 0;

    ValvulaEstado(uint8_t pinOpen, uint8_t pinShut);
    void begin();
    void abrir(uint8_t t);
    void cerrar(uint8_t t);
};

#endif