#ifndef CONTROLADORSENSORES_H
#define CONTROLADORSENSORES_H

#include "Tanque.h"
#include "Manometro.h"

// Definir el tamaño del arreglo de tanques

class ControladorSensores
{
public:
    static constexpr uint8_t NUM_TANQUES = 3;
    Manometro manometroSensor;
    Tanque tanques[NUM_TANQUES];

    ControladorSensores(); // constructor
    void begin();
};

#endif
