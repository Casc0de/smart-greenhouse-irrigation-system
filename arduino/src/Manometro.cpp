#include "Manometro.h"

// Parámetros del sensor lineal 0.5–4.5V → 0–30 psi
static const float V_MIN = 0.5f;
static const float V_MAX = 4.5f;
static const float PSI_MIN = 0.0f;
static const float PSI_MAX = 30.0f;

static const float ADC_REF = 5.0f; // referencia ADC
static const int ADC_MAX = 1023;   // resolución ADC 10 bits

Manometro::Manometro(uint8_t pinIn)
    : _pinIn(pinIn)
{
}

void Manometro::begin()
{
    pinMode(_pinIn, INPUT);
}

float Manometro::medirPresion()
{
    int lecturaADC = analogRead(_pinIn);

    // Convertir a voltaje (0-1023 → 0–5.0V)
    float voltaje = (lecturaADC * ADC_REF) / ADC_MAX;

    // Limitar el rango válido del sensor
    voltaje = constrain(voltaje, V_MIN, V_MAX);

    // Conversión lineal a psi
    float psi = (voltaje - V_MIN) * (PSI_MAX - PSI_MIN) / (V_MAX - V_MIN) + PSI_MIN;

    // Guardar en atributo público redondeado (0–30)
    presion = (uint8_t)(psi + 0.5f);

    return psi; // devuelve la versión precisa
}
