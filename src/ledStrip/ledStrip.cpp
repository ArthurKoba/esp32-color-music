#include "ledStrip.h"

LedStrip::LedStrip() {
    this->leds = nullptr;
    this->length = 0;
}

LedStrip::~LedStrip() {
    if (handleShowTask != nullptr) vTaskDelete(handleShowTask);
}

void LedStrip::start(CRGB *ledsPointer, uint16_t ledsLength) {
    this->leds = ledsPointer;
    this->length = ledsLength;
    xTaskCreate(
            LedStrip::sendExecutor,
            "LedStripExecutor",
            LED_STRIP_TASK_STACK_SIZE,
            this,
            LED_STRIP_TASK_PRIORITY,
            &handleShowTask);
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
    if (length == 0) return;
    showColorValue = color;
    xTaskNotify(handleShowTask, SET_COLOR, eSetValueWithOverwrite);
}

void LedStrip::show() {
    if (length == 0) return;
    xTaskNotify(handleShowTask, SET_LEDS, eSetValueWithOverwrite);
}

void LedStrip::clear() {
    for (uint16_t i = 0; i < length; ++i) leds[i] = CRGB::Black;
}

void LedStrip::setBrightness(uint8_t bright) {
    FastLED.setBrightness(bright);
}

uint8_t LedStrip::getBrightness() {
    return FastLED.getBrightness();
}

void LedStrip::changeBrightness(int16_t value) {
    value += getBrightness();
    uint8_t newValue = ((value)<(0)?(0):((value)>(255)?(255):(value)));
    setBrightness(newValue);
}