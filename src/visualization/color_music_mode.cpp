#include "color_music_mode.h"

uint16_t ColorMusicMode::_get_show_delay() const {
    return COLOR_MUSIC_MODE_DEF_DELAY_MS;
}

ColorMusicMode::ColorMusicMode(AudioAnalyzer &analyzer) : _analyzer(analyzer) {}


void ColorMusicMode::_calculate(LedController &controller) {
//    uint32_t time = millis();
//    bool needUpdateRed = false;
//    bool needUpdateGreen = false;
//    bool needUpdateBlue = false;
//
//    if (time - this->lastTimeRed > 14) {
//        needUpdateRed = true;
//        this->lastTimeRed = time;
//    }
//
//    if (time - this->lastTimeGreen > 11) {
//        needUpdateGreen = true;
//        this->lastTimeGreen = time;
//    }
//
//    if (time - this->lastTimeBlue > 5) {
//        needUpdateBlue = true;
//        this->lastTimeBlue = time;
//    }
//
//    ChannelBright leftChannelBright = calculateBrightFromChannel(fft->amplitudes.left);
//    ChannelBright rightChannelBright = calculateBrightFromChannel(fft->amplitudes.right);
    static uint8_t index = 0;

    controller.move_colors(114, 0);
    controller.move_colors(115, 229);
    controller.set_color_to_led(115, CHSV(index, 255, 255));
    controller.set_color_to_led(114, CHSV(index, 255, 255));

    index++;

//    strip->flowDown(needUpdateRed, needUpdateGreen, needUpdateBlue);
//

//
//    BottomEQData data;
//    data.red_left=leftChannelBright.low;
//    data.green_left=leftChannelBright.middle;
//    data.blue_left=leftChannelBright.high;
//    data.red_right=rightChannelBright.low;
//    data.green_right=rightChannelBright.middle;
//    data.blue_right=rightChannelBright.high;
//    strip->setBottomEQ(data);
}

//ChannelBright ColorMusic::calculateBrightFromChannel(const float *channel) const {
//    ChannelBright bright;
//    auto startLowIndex = (uint16_t) (340/fftConfig.frequencyStep);
//    auto endLowIndex = (uint16_t) (400/fftConfig.frequencyStep);
//    auto startMiddleIndex = (uint16_t) (600/fftConfig.frequencyStep);
//    auto endMiddleIndex = (uint16_t) (1000/fftConfig.frequencyStep);
//    auto startHighIndex = (uint16_t) (10000/fftConfig.frequencyStep);
//    auto endHighIndex = (uint16_t) (20000/fftConfig.frequencyStep);
//
//
//    uint32_t sum = 0;
//
////    for (auto i = startLowIndex; i < endLowIndex; ++i) {
////        uint8_t targetLow = (uint16_t) channel[i] >> 6;
////        if (targetLow > bright.low) bright.low = targetLow;
////    }
//
//    bright.low = (uint16_t) channel[6] >> 6;
//
//    sum = 0;
//    for (auto i = startMiddleIndex; i < endMiddleIndex; ++i) {
//        sum += (uint16_t) channel[i] >> 6;
//    }
//    bright.middle = sum/(endMiddleIndex - startMiddleIndex);
//
//    sum = 0;
//    for (auto i = startHighIndex; i < endHighIndex; ++i) {
//        sum += (uint16_t) channel[i] >> 6;
//    }
//    bright.high = sum/(endHighIndex - startHighIndex);
//
//
////    bright.low = getBrightCRT(bright.low);
//    bright.low = bright.low > 0 ? (1 + (uint16_t)(bright.low * bright.low + 255)) >> 8 : 0;
//
//    // test crt correction for dynamic green leds
//    bright.middle = bright.middle > 0 ? (1 + (uint16_t)(bright.middle * bright.middle + 255)) >> 8 : 0;
//    bright.high = ((uint16_t)bright.high) * 2;
//
//    if (bright.low > bright.middle && bright.low > bright.high) {
//        bright.high *= 0.3;
//        bright.middle *= 0.3;
//    }
//
//    if (bright.middle > bright.low && bright.middle > bright.high) {
//        bright.low *= 0.3;
//        bright.high *= 0.3;
//    }
//
//    if (bright.high > bright.low && bright.high > bright.middle) {
//        bright.low *= 0.3;
//        bright.middle *= 0.3;
//    }
//
//    return bright;
//}