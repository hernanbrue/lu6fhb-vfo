#include "display_driver.h"
#include "config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <stdio.h>

namespace VfoDisplay {

namespace {
Adafruit_SSD1306 s_oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

// Formatea Hz como "7.100.000" (separador de miles con punto, estilo
// que se lee comodo en un VFO). Devuelve la cantidad de caracteres
// escritos en buf.
int formatFrequency(uint64_t freqHz, char* buf, size_t bufLen) {
    char raw[16];
    snprintf(raw, sizeof(raw), "%llu", (unsigned long long)freqHz);
    int rawLen = strlen(raw);

    int w = 0;
    for (int i = 0; i < rawLen; i++) {
        if (i > 0 && (rawLen - i) % 3 == 0) {
            if (w < (int)bufLen - 1) buf[w++] = '.';
        }
        if (w < (int)bufLen - 1) buf[w++] = raw[i];
    }
    buf[w] = '\0';
    return w;
}
}

bool begin() {
    if (!s_oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
        return false;
    }
    s_oled.clearDisplay();
    s_oled.setTextColor(SSD1306_WHITE);
    s_oled.display();
    return true;
}

void render(uint64_t freqHz, uint32_t stepHz) {
    char freqBuf[16];
    formatFrequency(freqHz, freqBuf, sizeof(freqBuf));

    char stepBuf[24];
    snprintf(stepBuf, sizeof(stepBuf), "Paso: %lu Hz", (unsigned long)stepHz);

    s_oled.clearDisplay();

    s_oled.setTextSize(2);
    s_oled.setCursor(0, 20);
    s_oled.print(freqBuf);
    s_oled.print(" Hz");

    s_oled.setTextSize(1);
    s_oled.setCursor(0, 50);
    s_oled.print(stepBuf);

    s_oled.display();
}

} // namespace VfoDisplay
