#include "Tanque.h"

Tanque::Tanque(uint8_t pinEcho, uint8_t pinTrigger, char tipoFertilizante) // Constructor
    : _pinEcho(pinEcho),
      _pinTrigger(pinTrigger),
      tipo(tipoFertilizante)
{
}

void Tanque::begin()
{
    pinMode(_pinEcho, INPUT);
    pinMode(_pinTrigger, OUTPUT);
}

// TODO:
uint8_t Tanque::medirNivel()
{
    // TODO: Lógica para medir el nivel del tanque

    return nivel;
}