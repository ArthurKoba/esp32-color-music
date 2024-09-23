#include "rainbow.h"

uint16_t RainbowMode::_get_show_delay() const {
    return RAINBOW_MODE_DEF_DELAY_MS;
}

void RainbowMode::_calculate(LedController &controller) {

    static uint8_t current_index = 0;

    for (size_t i = 0; i < controller.number_of_leds(); ++i) {
        controller.set_color_to_led(i, CHSV(current_index + i * RAINBOW_MODE_HUE_MUL, 255, 255));
    }

    current_index++;
}
