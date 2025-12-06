#include "ValvulaOnOff.h"

ValvulaOnOff::ValvulaOnOff(uint8_t pinOpen)
    : _pinOpen(pinOpen)
{
}

void ValvulaOnOff::begin()
{
    pinMode(_pinOpen, OUTPUT);

    // Asumimos relé low-level trigger:
    // HIGH = relé OFF (válvula cerrada)
    // LOW  = relé ON  (válvula abierta)
    digitalWrite(_pinOpen, HIGH);
    posicion = 0; // cerrada
}

void ValvulaOnOff::abrir()
{
    // Activar relé → abrir válvula
    digitalWrite(_pinOpen, LOW);
    posicion = 1;
}

void ValvulaOnOff::cerrar()
{
    // Desactivar relé → cerrar válvula
    digitalWrite(_pinOpen, HIGH);
    posicion = 0;
}
