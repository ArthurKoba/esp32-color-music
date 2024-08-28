#ifndef ESP32_COLOR_MUSIC_APP_H
#define ESP32_COLOR_MUSIC_APP_H

#include <Arduino.h>

#include "configs.h"

//#define NO_LED_FEEDBACK_CODE // remove warning IRRemote: No definition for LED_BUILTIN found -> default LED feedback is disabled.
//#define SEND_PWM_BY_TIMER // remove warning IRRemote: For ESP32, RP2040, mbed and particle boards SEND_PWM_BY_TIMER is enabled by default.
#define FASTLED_INTERNAL
#define FASTLED_ALL_PINS_HARDWARE_SPI
#define FASTLED_ESP32_SPI_BUS VSPI

#include <AsyncInput.h>

#include "CustomBluetoothA2DPSink.h"
#include "ledStrip/customLedStrip.h"
//#include "colorModes/colorModes.h"
//#include "control/remoteControl.h"
#include "colorMusic/colorMusic.h"



class App {
    CustomLedStrip strip;
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
