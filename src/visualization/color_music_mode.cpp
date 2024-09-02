#include "color_music_mode.h"

uint16_t ColorMusicMode::_get_show_delay() const {
    return COLOR_MUSIC_MODE_DEF_DELAY_MS;
}

ColorMusicMode::ColorMusicMode(AudioAnalyzer &analyzer) : _analyzer(analyzer) {}

void ColorMusicMode::_calculate(LedController &controller) {
    auto info = _analyzer.get_channels_info();

    auto left = _calc_bright_info(info.left);
    auto right = _calc_bright_info(info.right);

    controller.move_colors(114, 0);
    controller.move_colors(115, 229);
    controller.set_color_to_led(115, CRGB(right.low, right.middle, right.high));
    controller.set_color_to_led(114, CRGB(left.low, left.middle, left.high));
}

BrightInfo ColorMusicMode::_calc_bright_info(AudioInformation &info) {
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
