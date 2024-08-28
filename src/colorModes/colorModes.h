#ifndef ESP32_COLOR_MUSIC_COLORMODES_H
#define ESP32_COLOR_MUSIC_COLORMODES_H

#include "colorMusic/colorMusic.h"

#ifndef RAINBOW_MODE_DELAY_MS
#define RAINBOW_MODE_DELAY_MS 10
#endif

enum ColorMode : uint8_t {
    OFF_MODE, COLOR_MUSIC_MODE, RAINBOW_MODE
};

struct ModesDelay {
    uint8_t rainbow = RAINBOW_MODE_DELAY_MS;
};

class ColorModes {
public:

    void setMode(ColorMode mode);
    void show();
protected:
    void rainbowMode();
    ModesDelay delays;
    ColorMode mode = OFF_MODE;
    ColorMusic &colorMusic;
};

#endif //ESP32_COLOR_MUSIC_COLORMODES_H
