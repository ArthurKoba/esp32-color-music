#ifndef ESP32_COLOR_MUSIC_APP_H
#define ESP32_COLOR_MUSIC_APP_H

#include "configs.h"

#include <Arduino.h>
#include <AsyncInput.h>
#include <BDSP.h>

#include "audio_analysis/audio_analyzer.h"
#include "visualization/led_controller.h"
#include "visualization/color_modes_controller.h"
#include "CustomBluetoothA2DPSink.h"

class App {
    LedController led_controller;
    ColorModesController color_modes_manager;
    AudioAnalyzer analyzer;
    CustomBluetoothA2DPSink a2dp_sink;
    Encoder encoder;
    Button button;
public:
    void setup();
    void tick();
};


#endif //ESP32_COLOR_MUSIC_APP_H
