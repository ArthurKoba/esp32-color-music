#ifndef ESP32_COLOR_MUSIC_WATERFALL_H
#define ESP32_COLOR_MUSIC_WATERFALL_H

#include "abs_color_music.h"
#include "visualization/led_controller.h"

class WaterfallColorMusic final: public AbstractColorMusic {
    void _calculate(LedController &controller) override;
public:
    explicit WaterfallColorMusic(AudioAnalyzer &analyzer);
};

#endif //ESP32_COLOR_MUSIC_WATERFALL_H