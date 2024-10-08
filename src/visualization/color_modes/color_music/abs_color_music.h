#ifndef ESP32_COLOR_MUSIC_ABS_COLOR_MUSIC_H
#define ESP32_COLOR_MUSIC_ABS_COLOR_MUSIC_H

#if not defined(COLOR_MUSIC_MODE_DEF_DELAY_MS)
#define COLOR_MUSIC_MODE_DEF_DELAY_MS 10
#endif

#include <cstdint>
#include "visualization/color_modes/abs_color_mode.h"
#include "audio_analysis/audio_analyzer.h"

struct BrightInfo {
    uint8_t low;
    uint8_t middle;
    uint8_t high;
};

class AbstractColorMusic: public AbstractColorMode {
protected:
    AudioAnalyzer &_analyzer;
    uint16_t _get_show_delay() const override;
    static BrightInfo _calc_bright_info(AudioInformation &info);
public:
    explicit AbstractColorMusic(AudioAnalyzer &analyzer);
};


#endif //ESP32_COLOR_MUSIC_ABS_COLOR_MUSIC_H
