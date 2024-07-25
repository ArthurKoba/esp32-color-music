#include "configs.h"

#include <Arduino.h>
#include <AsyncInput.h>
#include "audioReceiver.h"
#include "ledStrip/customLedStrip.h"
//#include "colorModes/colorModes.h"
//#include "control/remoteControl.h"
#include "colorMusic/colorMusic.h"


struct Context {
    CustomLedStrip strip;
    ColorMusic colorMusic;
//ColorModes colorModes(strip, colorMusic);
//RemoteControl control;
    CustomBluetoothA2DPSink a2dp_sink;
    Encoder encoder;
} ctx;



void setup() {
    Serial.begin(SERIAL_SPEED);
    printf("\n%s Starting ...\n", BLUETOOTH_DEVICE_NAME);

    ctx.strip.init(NUM_LEDS);
    ctx.strip.setBrightness(100);
//
    ctx.colorMusic.setupCallbacks(&ctx.a2dp_sink);
    ctx.colorMusic.setStrip(&ctx.strip);
    ctx.colorMusic.enable();
////    colorModes.setMode(COLOR_MUSIC_MODE);
    ctx.a2dp_sink.set_pin_config(i2sPins);
    ctx.a2dp_sink.set_volume(64);
    ctx.a2dp_sink.start(BLUETOOTH_DEVICE_NAME);
//    control.setup();
//    control.setLedStrip((LedStrip*)&strip);
//    control.setColorModes(&colorModes);
    AsyncInput::encoder_config_t encoder_cfg;

    encoder_cfg.clock_pin_cfg.pin = gpio_num_t(ENCODER_CLK_PIN);
    encoder_cfg.data_pin_cfg.pin = gpio_num_t(ENCODER_DT_PIN);
    encoder_cfg.max_events = 50;

    ctx.encoder.set_handler([] (AsyncInput::encoder_event_t event, void *context) {
        Context &ctx = *reinterpret_cast<Context*>(context);
        int new_volume = ctx.a2dp_sink.get_volume() + ctx.encoder.get_counter();
        new_volume = constrain(new_volume, 0, 127);
//        Serial.println(new_volume);
        ctx.encoder.reset_counter();
        ctx.a2dp_sink.set_volume(new_volume);
    }, &ctx);

    if (ctx.encoder.enable(encoder_cfg)) {
        Serial.println("ENC started");
    }
}

void loop() {
//    vTaskDelete(nullptr);
//    control.handleCommands();
//    colorModes.show();
    ctx.encoder.tick();
    vTaskDelay(pdMS_TO_TICKS(10));
}


