#include "ControladorRiego.h"

// Duraciones de cada estado (simulación) en ms
static const unsigned long T_INICIO_MS = 5UL * 1000UL;
static const unsigned long T_ENCENDIDO_MOTOR_MS = 20UL * 1000UL;
static const unsigned long T_PRESURIZACION_MS = 20UL * 1000UL;
static const unsigned long T_TANQUE_MS = 20UL * 1000UL;
static const unsigned long T_LAVADO_MS = 20UL * 1000UL;
static const unsigned long T_TRANSICION_SECCION_MS = 20UL * 1000UL;
static const unsigned long T_CIERRE_MS = 20UL * 1000UL;

ControladorRiego::ControladorRiego()
    : bomba(11, 12),
      valvulasOnOff{
          ValvulaOnOff(4),
          ValvulaOnOff(5),
          ValvulaOnOff(6),
          ValvulaOnOff(9),
          ValvulaOnOff(10)},
      valvulasEstado{
          ValvulaEstado(2, 3),
          ValvulaEstado(7, 8)},
      _estadoActual(EST_FIN_CICLO), // arrancamos “en reposo”
      _tInicioEstado(0),
      _entryPendiente(true),
      _seccionActual(0)
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

    // Estado inicial: sistema en reposo
    _estadoActual = EST_FIN_CICLO;
    _tInicioEstado = millis();
    _entryPendiente = true;
    _seccionActual = 0;
}

void ControladorRiego::iniciarCiclo()
{
    // Solo arrancar si estamos “quietos”
    if (_estadoActual == EST_FIN_CICLO || _estadoActual == EST_INICIO_RIEGO)
    {
        cambiarEstado(EST_INICIO_RIEGO);
    }
}

void ControladorRiego::cambiarEstado(EstadoRiego nuevo)
{
    _estadoActual = nuevo;
    _tInicioEstado = millis();
    _entryPendiente = true; // marca que falta hacer ENTRY del nuevo estado
}

void ControladorRiego::leerValoresValvulasOnOff()
{
    for (int i = 0; i < NUM_VALVULAS_ON_OFF; ++i)
    {
        Serial.println("VálvulaOnOff " + String(i) + " leída.");
        Serial.println("  Posicion: " + String(valvulasOnOff[i].posicion));
    }
}

void ControladorRiego::leerValoresValvulasEstado()
{
    for (int i = 0; i < NUM_VALVULAS_ESTADO; ++i)
    {
        Serial.println("VálvulaEstado " + String(i) + " leída.");
        Serial.println("  Posicion: " + String(valvulasEstado[i].posicion));
    }
}

// Máquina de estados principal
void ControladorRiego::regar()
{
    unsigned long ahora = millis();
    unsigned long tEnEstado = ahora - _tInicioEstado;
    static unsigned long _ultimoPrint = 0;

    // Actualizar SIEMPRE las válvulas de estado (no bloqueantes)
    for (int i = 0; i < NUM_VALVULAS_ESTADO; i++)
    {
        valvulasEstado[i].actualizar();
    }

    // ==== DEBUG CADA 1 SEGUNDO ====
    if (ahora - _ultimoPrint >= 1000UL)
    {
        _ultimoPrint = ahora;

        Serial.print("[ESTADO] ");
        switch (_estadoActual)
        {
        case EST_INICIO_RIEGO:
            Serial.print("INICIO_RIEGO");
            break;
        case EST_ENCENDIDO_MOTOR:
            Serial.print("ENCENDIDO_MOTOR");
            break;
        case EST_PRESURIZACION:
            Serial.print("PRESURIZACION");
            break;
        case EST_TANQUE1:
            Serial.print("TANQUE1");
            break;
        case EST_TANQUE2:
            Serial.print("TANQUE2");
            break;
        case EST_TANQUE3:
            Serial.print("TANQUE3");
            break;
        case EST_LAVADO_FINAL:
            Serial.print("LAVADO_FINAL");
            break;
        case EST_TRANSICION_SECCION:
            Serial.print("TRANSICION_SECCION");
            break;
        case EST_CIERRE_OPERACIONES:
            Serial.print("CIERRE_OPERACIONES");
            break;
        case EST_FIN_CICLO:
            Serial.print("FIN_CICLO");
            break;
        case EST_ERROR_MOTOR:
            Serial.print("ERROR_MOTOR");
            break;
        }
        Serial.print(" | t = ");
        Serial.print(tEnEstado / 1000);
        Serial.println(" s");
    }

    // Chequeo global de alarma de motor
    if (digitalRead(12) == LOW) // o HIGH, según tu wiring real
    {
        cambiarEstado(EST_ERROR_MOTOR);
    }

    switch (_estadoActual)
    {
    // =========================================================
    // ESTADO: INICIO_RIEGO
    // =========================================================
    case EST_INICIO_RIEGO:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            // ENTRY: se ejecuta SOLO UNA VEZ al entrar aquí
            // Puedes hacer logs, reset de contadores, etc.
        }

        // DO: por ahora solo esperar T_INICIO_MS antes de arrancar motor
        if (tEnEstado >= T_INICIO_MS)
        {
            cambiarEstado(EST_ENCENDIDO_MOTOR);
        }
        break;

    // =========================================================
    // ESTADO: ENCENDIDO_MOTOR
    // =========================================================
    case EST_ENCENDIDO_MOTOR:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            _seccionActual = 0; // empezamos en sección A
        }

        // DO:
        bomba.encender();
        valvulasOnOff[3].abrir(); // válvula de sección A

        // EXIT
        if (tEnEstado >= T_ENCENDIDO_MOTOR_MS)
        {
            cambiarEstado(EST_PRESURIZACION);
        }
        break;

    // =========================================================
    // ESTADO: PRESURIZACION
    // =========================================================
    case EST_PRESURIZACION:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            // ENTRY:
            // - Poner agua al 100%
            valvulasEstado[0].abrir(12); // tiempo real de apertura
        }

        // DO: más adelante meterás control de presión aquí

        // EXIT
        if (tEnEstado >= T_PRESURIZACION_MS)
        {
            cambiarEstado(EST_TANQUE1);
        }
        break;

    // =========================================================
    // ESTADO: TANQUE1
    // =========================================================
    case EST_TANQUE1:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            // ENTRY:
            // - Poner agua al 50% (cerrando un poco la válvula)
            valvulasEstado[0].cerrar(6);
            // Abrir tanque 1
            valvulasOnOff[0].abrir();
        }

        // DO: aquí luego leerás nivel real del tanque 1

        // EXIT
        if (tEnEstado >= T_TANQUE_MS)
        {
            valvulasOnOff[0].cerrar(); // cerrar tanque 1
            cambiarEstado(EST_TANQUE2);
        }
        break;

    // =========================================================
    // ESTADO: TANQUE2
    // =========================================================
    case EST_TANQUE2:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            // ENTRY:
            valvulasOnOff[1].abrir(); // abrir tanque 2
        }

        // DO: luego pondrás el nivel real del tanque 2

        // EXIT
        if (tEnEstado >= T_TANQUE_MS)
        {
            valvulasOnOff[1].cerrar(); // cerrar tanque 2
            cambiarEstado(EST_TANQUE3);
        }
        break;

    // =========================================================
    // ESTADO: TANQUE3
    // =========================================================
    case EST_TANQUE3:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            // ENTRY:
            valvulasOnOff[2].abrir(); // abrir tanque 3
        }

        // DO: luego pondrás el nivel real del tanque 3

        // EXIT
        if (tEnEstado >= T_TANQUE_MS)
        {
            valvulasOnOff[2].cerrar(); // cerrar tanque 3
            cambiarEstado(EST_LAVADO_FINAL);
        }
        break;

    // =========================================================
    // ESTADO: LAVADO FINAL
    // =========================================================
    case EST_LAVADO_FINAL:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            // ENTRY:
            // Agua al 100%, lavado
            valvulasEstado[0].abrir(6);
        }

        // DO:
        // Nada especial por ahora, solo tiempo

        // EXIT:
        if (tEnEstado >= T_LAVADO_MS)
        {
            if (_seccionActual == 0)
            {
                // Pasar a sección B
                _seccionActual = 1;
                cambiarEstado(EST_TRANSICION_SECCION);
            }
            else
            {
                // Ya fue sección B → cierre total
                _seccionActual = 0;
                cambiarEstado(EST_CIERRE_OPERACIONES);
            }
        }
        break;

    // =========================================================
    // ESTADO: TRANSICION DE SECCION (A → B)
    // =========================================================
    case EST_TRANSICION_SECCION:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            // ENTRY:
            // cerrar sección A, abrir sección B
            valvulasOnOff[3].cerrar(); // sección A
            valvulasOnOff[4].abrir();  // sección B
        }

        // DO: nada más, solo esperar

        // EXIT
        if (tEnEstado >= T_TRANSICION_SECCION_MS)
        {
            cambiarEstado(EST_PRESURIZACION); // presuriza nuevamente para sección B
        }
        break;

    // =========================================================
    // ESTADO: CIERRE DE OPERACIONES
    // =========================================================
    case EST_CIERRE_OPERACIONES:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            // ENTRY:
            // Cerrar agua y sección B
            valvulasEstado[0].cerrar(12);
            valvulasOnOff[4].cerrar();
        }

        // DO: nada más, solo tiempo

        // EXIT
        if (tEnEstado >= T_CIERRE_MS)
        {
            cambiarEstado(EST_FIN_CICLO);
        }
        break;

    // =========================================================
    // ESTADO: FIN CICLO (reposo)
    // =========================================================
    case EST_FIN_CICLO:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            // ENTRY:
            // Dejar todo seguro
            bomba.apagar();
            for (uint8_t i = 0; i < NUM_VALVULAS_ON_OFF; ++i)
            {
                valvulasOnOff[i].cerrar();
            }
            // Las válvulas de estado quedarán quietas
        }
        // DO: el sistema se queda quieto hasta que SistemaRiego llame iniciarCiclo()
        break;

    // =========================================================
    // ESTADO: ERROR MOTOR
    // =========================================================
    case EST_ERROR_MOTOR:
        if (_entryPendiente)
        {
            _entryPendiente = false;

            // ENTRY: apagar TODO por seguridad
            bomba.apagar();
            for (uint8_t i = 0; i < NUM_VALVULAS_ON_OFF; ++i)
            {
                valvulasOnOff[i].cerrar();
            }
            // Intentamos llevar válvulas de estado a posición segura
            for (uint8_t i = 0; i < NUM_VALVULAS_ESTADO; ++i)
            {
                valvulasEstado[i].cerrar(1); // pequeño pulso de cierre
            }
            Serial.println("ERROR: Motor en falla. Sistema detenido.");
        }
        // DO: quedarse aquí hasta reset manual
        break;
    }
}
