#include "configs.h"
#include "imports.h"

CRGB leds[NUM_LEDS];
ColorModes colorModes;
BluetoothA2DPSink a2dp_sink;

void setup() {
    Serial.begin(500000);
    setupColorModes(colorModes);
    CFastLED::addLeds<WS2812B, WS2812B_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(colorModes.bright);
    FastLED.showColor(CRGB::Black);
    a2dp_sink.set_pin_config(i2sPins);
    a2dp_sink.set_raw_stream_reader(appendSamples);
    a2dp_sink.set_on_volumechange(change_volume);
    a2dp_sink.start(BLUETOOTH_DEVICE_NAME);
    a2dp_sink.set_volume(64);
    setupEncoder();
    IrReceiver.begin(IR_RECEIVE_PIN);
    setupColorMusic(fftData);
    Serial.print('\n');
    Serial.print(BLUETOOTH_DEVICE_NAME);
    Serial.println(" Started!");
}

uint8_t fftArea = 0;
uint8_t info[3];

void colorMusic() {
    if (fftData.samples.fullness != SAMPLES_SIZE) return;
//    sendJsonArray(fftData.samples.left, 256, "samples");

    uint16_t calcTime = millis();
    calculateAmplitudes(fftData.samples.right, fftData.amplitudes.right);
    calculateAmplitudes(fftData.samples.left, fftData.amplitudes.left);
    info[0] = millis() - calcTime;
//    Serial.println("Calc time (ms): " + String(info[0]));

    calcTime = millis();
    if (fftData.sendType == WINDOW) {
        sendJsonArray(fftData.fftWindow, SAMPLES_SIZE, "test");
    } else {
        sendAmplitudesArea(fftData.amplitudes.left, AMPLITUDES_SIZE, "test", fftArea);
    }
    info[1] = millis() - calcTime;
    info[2] = fftArea;
    sendJsonArray(info, 3, "info");

    fftData.samples.fullness = 0;
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
        case 4:
            Serial.println("LOG EXIT");
            fftData.sendType = LOG;
            break;
        case 5:
            Serial.println("BARK EXIT");
            fftData.sendType = BARK;
            break;
        case 6:
            Serial.println("LIN EXIT");
            fftData.sendType = LIN;
            break;
        case 7:
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


