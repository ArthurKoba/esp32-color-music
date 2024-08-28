#ifndef ESP32_COLOR_MUSIC_APP_H
#define ESP32_COLOR_MUSIC_APP_H

#include "configs.h"

#include <Arduino.h>

#include <AsyncInput.h>

#include "visualization/led_controller.h"

#include "CustomBluetoothA2DPSink.h"
//#include "colorModes/colorModes.h"
//#include "control/remoteControl.h"
#include "colorMusic/colorMusic.h"



class App {
    LedController led_controller;
//    CustomLedStrip strip;
    ColorMusic colorMusic;
//    ColorModes colorModes(strip, colorMusic);
//RemoteControl control;
    CustomBluetoothA2DPSink a2dp_sink;
    Encoder encoder;

    void show_mode();
public:
    void setup();
    void tick();
};


#endif //ESP32_COLOR_MUSIC_APP_H
