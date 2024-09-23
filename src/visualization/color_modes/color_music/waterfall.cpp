#include "waterfall.h"

void WaterfallColorMusic::_calculate(LedController &controller) {
    auto info = _analyzer.get_channels_info();

    auto left = _calc_bright_info(info.left);
    auto right = _calc_bright_info(info.right);

    controller.move_colors(114, 0);
    controller.move_colors(115, 229);
    controller.set_color_to_led(115, CRGB(right.low, right.middle, right.high));
    controller.set_color_to_led(114, CRGB(left.low, left.middle, left.high));
}

WaterfallColorMusic::WaterfallColorMusic(AudioAnalyzer &analyzer) : AbstractColorMusic(analyzer) {}
