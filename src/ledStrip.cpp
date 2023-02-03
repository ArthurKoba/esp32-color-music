//
// Created by Koba on 03.02.2023.
//

#include "ledStrip.h"

LedStrip::LedStrip(CRGB *leds) {
    this->leds = *leds;
}

