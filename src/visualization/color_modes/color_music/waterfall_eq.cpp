#include "waterfall_eq.h"

void WaterfallEQColorMusic::_calculate(LedController &controller) {
    auto info = _analyzer.get_channels_info();

    auto left = _calc_bright_info(info.left);
    auto right = _calc_bright_info(info.right);

    controller.move_colors(114, 0);
    controller.move_colors(115, 229);
    controller.set_color_to_led(115, CRGB(right.low, right.middle, right.high));
    controller.set_color_to_led(114, CRGB(left.low, left.middle, left.high));
    controller.set_color_to_segment(2, CRGB(left.low, 0, 0));
    controller.set_color_to_segment(3, CRGB(0, left.middle, 0));
    controller.set_color_to_segment(4, CRGB(0, 0, left.high));

    controller.set_color_to_segment(5, CRGB(right.low, 0, 0));
    controller.set_color_to_segment(6, CRGB(0, right.middle, 0));
    controller.set_color_to_segment(7, CRGB(0, 0, right.high));
}

WaterfallEQColorMusic::WaterfallEQColorMusic(AudioAnalyzer &analyzer) : AbstractColorMusic(analyzer) {}
