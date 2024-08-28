#include "table_lighting.h"

uint16_t TableLightingMode::_get_show_delay() const {
    return TABLE_LIGHTING_MODE_DEF_DELAY_MS;
}

void TableLightingMode::_calculate(LedController &controller) {
    controller.fill_leds(CRGB::Black);
    controller.set_color_to_segment(0, CRGB::White);
    controller.set_color_to_segment(1, CRGB::White);
}
