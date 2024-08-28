#ifndef ESP32_COLOR_MUSIC_RAINBOW_H
#define ESP32_COLOR_MUSIC_RAINBOW_H


#if not defined(RAINBOW_MODE_DEF_DELAY_MS)
#define RAINBOW_MODE_DEF_DELAY_MS 30
#endif

#if not defined(RAINBOW_MODE_MIN_DELAY_MS)
#define RAINBOW_MODE_MIN_DELAY_MS 1
#endif

#if not defined(RAINBOW_MODE_MAX_DELAY_MS)
#define RAINBOW_MODE_MAX_DELAY_MS 50
#endif

#if not defined(RAINBOW_MODE_HUE_MUL)
#define RAINBOW_MODE_HUE_MUL 1
#endif

#include "abs_color_mode.h"

class RainbowMode final: public AbstractColorMode {
    uint16_t _get_show_delay() const override;
    void _calculate(LedController &controller) override;
};

#endif //ESP32_COLOR_MUSIC_RAINBOW_H
