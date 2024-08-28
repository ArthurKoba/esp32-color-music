#include "color_modes_controller.h"
#include "color_modes/rainbow.h"
#include "color_modes/off.h"
#include "color_modes/table_lighting.h"

void ColorModesController::show_mode(LedController &led_controller) {
    if (not _color_mode_p) set_mode(_mode);
    if (not _color_mode_p->calculate(led_controller)) return;
    if (_mode == OFF_MODE) led_controller.show_color(CRGB::Black);
    else led_controller.show();
}

void ColorModesController::set_mode(ColorMode mode) {
    _mode = mode >= NUMBER_OF_COLOR_MODES ? OFF_MODE : mode;
    delete _color_mode_p;
    switch (_mode) {
        case RAINBOW_MODE:
            _color_mode_p = new RainbowMode();
            break;
        case TABLE_LIGHTING_MODE:
            _color_mode_p = new TableLightingMode();
            break;
        case OFF_MODE:
            _color_mode_p = new OffMode();
        case NUMBER_OF_COLOR_MODES:
            break;
    }
}

void ColorModesController::next_mode() {
    set_mode(static_cast<ColorMode>(_mode + 1));
}