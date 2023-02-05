#include "colorModes.h"

ColorModes::ColorModes(CustomLedStrip &strip) : strip(strip) {
    this->mode = OFF_MODE;
}

void ColorModes::setMode(ColorMode newMode) {
    this->mode = newMode;
    if (newMode == OFF_MODE) strip.showColor(CRGB::Black);
}

void ColorModes::show() {
    switch (mode) {
        case OFF_MODE:
            break;
        case RAINBOW_MODE:
            rainbowMode();
            break;
    }
}

void ColorModes::rainbowMode() {
    static uint8_t index = 0;
    for (uint16_t i = 0; i < strip.length; i++) strip.leds[i] = static_cast<CRGB>(CHSV(index - i, 255, 255));
    index++;
    strip.show();
    delay(delays.rainbow);
}
