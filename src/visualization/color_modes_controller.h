#ifndef ESP32_COLOR_MUSIC_COLOR_MODES_CONTROLLER_H
#define ESP32_COLOR_MUSIC_COLOR_MODES_CONTROLLER_H

#include "color_modes/abs_color_mode.h"
#include "led_controller.h"
#include "audio_analysis/audio_analyzer.h"
#include "visualization/color_modes/color_music/waterfall.h"
#include "visualization/color_modes/color_music/waterfall_eq.h"


enum ColorMode {OFF_MODE, CM_WATERFALL_MODE, CM_WATERFALL_EQ_MODE, RAINBOW_MODE, TABLE_LIGHTING_MODE, NUMBER_OF_COLOR_MODES};

class ColorModesController final {
    AbstractColorMode* _color_mode_p{};
    ColorMode _mode = OFF_MODE;
    AudioAnalyzer *_analyzer = nullptr;
    LedController *_led_controller = nullptr;
public:
    void show_mode();
    void set_mode(ColorMode mode);
    void next_mode();

    void set_analyzer(AudioAnalyzer *analyzer);
    void set_led_controller(LedController *led_controller);
};

#endif //ESP32_COLOR_MUSIC_COLOR_MODES_CONTROLLER_H
