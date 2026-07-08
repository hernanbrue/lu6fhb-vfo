#include "vfo_output_settings.h"

bool OutputSettings::toggleRf() {
    m_rfEnabled = !m_rfEnabled;
    return m_rfEnabled;
}

DriveStrengthMa OutputSettings::cycleDrive() {
    switch (m_drive) {
        case DriveStrengthMa::MA2: m_drive = DriveStrengthMa::MA4; break;
        case DriveStrengthMa::MA4: m_drive = DriveStrengthMa::MA6; break;
        case DriveStrengthMa::MA6: m_drive = DriveStrengthMa::MA8; break;
        case DriveStrengthMa::MA8: m_drive = DriveStrengthMa::MA2; break;
    }
    return m_drive;
}
