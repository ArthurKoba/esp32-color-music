#include "app.h"

void App::setup() {
    Serial.begin(SERIAL_SPEED);
    printf("\n%s Starting ...\n", BLUETOOTH_DEVICE_NAME);

    led_controller.init();
    led_controller.set_brightness(100);
    color_modes_manager.set_mode(RAINBOW_MODE);
//
//    colorMusic.setupCallbacks(&a2dp_sink);
////    colorMusic.setStrip(&strip);
//    colorMusic.enable();
//////    colorModes.setMode(COLOR_MUSIC_MODE);
//
//    a2dp_sink.set_pin_config({
//        .bck_io_num = gpio_num_t(I2S_BCK_PIN),
//        .ws_io_num = gpio_num_t(I2S_WS_PIN),
//        .data_out_num = gpio_num_t(I2S_DATA_OUT_PIN)
//    });
//    a2dp_sink.set_volume(64);
//    a2dp_sink.start(BLUETOOTH_DEVICE_NAME);
//    control.setup();
//    control.setLedStrip((LedStrip*)&strip);
//    control.setColorModes(&colorModes);
//    AsyncInput::encoder_config_t encoder_cfg;
//    encoder_cfg.clock_pin_cfg.pin = gpio_num_t(ENCODER_CLK_PIN);
//    encoder_cfg.data_pin_cfg.pin = gpio_num_t(ENCODER_DT_PIN);
//    encoder_cfg.max_events = 50;
//    encoder.set_handler([] (AsyncInput::encoder_event_t event, void *context) {
//        auto &app = *reinterpret_cast<App*>(context);
//        int new_volume = app.a2dp_sink.get_volume() + app.encoder.get_counter();
//        new_volume = constrain(new_volume, 0, 127);
////        Serial.println(new_volume);
//        app.encoder.reset_counter();
//        app.a2dp_sink.set_volume(new_volume);
//    }, this);
//
//    encoder.enable(encoder_cfg);

    AsyncInput::button_config_t button_cfg;
    button_cfg.pin_cfg.pin = gpio_num_t(ENCODER_BTN_PIN);
    button_cfg.max_events = 10;

    button.set_handler([] (AsyncInput::button_event_t event, void *context) {
        auto &app = *reinterpret_cast<App*>(context);
        if (event == AsyncInput::CLICK) app.color_modes_manager.next_mode();
    }, this);

    button.enable(button_cfg);

}

void App::tick() {
    color_modes_manager.show_mode(led_controller);
//    encoder.tick();
    button.tick();
//    vTaskDelay(pdMS_TO_TICKS(10));
}
