#include "ControladorRiego.h"

// Duraciones de cada estado (simulación) en ms
static const unsigned long T_INICIO_MS = 5UL * 1000UL;
static const unsigned long T_ENCENDIDO_MOTOR_MS = 10UL * 1000UL;
static const unsigned long T_PRESURIZACION_MS = 10UL * 1000UL;
static const unsigned long T_TANQUE_MS = 20UL * 1000UL;
static const unsigned long T_LAVADO_MS = 20UL * 1000UL;
static const unsigned long T_TRANSICION_SECCION_MS = 20UL * 1000UL;
static const unsigned long T_CIERRE_MS = 20UL * 1000UL;

ControladorRiego::ControladorRiego(ControladorComunicacionConRaspberry &ctlComExistente)
    : ctlComunicacion(ctlComExistente),
      bomba(11, 12),
      valvulasOnOff{
          ValvulaOnOff(4),   // Tanque 1
          ValvulaOnOff(5),   // Tanque 2
          ValvulaOnOff(6),   // Tanque 3
          ValvulaOnOff(9),   // Sección A
          ValvulaOnOff(10)}, // Sección B
      valvulasEstado{
          ValvulaEstado(2, 3),      // Válvula de agua principal
          ValvulaEstado(7, 8)},     // Presión
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
    /*
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
            Serial.print("PRESURIZACION\n");
            ctlComunicacion.enviarManometro(ctlSensores.manometroSensor);
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
    */
    // Chequeo global de alarma de motor
    if (digitalRead(12) == LOW) // o HIGH, según tu wiring real
    {
        // Serial.println("ALARMA DE MOTOR ACTIVADA!");
        cambiarEstado(EST_ERROR_MOTOR);
    }

    switch (_estadoActual)
    {
    // =========================================================
    // ESTADO: INICIO_RIEGO
    // =========================================================
    case EST_INICIO_RIEGO:
        // ENTRY:
        if (_entryPendiente)
        {
            ctlSensores.tanques[0].nivel = 100;
            ctlSensores.tanques[1].nivel = 100;
            ctlSensores.tanques[2].nivel = 100;
            // Enviar información de los tanques
            ctlComunicacion.enviarTanque(ctlSensores.tanques[0]);
            ctlComunicacion.enviarTanque(ctlSensores.tanques[1]);
            ctlComunicacion.enviarTanque(ctlSensores.tanques[2]);

            _entryPendiente = false;
            // ENTRY: se ejecuta SOLO UNA VEZ al entrar aquí
            // Puedes hacer logs, reset de contadores, etc.
        }

        // DO:
        // Por ahora solo esperar T_INICIO_MS antes de arrancar motor

        // EXIT:
        if (tEnEstado >= T_INICIO_MS)
        {
            cambiarEstado(EST_ENCENDIDO_MOTOR);
        }
        break;

    // =========================================================
    // ESTADO: ENCENDIDO_MOTOR
    // =========================================================
    case EST_ENCENDIDO_MOTOR:
        // ENTRY:
        if (_entryPendiente)
        {
            bomba.funcionando = true;
            bomba.errorAlarma = false;
            ctlComunicacion.enviarEstadoBomba(bomba);

            _entryPendiente = false;
            _seccionActual = 0; // empezamos en sección A
        }

        // DO:
        bomba.encender();
        valvulasOnOff[3].abrir(); // válvula de sección A

        // EXIT:
        if (tEnEstado >= T_ENCENDIDO_MOTOR_MS)
        {
            cambiarEstado(EST_PRESURIZACION);
        }
        break;

    // =========================================================
    // ESTADO: PRESURIZACION
    // =========================================================
    case EST_PRESURIZACION:
        // ENTRY:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            // - Poner agua al 100%
            valvulasEstado[0].abrir(12); // tiempo real de apertura
        }

        // DO:
        // TODO: meter control de presión aquí

        // EXIT:
        if (tEnEstado >= T_PRESURIZACION_MS)
        {
            cambiarEstado(EST_TANQUE1);
        }
        break;

    // =========================================================
    // ESTADO: TANQUE1
    // =========================================================
    case EST_TANQUE1:
        // ENTRY:
        if (_entryPendiente)
        {
            // Enviar datos del tanque 1
            if (_seccionActual == 0)
            {
                ctlSensores.tanques[0].nivel = 50; // simular tanque medio vacío
            }
            else
            {
                ctlSensores.tanques[0].nivel = 0; // simular tanque vacío
            }
            ctlComunicacion.enviarTanque(ctlSensores.tanques[0]);

            // Serial.println("=== Entrando en estado TANQUE1 ===");
            _entryPendiente = false;
            // - Poner agua al 50% (cerrando un poco la válvula)
            valvulasEstado[0].cerrar(6);
            // Abrir tanque 1
            valvulasOnOff[0].abrir();
        }

        // DO:
        // TODO: lógica de que acabe cuando el nivel del tanque 1 haya bajado lo que corresponde

        // EXIT:
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
        // ENTRY:
        if (_entryPendiente)
        {
            // Enviar datos del tanque 2
            if (_seccionActual == 0)
            {
                ctlSensores.tanques[1].nivel = 50; // simular tanque medio vacío
            }
            else
            {
                ctlSensores.tanques[1].nivel = 0; // simular tanque vacío
            }
            ctlComunicacion.enviarTanque(ctlSensores.tanques[1]);

            _entryPendiente = false;
            // Serial.println("=== Entrando en estado TANQUE2 ===");
            valvulasOnOff[1].abrir(); // abrir tanque 2
        }

        // DO:
        // TODO: lógica de que acabe cuando el nivel del tanque 2 haya bajado lo que corresponde

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
        // ENTRY:
        if (_entryPendiente)
        {
            // Enviar datos del tanque 3
            if (_seccionActual == 0)
            {
                ctlSensores.tanques[2].nivel = 50; // simular tanque medio vacío
            }
            else
            {
                ctlSensores.tanques[2].nivel = 0; // simular tanque vacío
            }
            ctlComunicacion.enviarTanque(ctlSensores.tanques[2]);

            _entryPendiente = false;
            // Serial.println("=== Entrando en estado TANQUE3 ===");
            valvulasOnOff[2].abrir(); // abrir tanque 3
        }

        // DO:
        // TODO: lógica de que acabe cuando el nivel del tanque 3 haya bajado lo que corresponde

        // EXIT:
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
        // ENTRY:
        if (_entryPendiente)
        {
            _entryPendiente = false;
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
        // ENTRY:
        if (_entryPendiente)
        {
            _entryPendiente = false;
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
        // ENTRY:
        if (_entryPendiente)
        {
            _entryPendiente = false;
            // Cerrar agua y sección B
            valvulasEstado[0].cerrar(12);
            valvulasOnOff[4].cerrar();
        }

        // DO:
        // nada más, solo tiempo

        // EXIT
        if (tEnEstado >= T_CIERRE_MS)
        {
            bomba.funcionando = false;
            ctlComunicacion.enviarEstadoBomba(bomba);

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
            bomba.errorAlarma = true;

            ctlComunicacion.enviarEstadoBomba(bomba);
            for (uint8_t i = 0; i < NUM_VALVULAS_ON_OFF; ++i)
            {
                valvulasOnOff[i].cerrar();
            }
            // Intentamos llevar válvulas de estado a posición segura
            for (uint8_t i = 0; i < NUM_VALVULAS_ESTADO; ++i)
            {
                valvulasEstado[i].cerrar(1); // pequeño pulso de cierre
            }
            // Serial.println("ERROR: Motor en falla. Sistema detenido.");
        }
        // DO: quedarse aquí hasta reset manual
        break;
    }
}
