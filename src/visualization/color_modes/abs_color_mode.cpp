#include "abs_color_mode.h"

bool AbstractColorMode::calculate(LedController &controller)  {
    if (millis() - last_update < _get_show_delay()) return false;
    last_update = millis();
    _calculate(controller);
    return true;
}
