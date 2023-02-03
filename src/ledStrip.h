//
// Created by Koba on 03.02.2023.
//

#ifndef ESP32_COLOR_MUSIC_LEDSTRIP_H
#define ESP32_COLOR_MUSIC_LEDSTRIP_H

#include <FastLED.h>

class LedStrip {
public:
    LedStrip(CRGB *leds);
private:
    CRGB leds;

};

#endif //ESP32_COLOR_MUSIC_LEDSTRIP_H
