#ifndef ESP32_COLOR_MUSIC_APP_H
#define ESP32_COLOR_MUSIC_APP_H

#include "configs.h"

#include <Arduino.h>

#include <AsyncInput.h>

#include "visualization/led_controller.h"
#include "visualization/color_modes_manager.h"

#include "CustomBluetoothA2DPSink.h"
//#include "colorModes/colorModes.h"
//#include "control/remoteControl.h"
#include "colorMusic/colorMusic.h"



class App {

    LedController led_controller;
    ColorModesManager color_modes_manager;
//    CustomLedStrip strip;
    ColorMusic colorMusic;
//    ColorModes colorModes(strip, colorMusic);
//RemoteControl control;
    CustomBluetoothA2DPSink a2dp_sink;
    Encoder encoder;


public:
    void setup();
    void tick();
};


#endif //ESP32_COLOR_MUSIC_APP_H
