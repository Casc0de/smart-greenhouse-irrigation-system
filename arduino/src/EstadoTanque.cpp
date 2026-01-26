#include "EstadoTanque.h"
// Constructors
EstadoTanque::EstadoTanque(char tipoFertilizante)
    : nivelPorcentaje(100), tipo(tipoFertilizante) // 'U' : undefined
{
}

// Setters
void EstadoTanque::setNivelPorcentaje(uint8_t nivel)
{
    nivelPorcentaje = nivel;
}

void EstadoTanque::setTipo(char tipoFertilizante)
{
    tipo = tipoFertilizante;
}

// Getters
uint8_t EstadoTanque::getNivelPorcentaje()
{
    return nivelPorcentaje;
}

char EstadoTanque::getTipo()
{
    return tipo;
}