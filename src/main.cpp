#include "configs.h"
#include "imports.h"

CRGB leds[NUM_LEDS];
ColorModes colorModes;
BluetoothA2DPSink a2dp_sink;

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


uint8_t fftArea = 0;
uint8_t fastAmplitudes[AMPLITUDES_SIZE];
uint8_t sendFullAmplitudes = false;
uint16_t timeFullness = 0;
bool useDivision = false;
int16_t info[5];


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
    calcTime = millis();
    switch (fftData.sendType) {
        case WINDOW:
            sendJsonArray(fftData.fftWindow, SAMPLES_SIZE, "window");
            break;
        case BARK_SCALE:
            sendJsonArray(fftData.barkScale, AMPLITUDES_SIZE, "bark");
            break;
        default:
            if (sendFullAmplitudes) {
                sendJsonArray(fastAmplitudes, AMPLITUDES_SIZE, "fft");
            } else {
                sendAmplitudesArea(fastAmplitudes, AMPLITUDES_SIZE, "fft", fftArea);
            }
    }
    info[2] = (int16_t) (millis() - calcTime);
    info[3] = fftArea;
    sendJsonArray(info, 5, "info");
}

void execIrCommandTest() {
    if (!IrReceiver.decode()) return;
    uint16_t command = IrReceiver.decodedIRData.command;
    Serial.println(String(command));
    switch (command) {
        case 0:
            Serial.println("USE WINDOW TRUE");
            fftData.useWindow = true;
            break;
        case 1:
            Serial.println("USE WINDOW FALSE");
            fftData.useWindow = false;
            break;
        case OFF_BUTTON:    colorModes.mode = COLOR_MODE;           break;
        case ON_BUTTON:     colorModes.mode = COLOR_MUSIC_MODE;     break;
        case RED_BUTTON:
            Serial.println("LIN EXIT");
            fftData.sendType = LIN;
            break;
        case GREEN_BUTTON:
            Serial.println("BARK EXIT");
            fftData.sendType = BARK;
            break;
        case BLUE_BUTTON:
            Serial.println("LOG EXIT");
            fftData.sendType = LOG;
            break;
        case WHITE_BUTTON:
            Serial.println("WINDOW EXIT");
            fftData.sendType = WINDOW;
            break;
        case 8:
            Serial.println("AREA 1-256");
            fftArea = 0;
            break;
        case 9:
            Serial.println("AREA 256-512");
            fftArea = 1;
            break;
        case 10:
            Serial.println("AREA 512-768");
            fftArea = 2;
            break;
        case 11:
            Serial.println("AREA 768-1024");
            fftArea = 3;
            break;
        case 12:
            Serial.println("WINDOW dsps_wind_blackman_f32");
            dsps_wind_blackman_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case 13:
            Serial.println("WINDOW dsps_wind_blackman_harris_f32");
            dsps_wind_blackman_harris_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case 14:
            Serial.println("WINDOW dsps_wind_blackman_nuttall_f32");
            dsps_wind_blackman_nuttall_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case 15:
            Serial.println("WINDOW dsps_wind_flat_top_f32");
            dsps_wind_flat_top_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case 16:
            Serial.println("WINDOW dsps_wind_hann_f32");
            dsps_wind_hann_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case 17:
            Serial.println("WINDOW dsps_wind_nuttall_f32");
            dsps_wind_nuttall_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case 18:
            Serial.println("BARK EXIT");
            fftData.sendType = BARK_SCALE;
            break;
    }
    IrReceiver.resume();
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


