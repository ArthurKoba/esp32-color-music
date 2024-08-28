#ifndef ESP32_COLOR_MUSIC_COLOR_MODES_CONTROLLER_H
#define ESP32_COLOR_MUSIC_COLOR_MODES_CONTROLLER_H

#include "color_modes/abs_color_mode.h"
#include "led_controller.h"

enum ColorMode {OFF_MODE, RAINBOW_MODE, TABLE_LIGHTING_MODE, NUMBER_OF_COLOR_MODES};

class ColorModesController final {
    AbstractColorMode* _color_mode_p{};
    ColorMode _mode = OFF_MODE;
public:
    void show_mode(LedController &led_controller);
    void set_mode(ColorMode mode);
    void next_mode();
};

#endif //ESP32_COLOR_MUSIC_COLOR_MODES_CONTROLLER_H
