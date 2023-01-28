#include "configs.h"
#include "imports.h"


CRGB leds[NUM_LEDS];

void setup() {
    Serial.begin(500000);
    setupA2DP();
    a2dp_sink.set_volume(64);
//    setupEncoder();

    CFastLED::addLeds <WS2812, WS2812B_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.showColor(CRGB::Black);
    IrReceiver.begin(IR_RECEIVE_PIN);
    setupColorModes();
    setupColorMusic();


    Serial.print('\n');
    Serial.print(BLUETOOTH_DEVICE_NAME);
    Serial.println(" Started!");

    Serial.print("samples size: ");
    Serial.println(SAMPLES_SIZE);
    Serial.print("ampl size: ");
    Serial.println(AMPLITUDES_SIZE);

}

uint8_t fftArea = 0;
bool sendWindow = false;

void colorMusic() {
    if (fftData.samples.fullness != SAMPLES_SIZE) return;
//    sendJsonArray(fftData.samples.left, 256, "samples");
    uint16_t calcTime = millis();
    calculateAmplitudes(fftData.samples.right, fftData.amplitudes.right);
    calculateAmplitudes(fftData.samples.left, fftData.amplitudes.left);
    calcTime = millis() - calcTime;
    Serial.print("Calc time (ms): " + String(calcTime));
    Serial.println(" Area: " + String(fftArea));
    if (sendWindow) {
        sendJsonArray(fftData.fftWindow, SAMPLES_SIZE, "test");
    } else {
        sendAmplitudesArea(fftData.amplitudes.left, AMPLITUDES_SIZE, "test", fftArea);
    }

//    calcTime = millis() - calcTime;
//    Serial.println("Calc time (ms): " + String(calcTime));
    fftData.samples.fullness = 0;
}

void execIrCommandTest() {
    if (!IrReceiver.decode()) return;
    uint16_t command = IrReceiver.decodedIRData.command;
    Serial.println(String(command));
    switch (command) {
        case OFF_BUTTON:    colorModes.mode = COLOR_MODE;           break;
        case ON_BUTTON:     colorModes.mode = COLOR_MUSIC_MODE;     break;
        case 4:             fftData.useWindow = false;              break;
        case 5:             fftData.useWindow = true;               break;
        case 6:             fftData.logarithmResult = false;        break;
        case 7:             fftData.logarithmResult = true;         break;
        case 8:             fftArea = 0;         break;
        case 9:             fftArea = 1;         break;
        case 10:             fftArea = 2;         break;
        case 11:             fftArea = 3;         break;
        case 12: dsps_wind_blackman_f32(fftData.fftWindow, SAMPLES_SIZE); break;
        case 13: dsps_wind_blackman_harris_f32(fftData.fftWindow, SAMPLES_SIZE); break;
        case 14: dsps_wind_blackman_nuttall_f32(fftData.fftWindow, SAMPLES_SIZE); break;
        case 15: dsps_wind_flat_top_f32(fftData.fftWindow, SAMPLES_SIZE); break;
        case 16: dsps_wind_hann_f32(fftData.fftWindow, SAMPLES_SIZE); break;
        case 17: dsps_wind_nuttall_f32(fftData.fftWindow, SAMPLES_SIZE); break;
        case 18: sendWindow = false; break;
        case 19: sendWindow = true; break;


//        case FLASH_BUTTON:  colorModes.mode = FLASH_MODE;       break;
//        case STROBE_BUTTON: colorModes.mode = STROBE_MODE;      break;
//        case FADE_BUTTON:   colorModes.mode = FADE_MODE;        break;
//        case SMOOTH_BUTTON: colorModes.mode = SMOOTH_MODE;      break;
    }
    IrReceiver.resume();
}



void loop() {
//    colorMusic();
    execEncoder(a2dp_sink);
//    execIrCommand(colorModes);
    execIrCommandTest();
    switch (colorModes.mode) {
        case OFF_MODE:
        case COLOR_MODE: break;
        case COLOR_MUSIC_MODE: colorMusic(); break;
//        case FLASH_MODE: rainbowColorMode(colorModes.rainbow); break;
////        case FLASH_MODE: flashColorMode(colorModes.flash); break;
//        case STROBE_MODE: strobeColorMode(colorModes.strobe); break;
//        case FADE_MODE: fadeColorMode(colorModes.fade); break;
//        case SMOOTH_MODE: smoothColorMode(colorModes.smooth); break;
    }
}


