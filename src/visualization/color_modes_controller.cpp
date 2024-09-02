#include "color_modes_controller.h"
#include "color_modes/rainbow.h"
#include "color_modes/off.h"
#include "color_modes/table_lighting.h"
#include "color_music_mode.h"

void ColorModesController::show_mode() {
    if (not _led_controller) return;
    if (not _color_mode_p) set_mode(_mode);
    if (not _color_mode_p->calculate(*_led_controller)) return;
    if (_mode == OFF_MODE) _led_controller->show_color(CRGB::Black);
    else _led_controller->show();
}

void ColorModesController::set_mode(ColorMode mode) {
    _mode = mode >= NUMBER_OF_COLOR_MODES ? OFF_MODE : mode;
    delete _color_mode_p;
    switch (_mode) {
        case COLOR_MUSIC_MODE:
            if (_analyzer) _color_mode_p = new ColorMusicMode(*_analyzer);
            else _color_mode_p = new OffMode();
            break;
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
    if (_led_controller) _led_controller->fill_leds(CRGB::Black);
}

void ColorModesController::next_mode() {
    set_mode(static_cast<ColorMode>(_mode + 1));
}

void ColorModesController::set_analyzer(AudioAnalyzer *analyzer) {
    _analyzer = analyzer;
}

void ColorModesController::set_led_controller(LedController *led_controller) {
    _led_controller = led_controller;
}
