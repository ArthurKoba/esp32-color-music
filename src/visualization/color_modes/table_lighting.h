#ifndef ESP32_COLOR_MUSIC_TABLE_LIGHTING_H
#define ESP32_COLOR_MUSIC_TABLE_LIGHTING_H


#if not defined(TABLE_LIGHTING_MODE_DEF_DELAY_MS)
#define TABLE_LIGHTING_MODE_DEF_DELAY_MS 255
#endif

#include "abs_color_mode.h"

class TableLightingMode final: public AbstractColorMode {
    uint16_t _get_show_delay() const override;
    void _calculate(LedController &controller) override;
};

#endif //ESP32_COLOR_MUSIC_TABLE_LIGHTING_H
