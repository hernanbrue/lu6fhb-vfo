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

void renderSplash() {
    s_oled.clearDisplay();
    s_oled.setTextSize(2);
    // Centrado aproximado: fuente 6px/char a tamaño 2, APP_TITLE de 10
    // caracteres -> ~120px de ancho sobre 128px de pantalla.
    int textWidthPx = strlen(APP_TITLE) * 12;
    int x = (OLED_WIDTH - textWidthPx) / 2;
    if (x < 0) x = 0;
    s_oled.setCursor(x, 24);
    s_oled.print(APP_TITLE);
    s_oled.display();
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

void renderMenu(uint8_t cursorIndex, bool rfEnabled, DriveStrengthMa drive,
                 const char* bandLabel) {
    char rfLine[20];
    snprintf(rfLine, sizeof(rfLine), "RF: %s", rfEnabled ? "ON" : "OFF");

    char powerLine[24];
    snprintf(powerLine, sizeof(powerLine), "Potencia: %u mA",
             (unsigned)drive);

    char bandLine[24];
    snprintf(bandLine, sizeof(bandLine), "Banda: %s", bandLabel);

    const char* labels[4] = {rfLine, powerLine, bandLine, "Salir"};

    s_oled.clearDisplay();

    // Cabecera: titulo de la aplicacion + linea separadora
    s_oled.setTextSize(1);
    s_oled.setCursor(0, 0);
    s_oled.print(APP_TITLE);
    s_oled.drawFastHLine(0, 10, OLED_WIDTH, SSD1306_WHITE);

    // 4 items debajo de la cabecera, espaciado ajustado para que
    // entren en los 64px de alto sin amontonarse.
    for (uint8_t i = 0; i < 4; i++) {
        int y = 14 + i * 12;
        s_oled.setCursor(0, y);
        s_oled.print(i == cursorIndex ? "> " : "  ");
        s_oled.print(labels[i]);
    }

    s_oled.display();
}

} // namespace VfoDisplay
