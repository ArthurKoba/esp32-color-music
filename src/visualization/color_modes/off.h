#ifndef ESP32_COLOR_MUSIC_OFF_H
#define ESP32_COLOR_MUSIC_OFF_H


#if not defined(OFF_MODE_DEF_DELAY_MS)
#define OFF_MODE_DEF_DELAY_MS 256
#endif

#include "abs_color_mode.h"

class OffMode final: public AbstractColorMode {
    uint16_t _get_show_delay() const override;
    void _calculate(LedController &controller) override;
};

#endif //ESP32_COLOR_MUSIC_OFF_H
