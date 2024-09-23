#ifndef ESP32_COLOR_MUSIC_ABS_COLOR_MODE_H
#define ESP32_COLOR_MUSIC_ABS_COLOR_MODE_H

#include <cstdint>
#include "visualization/led_controller.h"


class AbstractColorMode {
protected:
    uint32_t last_update = 0;
//    EEPROMMemory &mem;
    virtual void _calculate(LedController &controller) = 0;
    virtual uint16_t _get_show_delay() const = 0;
public:
//    explicit AbstractColorMode(EEPROMMemory &mem) : mem(mem) {};
    virtual ~AbstractColorMode() = default;
    virtual bool calculate(LedController &controller);
};

#endif //ESP32_COLOR_MUSIC_ABS_COLOR_MODE_H
