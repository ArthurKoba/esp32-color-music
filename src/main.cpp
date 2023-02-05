#include "configs.h"
#include "imports.h"
#include "testIRexecutor.h"

CRGB leds[NUM_LEDS];
CustomLedStrip strip;
ColorModes colorModes(strip);

//BluetoothA2DPSink a2dp_sink;
//FFTColorMusic colorMusic(leds);

//void callbackChangeConnectionState(esp_a2d_connection_state_t state, void *obj) {
//    Serial.println("Change connection state: " + String(state));
//    if (state == ESP_A2D_CONNECTION_STATE_CONNECTED) {
//        Serial.print("Connected host: ");
//        Serial.print(a2dp_sink.get_connected_source_name());
//        Serial.println();
//    }
//}
//
//void callbackAVRCMetadata(uint8_t id, const uint8_t *text) {
////    Serial.print("AVRCMetadata: " + String(id));
//    printf("AVRCMetadata: id 0x%x, %s\n", id, text);
//////    Serial.write(text, strlen((char*)&text));
////    Serial.write(text, strlen((char*)&text));
////    Serial.println();
//    for (auto & led : leds) led = CRGB::Black;
//    colorMusic.samplesFullness = 0;
//}
//
//void callbackOnChangeAudioState(esp_a2d_audio_state_t state, void *obj) {
//    Serial.println("Change audio state!");
//    FastLED.showColor(CRGB::Black);
//}


void setup() {
    Serial.begin(SERIAL_SPEED);
    CFastLED::addLeds<WS2812B, WS2812B_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    strip.start(leds, NUM_LEDS);
    strip.setBrightness(100);
    colorModes.setMode(RAINBOW_MODE);

//    a2dp_sink.set_pin_config(i2sPins);
//    a2dp_sink.set_raw_stream_reader([] (const uint8_t *d, uint32_t l) {colorMusic.addSamples(d, l);});
//    a2dp_sink.set_sample_rate_callback([] (uint16_t rate) {colorMusic.setSampleRate(rate);});
//    a2dp_sink.set_volume(127);
//    a2dp_sink.set_on_volumechange(change_volume);
//    a2dp_sink.set_on_audio_state_changed_post(callbackOnChangeAudioState);
//    a2dp_sink.set_on_connection_state_changed(callbackChangeConnectionState);
//    a2dp_sink.set_avrc_metadata_callback(callbackAVRCMetadata);
////    a2dp_sink.set_task_core(0);
//    a2dp_sink.start(BLUETOOTH_DEVICE_NAME);
//    setupEncoder();
//    IrReceiver.begin(IR_RECEIVE_PIN);
    Serial.print('\n');
    Serial.print(BLUETOOTH_DEVICE_NAME);
    Serial.println(" Started!");
}

void loop() {
    colorModes.show();
//    execEncoder(a2dp_sink);
//    execIrCommandTest(colorMusic);
}


