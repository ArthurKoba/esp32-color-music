//
// Created by Koba on 03.02.2023.
//

#include "ledStrip.h"

LedStrip::LedStrip(CRGB *leds, uint16_t ledsLength) {
    this->leds = leds;
    this->ledsLength = ledsLength;
    showColorQueue = xQueueCreate(1, sizeof(CRGB));
    xTaskCreate(
            LedStrip::sendExecutor,
            "LedStripShowTask",
            2048,
            this,
            9, // todo setting old
            &handleShowTask);
}

LedStrip::~LedStrip() {
    vTaskDelete(&handleShowTask);
    vQueueDelete(showColorQueue);
}

void LedStrip::sendExecutor(void *pvParam) {
    LedStrip thisObject = *(LedStrip*) pvParam;
    CRGB color;
    while (true) {
//        if (xQueueReceive(thisObject.showColorQueue, &color, pdMS_TO_TICKS(1)) == pdTRUE) {
//            printf("[%lu] сolor notification", millis());
////            if (color.red == 0 == CRGB::Black)
//            FastLED.showColor(color);
//            printf("[%lu] сolor showed\n", millis());
//        }
//
//        if (xTaskNotifyWait(0, 0, 0, pdMS_TO_TICKS(1)) == pdPASS) {
//            printf("[%lu] segment notification\n", millis());
//            FastLED.show();
//            printf("[%lu] segment showed\n", millis());
//        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        FastLED.showColor(CRGB::Red);
        vTaskDelay(pdMS_TO_TICKS(1000));
        FastLED.showColor(CRGB::Black);
        vTaskDelay(pdMS_TO_TICKS(1000));
        thisObject.setRightSegmentColor(CRGB::Blue);
        FastLED.show();
        vTaskDelay(pdMS_TO_TICKS(1000));
        FastLED.showColor(CRGB::Black);

    }
}

void LedStrip::showColor(CRGB color) {
    xQueueOverwrite(showColorQueue, &color);
}

void LedStrip::show() {
    xTaskNotify(handleShowTask, 0, eNoAction);
}

void LedStrip::clearLeds() {
    for (uint16_t i = 0; i < ledsLength; ++i) leds[i] = CRGB::Black;
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

