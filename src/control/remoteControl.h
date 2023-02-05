#ifndef ESP32_COLOR_MUSIC_REMOTECONTROL_H
#define ESP32_COLOR_MUSIC_REMOTECONTROL_H

#include "configs.h"
#include "Arduino.h" // from import uint16_t and another types
#include "colorModes/colorModes.h"


#ifndef IR_RECEIVE_PIN
#error IR_RECEIVE_PIN not defined!
#endif

class RemoteControl {
public:
    RemoteControl();
    void setup();
    void handleCommands();
    void setColorModes(ColorModes *colorModes);
private:
    void handleIR();
    void handleIRCommand(uint16_t command);
    ColorModes *colorModes;
};

#endif //ESP32_COLOR_MUSIC_REMOTECONTROL_H
