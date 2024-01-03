#ifndef ESP32_COLOR_MUSIC_LEDSTRIP_H
#define ESP32_COLOR_MUSIC_LEDSTRIP_H

#include "FastLED.h"
#include "configs.h"

enum NotificationLedStrip : uint8_t {
    SET_COLOR, SET_LEDS
};

class LedStrip {
public:
     LedStrip();
    ~LedStrip();

    void init(uint16_t ledsLength);
    void showColor(CRGB color);
    void show();
    void clear() const;

    void changeBrightness(int16_t changeValue);
    void setBrightness(uint8_t bright);
    uint8_t getBrightness() const;

    CRGB *leds = nullptr;
    uint16_t lengthLeds = 0;

protected:
    TaskHandle_t handleShowTask = nullptr;
    [[noreturn]] static void sendExecutor(void *context);
    uint8_t bright = 255;
    CRGB showColorValue{};
};

#endif //ESP32_COLOR_MUSIC_LEDSTRIP_H
