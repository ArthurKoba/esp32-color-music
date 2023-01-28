#ifndef WS2812B_PIN
    #error WS2812B_PIN required!
#endif

#define NUM_LEDS 230

#include <FastLED.h>

CRGB leds[NUM_LEDS];

void setupFastLED() {
    CFastLED::addLeds <WS2812, WS2812B_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.showColor(CRGB::Black);
}