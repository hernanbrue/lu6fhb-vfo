#include "vfo_logic.h"

VfoLogic::VfoLogic(uint64_t initialFreqHz, uint32_t initialStepIndex)
    : m_freqHz(initialFreqHz), m_stepIndex(initialStepIndex) {}

uint64_t VfoLogic::clamp(uint64_t freqHz) const {
    if (freqHz < VFO_FREQ_MIN_HZ) return VFO_FREQ_MIN_HZ;
    if (freqHz > VFO_FREQ_MAX_HZ) return VFO_FREQ_MAX_HZ;
    return freqHz;
}

bool VfoLogic::stepFrequency(int32_t direction) {
    uint64_t before = m_freqHz;
    uint64_t step = stepHz();

    if (direction > 0) {
        m_freqHz = clamp(m_freqHz + step);
    } else if (direction < 0) {
        // evita underflow si step > frecuencia actual
        uint64_t candidate = (m_freqHz > step) ? (m_freqHz - step) : VFO_FREQ_MIN_HZ;
        m_freqHz = clamp(candidate);
    }

    return m_freqHz != before;
}

void VfoLogic::cycleStep() {
    m_stepIndex = (m_stepIndex + 1) % VFO_STEP_TABLE_LEN;
}

bool VfoLogic::setFrequency(uint64_t freqHz) {
    uint64_t before = m_freqHz;
    m_freqHz = clamp(freqHz);
    return m_freqHz != before;
}
