#include "customLedStrip.h"

void CustomLedStrip::setLeftColor(CRGB color) {
    for (uint16_t i = 0; i < lengthLeds / 2 && leds != nullptr; ++i) leds[i] = color;
}

void CustomLedStrip::setRightColor(CRGB color) {
    for (uint16_t i = lengthLeds - 1; i > lengthLeds / 2 - 1 && leds != nullptr; --i) leds[i] = color;
}

void CustomLedStrip::setUpperSegmentColor(CRGB color) {
    for (uint16_t i = 79; i < 151 && leds != nullptr; ++i) leds[i] = color;
}

void CustomLedStrip::setUnderSegmentColor(CRGB color) {
    for (uint16_t i = 0; i < 25 && leds != nullptr; ++i) leds[i] = color;
    for (uint16_t i = lengthLeds - 1; i > lengthLeds - 25 - 1 && leds != nullptr; --i) leds[i] = color;
}

void CustomLedStrip::setLeftSegmentColor(CRGB color) {
    for (uint16_t i = 25; i < 79 && leds != nullptr; ++i) leds[i] = color;
}

void CustomLedStrip::setRightSegmentColor(CRGB color) {
    for (uint16_t i = 151; i < lengthLeds - 25 && leds != nullptr; ++i) leds[i] = color;
}

void CustomLedStrip::flowDown(bool red, bool green, bool blue) {
    CRGB color {};
    for (int i = 1; i < 115; i++) {
        color = leds[i-1];
        if (red)   color.red   = leds[i].red;
        if (green) color.green = leds[i].green;
        if (blue)  color.blue  = leds[i].blue;
        leds[i-1] = color;
    }
    for (int i = 230; i > 114; i--) {
        color = leds[i];
        if (red)   color.red   = leds[i-1].red;
        if (green) color.green = leds[i-1].green;
        if (blue)  color.blue  = leds[i-1].blue;
        leds[i] = color;
    }

}

void CustomLedStrip::setBottomEQ(BottomEQData data) {
    int index = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 9; j++) {
            switch (i) {
                case 0:
                    leds[index].setRGB(data.red_left, 0, 0);
                    leds[229-index].setRGB(data.red_right, 0, 0);
                    break;
                case 1:
                    leds[index].setRGB(0, data.green_left, 0);
                    leds[229-index].setRGB(0, data.green_right, 0);
                    break;
                default:
                    leds[index].setRGB(0, 0, data.blue_left);
                    leds[229-index].setRGB(0, 0, data.blue_right);
                    break;
            }
            index += 1;
        }
    }
}
