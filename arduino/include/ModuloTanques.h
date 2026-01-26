#ifndef MODULOTANQUES_H
#define MODULOTANQUES_H

#include "Tanque.h"

static constexpr uint8_t NUM_TANQUES = 3;

class ModuloTanques
{
private:
    Tanque tanques[NUM_TANQUES];
    int bateriaNivel;

public:
    ModuloTanques(); // constructor
    void begin();
    void medirNivel();
    void medirBateria();
    void enviarDatos();
};

#endif
