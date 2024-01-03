#include "ledStrip.h"

LedStrip::LedStrip() {
    this->leds = nullptr;
    this->lengthLeds = 0;
}

LedStrip::~LedStrip() {
    if (handleShowTask) {
        vTaskDelete(handleShowTask);
        handleShowTask = nullptr;
    }
    delete []leds;
//    leds = nullptr;
}

void LedStrip::init(uint16_t ledsLength_) {
    lengthLeds = ledsLength_;
    leds = new CRGB[lengthLeds];
    CFastLED::addLeds<WS2812B, WS2812B_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
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
            default:
                break;
        }
    }
}

void LedStrip::showColor(CRGB color) {
    if (lengthLeds == 0) return;
    showColorValue = color;
    xTaskNotify(handleShowTask, SET_COLOR, eSetValueWithOverwrite);
}

void LedStrip::show() {
    if (lengthLeds == 0) return;
    xTaskNotify(handleShowTask, SET_LEDS, eSetValueWithOverwrite);
}

void LedStrip::clear() const {
    for (uint16_t i = 0; i < lengthLeds; ++i) leds[i] = CRGB::Black;
}

void LedStrip::setBrightness(uint8_t bright_) {
    bright = bright_;
    FastLED.setBrightness(bright);
}

uint8_t LedStrip::getBrightness() const {
    return bright;
}

void LedStrip::changeBrightness(int16_t value) {
    value += bright;
    uint8_t newValue = value > 255 ? 255 : value < 0 ? 0 : value;
    setBrightness(newValue);
}