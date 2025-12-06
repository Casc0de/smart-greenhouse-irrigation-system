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

    // Llamado desde SistemaRiego cuando ControladorTiempo dice “ya es hora”
    void iniciarCiclo();

    void leerValoresValvulasOnOff();
    void leerValoresValvulasEstado();

    // Máquina de estados: se llama en loop() (o desde SistemaRiego::actualizar)
    void regar();

private:
    // ===== Máquina de estados de riego =====
    enum EstadoRiego
    {
        EST_INICIO_RIEGO,
        EST_ENCENDIDO_MOTOR,
        EST_PRESURIZACION,
        EST_TANQUE1,
        EST_TANQUE2,
        EST_TANQUE3,
        EST_LAVADO_FINAL,
        EST_TRANSICION_SECCION,
        EST_CIERRE_OPERACIONES,
        EST_ERROR_MOTOR,
        EST_FIN_CICLO
    };

    EstadoRiego _estadoActual;    // estado actual de la máquina
    unsigned long _tInicioEstado; // millis() al entrar al estado
    bool _entryPendiente;         // para ENTRY/DO/EXIT
    uint8_t _seccionActual;       // 0 = Sección A, 1 = Sección B

    // Helper interno para cambio de estado
    void cambiarEstado(EstadoRiego nuevo);
};

#endif
