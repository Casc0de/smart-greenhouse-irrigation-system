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

#include "Manometro.h"
#include "EstadoTanque.h"
#include "ValvulaOnOff.h"
#include "ValvulaEstado.h"
#include "Bomba.h"
#include "ServicioComunicacion.h"

static constexpr uint8_t NUM_VALVULAS_ON_OFF = 5;
static constexpr uint8_t NUM_VALVULAS_ESTADO = 2;
static constexpr uint8_t NUM_TANQUES = 3;

class ControladorRiego
{
private:
    Bomba bomba;
    Manometro manometroSensor;

    EstadoTanque tanquesEstado[NUM_TANQUES];
    ValvulaOnOff valvulasOnOff[NUM_VALVULAS_ON_OFF];
    ValvulaEstado valvulasEstado[NUM_VALVULAS_ESTADO];

    ServicioComunicacion &comms;

public:
    ControladorRiego(ServicioComunicacion &commsExistente); // constructor
    void begin();
    void regar();
};

#endif