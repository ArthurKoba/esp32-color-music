#include "configs.h"
#include "imports.h"

CRGB leds[NUM_LEDS];
CustomLedStrip strip;
ColorMusic colorMusic(strip);
ColorModes colorModes(strip, colorMusic);
RemoteControl control;
CustomBluetoothA2DPSink a2dp_sink;
SerialPortInteraction serialPortInteraction;


void setup() {
    Serial.begin(SERIAL_SPEED);
    printf("\n%s Starting...\n", BLUETOOTH_DEVICE_NAME);
    CFastLED::addLeds<WS2812B, WS2812B_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    strip.start(leds, NUM_LEDS);
    strip.setBrightness(100);
    colorModes.setMode(COLOR_MUSIC_MODE);
    colorMusic.setupCallbacks(&a2dp_sink);
    a2dp_sink.set_pin_config(i2sPins);
    a2dp_sink.set_volume(64);
////    a2dp_sink.set_task_core(0);
    a2dp_sink.start(BLUETOOTH_DEVICE_NAME);
    serialPortInteraction.start();
    control.setup();
    control.setSerialPortInteraction(&serialPortInteraction);
    control.setLedStrip((LedStrip*)&strip);
    control.setColorModes(&colorModes);
    control.setColorMusic(&colorMusic);
}

void loop() {
    control.handleCommands();
    colorModes.show();
}


