#if not defined(TEST_ENTRY)
#include "app.h"

App app;

void setup() {
    app.setup();
}

void loop() {
//    vTaskDelete(nullptr);
    app.tick();
}
#else
#include "configs.h"
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

#define LED_PIN 2

#include "spdif.h"

SPDIFOutput spdif;
BluetoothA2DPSink a2dp_sink(spdif);

void setup() {

    Serial.begin(SERIAL_SPEED);
    Serial.println("\nStart Test Entry!");


    pinMode(LED_PIN, OUTPUT);
    SPDIFConfigs configs;
    configs.pin = gpio_num_t(23);
    spdif.set_configs(configs);

    a2dp_sink.set_volume(64);
    a2dp_sink.start("MyMusic");
}

void loop() {
    digitalWrite(LED_PIN, not digitalRead(LED_PIN));
    delay(1000);
}
#endif

