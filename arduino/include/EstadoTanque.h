#ifndef ESTADOTANQUE_H
#define ESTADOTANQUE_H

#include <stdint.h>

class EstadoTanque
{
private:
    uint8_t nivelPorcentaje;
    char tipo; // Tipo de fertilizante por día
public:
    // Constructor
    EstadoTanque(char tipoFertilizante);
    // Setters
    void setNivelPorcentaje(uint8_t nivel);
    void setTipo(char tipoFertilizante);
    // Getters
    uint8_t getNivelPorcentaje();
    char getTipo();
};

#endif