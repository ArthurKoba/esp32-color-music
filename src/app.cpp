#include "app.h"

void App::setup() {
    Serial.begin(SERIAL_SPEED);
    printf("\n%s Starting ...\n", BLUETOOTH_DEVICE_NAME);

    led_controller.set_brightness(100);
    led_controller.add_segment({.start = 0, .end = 24});
    led_controller.add_segment({.start = 205, .end = 229});

    color_modes_manager.set_analyzer(&analyzer);
    color_modes_manager.set_led_controller(&led_controller);

    COBS::config_t cobs_config = {.delimiter = '\n', .depth = 255};
    transmitter.set_config(cobs_config, [] (uint8_t *data_p, size_t size, void *context) {
        Serial.write(data_p, size);
        Serial.flush();
    }, nullptr);

    analyzer.setup_callbacks(&a2dp_sink, &transmitter);

    a2dp_sink.set_pin_config({
        .bck_io_num = gpio_num_t(I2S_BCK_PIN),
        .ws_io_num = gpio_num_t(I2S_WS_PIN),
        .data_out_num = gpio_num_t(I2S_DATA_OUT_PIN)
    });
    a2dp_sink.set_volume(64);

    AsyncInput::encoder_config_t encoder_cfg;
    encoder_cfg.clock_pin_cfg.pin = gpio_num_t(ENCODER_CLK_PIN);
    encoder_cfg.data_pin_cfg.pin = gpio_num_t(ENCODER_DT_PIN);
    encoder_cfg.max_events = 50;
    encoder.set_handler([] (AsyncInput::encoder_event_t event, void *context) {
        auto &app = *reinterpret_cast<App*>(context);
        int new_volume = app.a2dp_sink.get_volume() + app.encoder.get_counter();
        new_volume = constrain(new_volume, 0, 127);
        app.encoder.reset_counter();
        app.a2dp_sink.set_volume(new_volume);
    }, this);

    AsyncInput::button_config_t button_cfg;
    button_cfg.pin_cfg.pin = gpio_num_t(ENCODER_BTN_PIN);
    button_cfg.max_events = 10;

    button.set_handler([] (AsyncInput::button_event_t event, void *context) {
        auto &app = *reinterpret_cast<App*>(context);
        if (event == AsyncInput::CLICK) app.color_modes_manager.next_mode();
    }, this);

    led_controller.start();
    encoder.enable(encoder_cfg);
    button.enable(button_cfg);
    a2dp_sink.start(BLUETOOTH_DEVICE_NAME);

    color_modes_manager.set_mode(COLOR_MUSIC_MODE);
}

void App::tick() {
    color_modes_manager.show_mode();
    encoder.tick();
    button.tick();
}
