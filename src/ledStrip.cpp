#include "ledStrip.h"

LedStrip::LedStrip(CRGB *leds, uint16_t ledsLength) {
    this->leds = leds;
    this->ledsLength = ledsLength;
    xTaskCreate(
            LedStrip::sendExecutor,
            "LedStripExecutor",
            2048,
            this,
            9, // todo setting old
            &handleShowTask);
}

LedStrip::~LedStrip() {
    vTaskDelete(&handleShowTask);
}

void LedStrip::sendExecutor(void *pvParam) {
    LedStrip &object = *(LedStrip*)pvParam;
    uint32_t notificationType;
    while (true) {
        if (xTaskNotifyWait(0, 0, &notificationType, portMAX_DELAY) != pdPASS) continue;
        switch (notificationType) {
            case SET_COLOR:
                FastLED.showColor(object.showColorValue);
                break;
            case SET_LEDS:
                FastLED.show();
                break;
        }
    }
}

void LedStrip::showColor(CRGB color) {
    showColorValue = color;
    xTaskNotify(handleShowTask, SET_COLOR, eSetValueWithOverwrite);
}

void LedStrip::show() {
    xTaskNotify(handleShowTask, SET_LEDS, eSetValueWithOverwrite);
}

void LedStrip::clearLeds() {
    for (uint16_t i = 0; i < ledsLength; ++i) leds[i] = CRGB::Black;
}

void LedStrip::setBrightness(uint8_t bright) {
    FastLED.setBrightness(bright);
}

void LedStrip::setLeftColor(CRGB color) {
    for (uint16_t i = 0; i < ledsLength/2; ++i) leds[i] = color;
}

void LedStrip::setRightColor(CRGB color) {
    for (uint16_t i = ledsLength - 1; i > ledsLength/2 - 1; --i) leds[i] = color;
}


void LedStrip::setUpperSegmentColor(CRGB color) {
    for (uint16_t i = 79; i < 151; ++i) leds[i] = color;
}

void LedStrip::setUnderSegmentColor(CRGB color) {
    for (uint16_t i = 0; i < 25; ++i) leds[i] = color;
    for (uint16_t i = ledsLength - 1; i > ledsLength-25 - 1; --i) leds[i] = color;
}

void LedStrip::setLeftSegmentColor(CRGB color) {
    for (uint16_t i = 25; i < 79; ++i) leds[i] = color;
}

void LedStrip::setRightSegmentColor(CRGB color) {
    for (uint16_t i = 151; i < ledsLength-25; ++i) leds[i] = color;
}
