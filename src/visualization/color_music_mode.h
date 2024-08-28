#ifndef ESP32_COLOR_MUSIC_COLOR_MUSIC_MODE_H
#define ESP32_COLOR_MUSIC_COLOR_MUSIC_MODE_H

#if not defined(COLOR_MUSIC_MODE_DEF_DELAY_MS)
#define COLOR_MUSIC_MODE_DEF_DELAY_MS 15
#endif

#include "color_modes/abs_color_mode.h"
#include "../audio_analysis/audio_analyzer.h"

class ColorMusicMode final: public AbstractColorMode {
    uint16_t _get_show_delay() const override;
    void _calculate(LedController &controller) override;
    AudioAnalyzer &_analyzer;
public:
    explicit ColorMusicMode(AudioAnalyzer &analyzer);
};


#endif //ESP32_COLOR_MUSIC_COLOR_MUSIC_MODE_H
