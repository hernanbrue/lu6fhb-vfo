#pragma once
#include <stdint.h>

// ---------------------------------------------------------------------
// OutputSettings
// ---------------------------------------------------------------------
// Estado puro de la salida RF (encendido/apagado, potencia). Sin
// dependencias de hardware - AppManager es quien traduce esto a
// llamadas reales sobre VfoSynth.

enum class DriveStrengthMa : uint8_t {
    MA2 = 2,
    MA4 = 4,
    MA6 = 6,
    MA8 = 8
};

class OutputSettings {
public:
    OutputSettings() : m_rfEnabled(true), m_drive(DriveStrengthMa::MA8) {}

    // Invierte el estado de RF y devuelve el nuevo valor.
    bool toggleRf();

    // Pasa a la siguiente potencia (2->4->6->8->2...) y devuelve el
    // nuevo valor.
    DriveStrengthMa cycleDrive();

    bool rfEnabled() const { return m_rfEnabled; }
    DriveStrengthMa drive() const { return m_drive; }

private:
    bool m_rfEnabled;
    DriveStrengthMa m_drive;
};
