#include "colorModes.h"

void ColorModes::setMode(ColorMode newMode) {
    if (mode == newMode) return;
//    strip.clear(); strip.show();
    if (mode != newMode && newMode == COLOR_MUSIC_MODE) colorMusic.enable();
    if (mode == COLOR_MUSIC_MODE && newMode != COLOR_MUSIC_MODE) colorMusic.disable();
    this->mode = newMode;
}

void ColorModes::show() {
    switch (mode) {
        case OFF_MODE:
            break;
        case RAINBOW_MODE:
            rainbowMode();
            break;
        case COLOR_MUSIC_MODE:
            break;
    }
}

void ColorModes::rainbowMode() {
    static uint8_t index = 0;
//    for (uint16_t i = 0; i < strip.lengthLeds; i++)
//        strip.leds[i] = static_cast<CRGB>(CHSV(index - i, 255, 255));
//    index++;
//    strip.show();
    delay(delays.rainbow);
}
