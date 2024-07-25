#include "configs.h"

#include "ledStrip/customLedStrip.h"
#include "colorModes/colorModes.h"
#include "control/remoteControl.h"
#include "control/display.h"
#include "colorMusic/colorMusic.h"
#include "audioReceiver.h"
#include "control/SystemManager.h"


SystemManager manager;
CustomLedStrip strip;
Display display;
ColorMusic colorMusic;
//ColorModes colorModes(strip, colorMusic);
//RemoteControl control;
CustomBluetoothA2DPSink a2dp_sink;


void setup() {
    Serial.begin(SERIAL_SPEED);
    printf("\n%s Starting...\n", BLUETOOTH_DEVICE_NAME);
    strip.init(NUM_LEDS);
    manager.init();
//    display.setAmplitudes(colorMusic.amplitudes, AMPLITUDES_SIZE);
//    display.initAndStart();
//    display.setSystemManager(&manager);
    strip.setBrightness(100);
    colorMusic.setupCallbacks(&a2dp_sink);
    colorMusic.setStrip(&strip);
    colorMusic.enable();
//    colorModes.setMode(COLOR_MUSIC_MODE);
    a2dp_sink.set_pin_config(i2sPins);
    a2dp_sink.set_volume(64);
    a2dp_sink.start(BLUETOOTH_DEVICE_NAME);
//    control.setup();
//    control.setLedStrip((LedStrip*)&strip);
//    control.setColorModes(&colorModes);
}

void loop() {
    vTaskDelete(nullptr);
//    control.handleCommands();
//    colorModes.show();
//    vTaskDelay(pdMS_TO_TICKS(10));
}


