#include "ControladorTiempo.h"

ControladorTiempo::ControladorTiempo()
    : horarios{
          Horario('U', 6, false),  // Domingo a las 6 AM
          Horario('M', 6, true),   // Lunes a las 6 AM
          Horario('T', 6, false),  // Martes a las 6 AM
          Horario('W', 6, true),   // Miércoles a las 6 AM
          Horario('R', 6, false),  // Jueves a las 6 AM
          Horario('F', 6, true),   // Viernes a las 6 AM
          Horario('S', 18, true)}, // Sábado a las 6 AM
      rtc()
{
}

void ControladorTiempo::begin()
{
    rtc.begin();
    _ultimoDiaRTC = -1;
    _yaDisparoHoy = false;
}

char ControladorTiempo::diaRTCToChar(int dow)
{
    // RTClib: 0=Dom,1=Lun,...,6=Sáb
    switch (dow)
    {
    case 0:
        return 'U'; // Sunday
    case 1:
        return 'M'; // Monday
    case 2:
        return 'T'; // Tuesday
    case 3:
        return 'W'; // Wednesday
    case 4:
        return 'R'; // Thursday
    case 5:
        return 'F'; // Friday
    case 6:
        return 'S'; // Saturday
    default:
        return '?';
    }
}

bool ControladorTiempo::esMomentoDeRiego()
{
    DateTime now = rtc.now();

    int diaRTC = now.dayOfTheWeek(); // 0..6
    char letraDia = diaRTCToChar(diaRTC);

    // Si cambió el día → rearmamos el disparo diario
    if (diaRTC != _ultimoDiaRTC)
    {
        _ultimoDiaRTC = diaRTC;
        _yaDisparoHoy = false;
    }

    // Si ya se disparó hoy, no volvemos a disparar:
    if (_yaDisparoHoy)
        return false;

    // Buscar el horario de hoy
    Horario *hHoy = nullptr;
    for (int i = 0; i < 7; ++i)
    {
        if (horarios[i].diaSemana == letraDia)
        {
            hHoy = &horarios[i];
            break;
        }
    }

    if (hHoy == nullptr)
        return false;

    if (!hHoy->fertiRiego)
        return false; // hoy no se riega

    // Hora actual y programada (solo horas, sin minutos/segundos)
    uint8_t horaActual = now.hour();     // 0..23
    uint8_t horaProg = hHoy->horaARegar; // 0..23

    if (horaActual >= horaProg)
    {
        _yaDisparoHoy = true; // ✅ solo una vez por día
        return true;          // 👉 es momento de arrancar el riego
    }

    return false;
}
