/*

M = Monday
T = Tuesday
W = Wednesday
R = Thursday (to avoid confusion with Tuesday)
F = Friday
S = Saturday
U = Sunday
*/

#ifndef TANQUE_H
#define TANQUE_H

#include <Arduino.h>
#include <stdint.h>

class Tanque
{
private:
    uint8_t _pinEcho;
    uint8_t _pinTrigger;

public:
    uint8_t nivel = 0; // Nivel de fertilizante en el tanque
    char tipo;         // Tipo de fertilizante por día

    Tanque(uint8_t pinEcho,
           uint8_t pinTrigger,
           char tipoFertilizante); // constructor
    void begin();                  // inicializador de Hardware para Arduino
    uint8_t medirNivel();
};

#endif