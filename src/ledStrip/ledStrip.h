//
// Created by Koba on 03.02.2023.
//

#ifndef ESP32_COLOR_MUSIC_LEDSTRIP_H
#define ESP32_COLOR_MUSIC_LEDSTRIP_H

#include "FastLED.h"

enum NotificationLedStrip : uint8_t {
    SET_COLOR, SET_LEDS
};

class LedStrip {
public:
     LedStrip();
    ~LedStrip();

    void start(CRGB *leds, uint16_t ledsLength);
    void showColor(CRGB color);
    void show();
    void clear();
    void setBrightness(uint8_t bright);

    CRGB *leds;
    uint16_t length;

protected:
    TaskHandle_t handleShowTask = nullptr;
    [[noreturn]] static void sendExecutor(void *pvParam);
    CRGB showColorValue;
};

#endif //ESP32_COLOR_MUSIC_LEDSTRIP_H
