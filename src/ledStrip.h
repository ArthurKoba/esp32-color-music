//
// Created by Koba on 03.02.2023.
//

#ifndef ESP32_COLOR_MUSIC_LEDSTRIP_H
#define ESP32_COLOR_MUSIC_LEDSTRIP_H

#include <FastLED.h>

enum NotificationLedStrip : uint8_t {
    SET_COLOR, SET_LEDS
};

class LedStrip {
public:
    explicit LedStrip(CRGB *leds, uint16_t ledsLength);
    ~LedStrip();

    void showColor(CRGB color);
    void show();
    void clearLeds();
    void setLeftColor(CRGB color);
    void setRightColor(CRGB color);
    void setUpperSegmentColor(CRGB color);
    void setUnderSegmentColor(CRGB color);
    void setLeftSegmentColor(CRGB color);
    void setRightSegmentColor(CRGB color);

    CRGB *leds;
    uint16_t ledsLength;

private:
    CRGB showColorValue;
    TaskHandle_t handleShowTask = nullptr;
    [[noreturn]] static void sendExecutor(void *pvParam);
};

#endif //ESP32_COLOR_MUSIC_LEDSTRIP_H
