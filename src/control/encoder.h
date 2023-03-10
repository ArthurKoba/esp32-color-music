#ifndef ESP32_COLOR_MUSIC_ENCODER_H
#define ESP32_COLOR_MUSIC_ENCODER_H

#ifndef ENCODER_CLK_PIN
#error ENCODER_CLK_PIN required.
#endif

#ifndef ENCODER_DT_PIN
#error ENCODER_DT_PIN required.
#endif

#ifndef ENCODER_DELTA_TIME_MS
#define ENCODER_DELTA_TIME_MS 50
#endif


struct EncoderData {
    bool isUpdated = false;
    uint8_t rotateState = 0;
    uint32_t lastUpdateAngle=0;
#ifdef ENCODER_BTN_PIN
    bool isPressed = false;
    bool isClicked = false;
    uint32_t lastUpdateBTN=0;
#endif
};


EncoderData encoderData;


void IRAM_ATTR isrEncoderAB() {
    bool dt = digitalRead(ENCODER_DT_PIN);
    if (dt == digitalRead(ENCODER_CLK_PIN)) return;
    uint32_t time = millis();
    if (time - encoderData.lastUpdateAngle < ENCODER_DELTA_TIME_MS) return;
    encoderData.rotateState = dt ? 1 : 2;
    encoderData.isUpdated = true;
    encoderData.lastUpdateAngle = time;
}


#ifdef ENCODER_BTN_PIN
void IRAM_ATTR isrEncoderBTN() {
    uint32_t time = millis();
    if (time - encoderData.lastUpdateBTN < ENCODER_DELTA_TIME_MS) return;
    encoderData.isPressed = !digitalRead(ENCODER_BTN_PIN);
    if (!encoderData.isPressed) encoderData.isClicked = true;
    encoderData.isUpdated = true;
    encoderData.lastUpdateBTN = time;
}
#endif


void setupEncoder() {
    pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
    pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
    attachInterrupt(ENCODER_CLK_PIN, isrEncoderAB, CHANGE);
    attachInterrupt(ENCODER_DT_PIN, isrEncoderAB, CHANGE);
#ifdef ENCODER_BTN_PIN
    pinMode(ENCODER_BTN_PIN, INPUT_PULLUP);
    attachInterrupt(ENCODER_BTN_PIN, isrEncoderBTN, CHANGE);
    encoderData.lastUpdateBTN = millis();
#endif
    encoderData.lastUpdateAngle = millis();
}


void execEncoder(BluetoothA2DPSink &a2dp_sink) {
    if (!encoderData.isUpdated) return;
    int volume = a2dp_sink.get_volume();
    if (encoderData.rotateState) {
        uint8_t step = 5;
        if (encoderData.isPressed) step = 1;
        volume += encoderData.rotateState == 2  ? step : -step;
        a2dp_sink.set_volume(constrain(volume, 0, 127));
        encoderData.rotateState = 0;
    }
    encoderData.isUpdated = false;
    encoderData.isClicked = false;


#endif //ESP32_COLOR_MUSIC_ENCODER_H}