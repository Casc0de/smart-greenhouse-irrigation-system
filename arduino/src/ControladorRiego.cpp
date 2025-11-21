#include "ControladorRiego.h"

ControladorRiego::ControladorRiego()
    : bomba(14, 15),
      valvulasOnOff{
          ValvulaOnOff(7),   // valvula 2 : on-off
          ValvulaOnOff(8),   // valvula 3 : on-off
          ValvulaOnOff(9),   // valvula 4 : on-off
          ValvulaOnOff(12),  // valvula 6 : on-off
          ValvulaOnOff(13)}, // valvula 7 : on-off
      valvulasEstado{
          ValvulaEstado(5, 6),   // valvula 1 : estado - agua
          ValvulaEstado(10, 11)} // valvula 5 : estado - presión general
{
}

void ControladorRiego::begin()
{
    ctlSensores.begin(); // Inicializa los sensores
    bomba.begin();       // Inicializa la bomba
    // Inicializa las válvulas On-Off
    for (int i = 0; i < NUM_VALVULAS_ON_OFF; ++i)
    {
        valvulasOnOff[i].begin();
    }
    // Inicializa las válvulas de estado
    for (int i = 0; i < NUM_VALVULAS_ESTADO; ++i)
    {
        valvulasEstado[i].begin();
    }
}

void ControladorRiego::leerValoresValvulasOnOff()
{
    for (int i = 0; i < NUM_VALVULAS_ON_OFF; ++i)
    {
        Serial.println("Válvula " + String(i) + " leída.");
        Serial.println(" ValorPos: " + String(valvulasOnOff[i].posicion));
    }
}

void ControladorRiego::leerValoresValvulasEstado()
{
    for (int i = 0; i < NUM_VALVULAS_ESTADO; ++i)
    {
        Serial.println("Válvula " + String(i) + " leída.");
        Serial.println(" ValorPos: " + String(valvulasEstado[i].posicion));
    }
}

// TODO: ESPERAR A BURGOS
void ControladorRiego::regar()
{
    // Lógica para encender la bomba y mover las válvulas según la presión del manómetro.
    // Además de que le envía un True a la Raspberry para indicar que se va a regar
    // Esto último hacerlo en el main, no aquí dentro de la función

    valvulasOnOff[0].abrir();
    valvulasEstado[0].abrir(6); // Abrir válvula de estado por 6 segundos. La func acaba despúes de ese tiempo
    bomba.encender();
    ctlSensores.manometroSensor.medirPresion();
    // Aquí hacer ciclo para mantener la presión adecuada
    // for ...{
    if (ctlSensores.manometroSensor.presion < 10)
    {
        Serial.println("Presión baja, ajustando...");
        // Lógica para ajustar la presión
        valvulasEstado[1].abrir(1); // Abrir válvula de estado por 1 segundos para aumentar presión
    }
    // }
    bomba.apagar();
    valvulasEstado[0].cerrar(12); // Cerrar válvula de estado
    valvulasOnOff[0].cerrar();
}