#include "app.h"



void App::setup() {
    Serial.begin(SERIAL_SPEED);
    printf("\n%s Starting ...\n", BLUETOOTH_DEVICE_NAME);

    strip.init(NUM_LEDS);
    strip.setBrightness(100);
//
    colorMusic.setupCallbacks(&a2dp_sink);
    colorMusic.setStrip(&strip);
    colorMusic.enable();
////    colorModes.setMode(COLOR_MUSIC_MODE);

    a2dp_sink.set_pin_config({
        .bck_io_num = gpio_num_t(I2S_BCK_PIN),
        .ws_io_num = gpio_num_t(I2S_WS_PIN),
        .data_out_num = gpio_num_t(I2S_DATA_OUT_PIN)
    });
    a2dp_sink.set_volume(64);
    a2dp_sink.start(BLUETOOTH_DEVICE_NAME);
//    control.setup();
//    control.setLedStrip((LedStrip*)&strip);
//    control.setColorModes(&colorModes);
    AsyncInput::encoder_config_t encoder_cfg;

    encoder_cfg.clock_pin_cfg.pin = gpio_num_t(ENCODER_CLK_PIN);
    encoder_cfg.data_pin_cfg.pin = gpio_num_t(ENCODER_DT_PIN);
    encoder_cfg.max_events = 50;

    encoder.set_handler([] (AsyncInput::encoder_event_t event, void *context) {
        auto &app = *reinterpret_cast<App*>(context);
        int new_volume = app.a2dp_sink.get_volume() + app.encoder.get_counter();
        new_volume = constrain(new_volume, 0, 127);
//        Serial.println(new_volume);
        app.encoder.reset_counter();
        app.a2dp_sink.set_volume(new_volume);
    }, this);

    encoder.enable(encoder_cfg);
}

void App::tick() {
//    control.handleCommands();
//    colorModes.show();
    encoder.tick();
//    vTaskDelay(pdMS_TO_TICKS(10));
}
