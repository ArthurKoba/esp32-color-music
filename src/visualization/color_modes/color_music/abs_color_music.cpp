#include "abs_color_music.h"


uint16_t AbstractColorMusic::_get_show_delay() const {
    return COLOR_MUSIC_MODE_DEF_DELAY_MS;
}

AbstractColorMusic::AbstractColorMusic(AudioAnalyzer &analyzer) : _analyzer(analyzer) {}

BrightInfo AbstractColorMusic::_calc_bright_info(AudioInformation &info) {
    BrightInfo bright_info{};

    bright_info.low = LedController::crt(info.low);
    bright_info.middle = LedController::crt(info.middle);

    bright_info.high = static_cast<uint16_t>(info.high) * 2;

    if (bright_info.low > bright_info.middle and bright_info.low > bright_info.high) {
        bright_info.high *= 0.3;
        bright_info.middle *= 0.3;
    }

    if (bright_info.middle > bright_info.low and bright_info.middle > bright_info.high) {
        bright_info.low *= 0.3;
        bright_info.high *= 0.3;
    }

    if (bright_info.high > bright_info.low and bright_info.high > bright_info.middle) {
        bright_info.low *= 0.3;
        bright_info.middle *= 0.3;
    }
    return bright_info;
}