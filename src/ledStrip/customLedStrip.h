#ifndef ESP32_COLOR_MUSIC_CUSTOMLEDSTRIP_H
#define ESP32_COLOR_MUSIC_CUSTOMLEDSTRIP_H

#include "ledStrip.h"
#include <FastLED.h>

class CustomLedStrip : public LedStrip {

public:
    void setLeftColor(CRGB color);
    void setRightColor(CRGB color);
    void setUpperSegmentColor(CRGB color);
    void setUnderSegmentColor(CRGB color);
    void setLeftSegmentColor(CRGB color);
    void setRightSegmentColor(CRGB color);
};

#endif //ESP32_COLOR_MUSIC_CUSTOMLEDSTRIP_H
