#include "color_modes_manager.h"
#include "color_modes/rainbow.h"
#include "color_modes/off.h"


void ColorModesManager::show_mode(LedController &led_controller) {
    if (not _color_mode_p) set_mode(_mode);
    if (_color_mode_p->calculate(led_controller)) led_controller.show();
}


void ColorModesManager::set_mode(ColorModes mode) {
    _mode = mode >= NUMBER_OF_COLOR_MODES ? OFF_MODE : mode;
    delete _color_mode_p;
    switch (_mode) {
        case RAINBOW_MODE:
            _color_mode_p = new RainbowMode();
            break;
        case OFF_MODE:
            _color_mode_p = new OffMode();
        case NUMBER_OF_COLOR_MODES:
            break;
    }
}


