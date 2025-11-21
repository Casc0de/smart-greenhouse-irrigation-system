/*
M = Monday
T = Tuesday
W = Wednesday
R = Thursday (to avoid confusion with Tuesday)
F = Friday
S = Saturday
U = Sunday
*/

#ifndef CONTROLADORRIEGO_H
#define CONTROLADORRIEGO_H

#include <Arduino.h>
#include "Bomba.h"
#include "ValvulaEstado.h"
#include "ValvulaOnOff.h"
#include "ControladorSensores.h"

class ControladorRiego
{
public:
    ControladorSensores ctlSensores;
    static constexpr uint8_t NUM_VALVULAS_ON_OFF = 5;
    static constexpr uint8_t NUM_VALVULAS_ESTADO = 2;
    Bomba bomba;
    ValvulaOnOff valvulasOnOff[NUM_VALVULAS_ON_OFF];
    ValvulaEstado valvulasEstado[NUM_VALVULAS_ESTADO];

    ControladorRiego(); // constructor
    void begin();
    void leerValoresValvulasOnOff();
    void leerValoresValvulasEstado();
    void regar();
};

#endif