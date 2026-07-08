#include "band_selector.h"

Band BandSelector::cycle() {
    m_index = (m_index + 1) % BAND_COUNT;
    return current();
}
