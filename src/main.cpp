#include "configs.h"
#include "imports.h"

CRGB leds[NUM_LEDS];
ColorModes colorModes;
BluetoothA2DPSink a2dp_sink;

uint16_t timeFullness = 0;

void callbackChangeConnectionState(esp_a2d_connection_state_t state, void *obj) {
    Serial.println("Change connection state: " + String(state));
    if (state == ESP_A2D_CONNECTION_STATE_CONNECTED) {
        Serial.print("Connected host: ");
        Serial.print(a2dp_sink.get_connected_source_name());
        Serial.println();
    }

}

void callbackAVRCMetadata(uint8_t param, const uint8_t *text, int length) {
    Serial.print("AVRCMetadata: " + String(param) + " ");
    Serial.write(text, length);
    Serial.println();
    for (auto & led : leds) led = CRGB::Black;
    fftData.samples.fullness = 0;
    timeFullness = millis();
}

void callbackOnChangeAudioState(esp_a2d_audio_state_t state, void *obj) {
    Serial.println("Change audio state!");
    FastLED.showColor(CRGB::Black);
}

void callbackSampleRate(uint16_t sampleRate) {
    Serial.println("Sample rate: " + String(sampleRate));
}

void setup() {
    Serial.begin(SERIAL_SPEED);
    setupColorModes(colorModes);
    CFastLED::addLeds<WS2812B, WS2812B_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setMaxRefreshRate(0, false);
    FastLED.setBrightness(colorModes.bright);
    FastLED.showColor(CRGB::Black);
    a2dp_sink.set_pin_config(i2sPins);
    a2dp_sink.set_raw_stream_reader(appendSamples);
    a2dp_sink.set_on_volumechange(change_volume);
    a2dp_sink.start(BLUETOOTH_DEVICE_NAME);
    a2dp_sink.set_volume(127);
    a2dp_sink.set_on_connection_state_changed(callbackChangeConnectionState);
    a2dp_sink.set_on_audio_state_changed_post(callbackOnChangeAudioState);
    a2dp_sink.set_sample_rate_callback(callbackSampleRate);
    a2dp_sink.set_avrc_metadata_callback(callbackAVRCMetadata);
//    a2dp_sink.set_task_core(0);
    setupEncoder();
    IrReceiver.begin(IR_RECEIVE_PIN);
    setupColorMusic(fftData);
    Serial.print('\n');
    Serial.print(BLUETOOTH_DEVICE_NAME);
    Serial.println(" Started!");
}

enum SendType : uint8_t {
    AMPLITUDES_AREA, AMPLITUDES_FULL, WINDOW, BARK_SCALE, NO_SEND
};

uint8_t fftArea = 0;
uint8_t fastAmplitudes[AMPLITUDES_SIZE];
uint8_t sendType = AMPLITUDES_AREA;

uint32_t timeFullness = 0;
bool useDivision = false;
int16_t info[5];

#include "colorMusic.h"
#include "testIRexecutor.h"


void colorMusic() {
    if (fftData.samples.fullness != SAMPLES_SIZE) return;
    info[4] = (int16_t) (millis() - timeFullness);
    timeFullness = millis();
//    sendJsonArray(fftData.samples.left, 256, "samples");

    uint16_t calcTime = millis();
    calculateAmplitudes(fftData.samples.right, fftData.amplitudes.right);
    calculateAmplitudes(fftData.samples.left, fftData.amplitudes.left);
    info[0] = (int16_t) (millis() - calcTime);
//    Serial.println("Calc time (ms): " + String(info[0]));

    for (int i = 0; i < AMPLITUDES_SIZE; ++i) {
        fastAmplitudes[i] = (int16_t) fftData.amplitudes.left[i] >> 6;
        if (useDivision) fastAmplitudes[i] >>=3;
    }
    fftData.samples.fullness = 0;

    calcTime = millis();
    calculateColors(fastAmplitudes);
    info[1] = (int16_t) (millis() - calcTime);

    calcTime = millis();
    switch (sendType) {
        case AMPLITUDES_AREA:
            sendAmplitudesArea(fastAmplitudes, AMPLITUDES_SIZE, "fft", fftArea);
            break;
        case AMPLITUDES_FULL:
            sendJsonArray(fastAmplitudes, AMPLITUDES_SIZE, "fft");
            break;
        case WINDOW:
            sendJsonArray(fftData.fftWindow, SAMPLES_SIZE, "window");
            break;
        case BARK_SCALE:
            sendJsonArray(fftData.barkScale, AMPLITUDES_SIZE, "bark");
            break;
        default: break;
    }
    info[2] = (int16_t) (millis() - calcTime);
    info[3] = fftArea;
    sendJsonArray(info, 5, "info");
}


void loop() {
    execEncoder(a2dp_sink);
//    execIrCommand(colorModes);
    execIrCommandTest();
    switch (colorModes.mode) {
        case OFF_MODE:
        case COLOR_MODE: break;
        case COLOR_MUSIC_MODE: colorMusic(); break;
        case FLASH_MODE: rainbowColorMode(leds, colorModes.rainbow); break;
//        case FLASH_MODE: flashColorMode(colorModes.flash); break;
        case STROBE_MODE: strobeColorMode(colorModes.strobe); break;
        case FADE_MODE: fadeColorMode(colorModes.fade); break;
        case SMOOTH_MODE: smoothColorMode(colorModes.smooth); break;
    }
}


