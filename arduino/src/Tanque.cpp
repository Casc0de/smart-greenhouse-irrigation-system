#include "Tanque.h"

Tanque::Tanque(uint8_t pinEcho, uint8_t pinTrigger, char tipoFertilizante)
    : _pinEcho(pinEcho),
      _pinTrigger(pinTrigger),
      tipo(tipoFertilizante)
{
}

void Tanque::begin()
{
    pinMode(_pinEcho, INPUT);
    pinMode(_pinTrigger, OUTPUT);
    digitalWrite(_pinTrigger, LOW); // reposo
}

float Tanque::medirNivel()
{
    // --- Pulso TRIG (como el que ya comprobaste que funciona con tu JSN) ---
    digitalWrite(_pinTrigger, LOW);
    delayMicroseconds(5);

    digitalWrite(_pinTrigger, HIGH);
    delayMicroseconds(20); // timing probado en tu laboratorio
    digitalWrite(_pinTrigger, LOW);

    // --- Lectura del ECHO ---
    long duracion = pulseIn(_pinEcho, HIGH, 60000); // timeout 60 ms

    if (duracion == 0)
    {
        // Sin eco: sensor fuera de rango o error.
        // Devolvemos -1.0 para marcarlo claramente como lectura inválida.
        return -1.0f;
    }

    // Distancia en cm (velocidad sonido ~343 m/s → 0.0343 cm/us)
    float distancia = (duracion * 0.0343f / 2.0f);

    // Aquí podrías convertir distancia → porcentaje de nivel si conoces
    // la altura útil del tanque. Ejemplo:
    //
    //   const float ALTURA_TANQUE_CM = 100.0f;
    //   float nivelPct = (1.0f - distancia / ALTURA_TANQUE_CM) * 100.0f;
    //   nivelPct = constrain(nivelPct, 0.0f, 100.0f);
    //   nivel = (uint8_t) nivelPct;
    //
    // Por ahora solo actualizamos nivel de forma “dummy” si quieres:
    // nivel = (uint8_t)constrain(distancia, 0.0f, 255.0f);

    return distancia; // cm
}
