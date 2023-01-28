#ifndef IR_RECEIVE_PIN
    #error IR_RECEIVE_PIN not defined!
#endif

#ifndef BRIGHT_STEP
    #define BRIGHT_STEP 5
#endif


enum IRButtons { // Перечисление кнопок пульта.
    BRIGHT_UP_BUTTON, BRIGHT_DOWN_BUTTON, OFF_BUTTON, ON_BUTTON,
    RED_BUTTON, GREEN_BUTTON, BLUE_BUTTON, WHITE_BUTTON,
    ORANGERED_BUTTON, LIME_BUTTON, VIOLET_BUTTON, FLASH_BUTTON,
    ORANGE_BUTTON, CYAN_BUTTON, PURPLE_BUTTON, STROBE_BUTTON,
    GOLD_BUTTON, GREENCYAN_BUTTON, ORCHID_BUTTON, FADE_BUTTON,
    YELLOW_BUTTON, NAVY_BUTTON, PINK_BUTTON, SMOOTH_BUTTON
};

void setColor(uint8_t ledId)
// Установка цветов согласно схеме пульта.
// Во многом сделано на основе цветов с сайта https://encycolorpedia.ru/
{
    switch (ledId) {
        case RED_BUTTON:        FastLED.showColor(CRGB(255,   0,   0)); break;
        case GREEN_BUTTON:      FastLED.showColor(CRGB(  0, 255,   0)); break;
        case BLUE_BUTTON:       FastLED.showColor(CRGB(  0,   0, 255)); break;
        case WHITE_BUTTON:      FastLED.showColor(CRGB(255, 255, 255)); break;
        case OFF_MODE:          FastLED.showColor(CRGB(  0,   0,   0)); break;

        case ORANGERED_BUTTON:  FastLED.showColor(CRGB(255,  50,   0)); break;
        case LIME_BUTTON:       FastLED.showColor(CRGB(100, 255,   0)); break;
        case VIOLET_BUTTON:     FastLED.showColor(CRGB( 21,  96, 189)); break;

        case ORANGE_BUTTON:     FastLED.showColor(CRGB(255,  80,   0)); break;
        case CYAN_BUTTON:       FastLED.showColor(CRGB( 44, 183, 185)); break;
        case PURPLE_BUTTON:     FastLED.showColor(CRGB(128,   0, 128)); break;

        case GOLD_BUTTON:       FastLED.showColor(CRGB(205, 100,   0)); break;
        case GREENCYAN_BUTTON:  FastLED.showColor(CRGB(  0, 170,  60)); break;
        case ORCHID_BUTTON:     FastLED.showColor(CRGB(211,   3, 158)); break;

        case YELLOW_BUTTON:     FastLED.showColor(CRGB(255, 150,   0)); break;
        case NAVY_BUTTON:       FastLED.showColor(CRGB( 100,  0, 141)); break;
        case PINK_BUTTON:       FastLED.showColor(CRGB(211,  40, 158)); break;
        default:                                                                       break;
    }
}

void execIrCommand (ColorModes &colorModes) {
    if (!IrReceiver.decode()) return;
    uint16_t command = IrReceiver.decodedIRData.command;

    if (command == BRIGHT_UP_BUTTON || command == BRIGHT_DOWN_BUTTON) {
        colorModes.bright += command == BRIGHT_UP_BUTTON ? BRIGHT_STEP : -BRIGHT_STEP;
        colorModes.bright = constrain(colorModes.bright, 1, 255);
        FastLED.setBrightness(colorModes.bright);
        if (colorModes.mode == COLOR_MODE) setColor(colorModes.colorID);
    }

    switch (command) {
        case ON_BUTTON:     colorModes.mode = COLOR_MUSIC_MODE; break;
        case FLASH_BUTTON:  colorModes.mode = FLASH_MODE;       break;
        case STROBE_BUTTON: colorModes.mode = STROBE_MODE;      break;
        case FADE_BUTTON:   colorModes.mode = FADE_MODE;        break;
        case SMOOTH_BUTTON: colorModes.mode = SMOOTH_MODE;      break;
        case BRIGHT_UP_BUTTON: case BRIGHT_DOWN_BUTTON:         break;
        default:
            colorModes.mode = COLOR_MODE;
            setColor(command);
            colorModes.colorID = command;
    }
    IrReceiver.resume();
}