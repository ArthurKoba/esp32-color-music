#ifndef ESP32_COLOR_MUSIC_WATERFALL_EQ_H
#define ESP32_COLOR_MUSIC_WATERFALL_EQ_H

#include "abs_color_music.h"
#include "visualization/led_controller.h"

class WaterfallEQColorMusic final: public AbstractColorMusic {
    void _calculate(LedController &controller) override;
public:
    explicit WaterfallEQColorMusic(AudioAnalyzer &analyzer);
};

#endif //ESP32_COLOR_MUSIC_WATERFALL_EQ_H
