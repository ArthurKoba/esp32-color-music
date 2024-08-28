#ifndef ESP32_COLOR_MUSIC_COLOR_MODES_MANAGER_H
#define ESP32_COLOR_MUSIC_COLOR_MODES_MANAGER_H

#include "color_modes/abs_color_mode.h"
#include "led_controller.h"

enum ColorModes {OFF_MODE, RAINBOW_MODE, NUMBER_OF_COLOR_MODES};

class ColorModesManager final {
    AbstractColorMode* _color_mode_p{};
    ColorModes _mode = OFF_MODE;
public:
    void show_mode(LedController &led_controller);
    void set_mode(ColorModes mode);
};

#endif //ESP32_COLOR_MUSIC_COLOR_MODES_MANAGER_H
