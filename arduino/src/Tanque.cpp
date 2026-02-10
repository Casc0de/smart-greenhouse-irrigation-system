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

    // digitalWrite(_pinTrigger, LOW); // reposo
}

float Tanque::medirNivel()
{
    digitalWrite(_pinTrigger, HIGH);
    delayMicroseconds(10); // Enviamos un pulso de 10us
    digitalWrite(_pinTrigger, LOW);

    // --- Lectura del ECHO ---
    long duracion = pulseIn(_pinEcho, HIGH); // obtenemos el ancho del pulso

    // Distancia en cm (velocidad sonido ~343 m/s → 0.0343 cm/us)
    float distancia = duracion / 59; // escalamos el tiempo a una distancia en cm
    // Serial.print("(Dentro de la función: distancia medida: ");
    // Serial.println(distancia);

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
    delay(100);
    return distancia; // cm
}
