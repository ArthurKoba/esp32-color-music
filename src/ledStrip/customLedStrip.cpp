#include "customLedStrip.h"

void CustomLedStrip::setLeftColor(CRGB color) {
    for (uint16_t i = 0; i < length/2 && leds != nullptr; ++i) leds[i] = color;
}

void CustomLedStrip::setRightColor(CRGB color) {
    for (uint16_t i = length - 1; i > length/2 - 1 && leds != nullptr; --i) leds[i] = color;
}

void CustomLedStrip::setUpperSegmentColor(CRGB color) {
    for (uint16_t i = 79; i < 151 && leds != nullptr; ++i) leds[i] = color;
}

void CustomLedStrip::setUnderSegmentColor(CRGB color) {
    for (uint16_t i = 0; i < 25 && leds != nullptr; ++i) leds[i] = color;
    for (uint16_t i = length - 1; i > length-25 - 1 && leds != nullptr; --i) leds[i] = color;
}

void CustomLedStrip::setLeftSegmentColor(CRGB color) {
    for (uint16_t i = 25; i < 79 && leds != nullptr; ++i) leds[i] = color;
}

void CustomLedStrip::setRightSegmentColor(CRGB color) {
    for (uint16_t i = 151; i < length-25 && leds != nullptr; ++i) leds[i] = color;
}
