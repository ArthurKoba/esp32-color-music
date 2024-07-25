#ifndef ESP32_COLOR_MUSIC_REMOTECONTROL_H
#define ESP32_COLOR_MUSIC_REMOTECONTROL_H

#include "configs.h"
#include "Arduino.h" // from import uint16_t and another types
#include "colorModes/colorModes.h"
#include "ledStrip/ledStrip.h"


#ifndef IR_RECEIVE_PIN
#error IR_RECEIVE_PIN not defined!
#endif

enum IRButton : uint8_t {
    BRIGHT_UP_BUTTON, BRIGHT_DOWN_BUTTON, OFF_BUTTON, ON_BUTTON,
    RED_BUTTON, GREEN_BUTTON, BLUE_BUTTON, WHITE_BUTTON,
    ORANGERED_BUTTON, LIME_BUTTON, VIOLET_BUTTON, FLASH_BUTTON,
    ORANGE_BUTTON, CYAN_BUTTON, PURPLE_BUTTON, STROBE_BUTTON,
    GOLD_BUTTON, GREENCYAN_BUTTON, ORCHID_BUTTON, FADE_BUTTON,
    YELLOW_BUTTON, NAVY_BUTTON, PINK_BUTTON, SMOOTH_BUTTON
};

class RemoteControl {
public:
    RemoteControl();
    void setup();
    void setColorModes(ColorModes *colorModesPointer);
    void setLedStrip(LedStrip *stripPointer);
    void setColorMusic(ColorMusic *colorMusicPointer);
    void handleCommands();
private:
    void handleIR();
    void handleIRCommand(IRButton command);
    void handleTestIRCommand(IRButton command);
    ColorMusic *colorMusic;
    ColorModes *colorModes;
    LedStrip *strip;
};

#endif //ESP32_COLOR_MUSIC_REMOTECONTROL_H
